#include <bjs/js/Handlers.hpp>
#include <iostream>

namespace bjs::js
{
    FunctionHandler::FunctionHandler(NativeContext Context)
    {
        this->ctx = Context;
    }

    u32 FunctionHandler::GetArgc()
    {
        u32 argc = duk_get_top(this->ctx);
        return argc;
    }

    bool FunctionHandler::CheckArgcEqualOrBigger(u32 Argc)
    {
        bool ok = ((u32)duk_get_top(this->ctx) >= Argc);
        return ok;
    }

    bool FunctionHandler::CheckArgcEqual(u32 Argc)
    {
        bool ok = ((u32)duk_get_top(this->ctx) == Argc);
        return ok;
    }

    Type FunctionHandler::GetArgumentType(u32 Index)
    {
        Type typ = static_cast<Type>(duk_get_type(this->ctx, Index));
        return typ;
    }

    bool FunctionHandler::CheckArgumentType(u32 Index, Type ArgType)
    {
        return (ArgType == this->GetArgumentType(Index));
    }

    bool FunctionHandler::IsConstructorCall()
    {
        return duk_is_constructor_call(this->ctx);
    }

    std::string FunctionHandler::GetString(u32 Index)
    {
        std::string str = "";
        if(duk_get_type(this->ctx, Index) == DUK_TYPE_STRING) str = std::string(duk_safe_to_string(this->ctx, Index));
        return str;
    }

    int FunctionHandler::GetInt(u32 Index)
    {
        int intt = 0;
        if(duk_get_type(this->ctx, Index) == DUK_TYPE_NUMBER) intt = duk_get_int(this->ctx, Index);
        return intt;
    }

    u32 FunctionHandler::GetUInt(u32 Index)
    {
        u32 intt = 0;
        if(duk_get_type(this->ctx, Index) == DUK_TYPE_NUMBER) intt = duk_get_uint(this->ctx, Index);
        return intt;
    }

    double FunctionHandler::GetDouble(u32 Index)
    {
        double flt = 0.0;
        if(duk_get_type(this->ctx, Index) == DUK_TYPE_NUMBER) flt = duk_get_number(this->ctx, Index);
        return flt;
    }

    bool FunctionHandler::GetBoolean(u32 Index)
    {
        bool booln = false;
        if(duk_get_type(this->ctx, Index) == DUK_TYPE_BOOLEAN) booln = duk_get_boolean(this->ctx, Index);
        return booln;
    }

    Buffer FunctionHandler::GetBuffer(u32 Index)
    {
        size_t dsize = 0;
        void *data = duk_get_buffer_data(this->ctx, Index, &dsize);
        Buffer buf = { 0 };
        buf.Pointer = data;
        buf.Size = dsize;
        return buf;
    }

    Callback FunctionHandler::GetCallback(u32 Index)
    {
        if(!duk_is_function(this->ctx, Index)) js::ThrowError(this->ctx, Error::TypeError, "Argument at index " + std::to_string(Index) + " is not a callable function");
        return Callback(this->ctx, Index);
    }

    Object FunctionHandler::GetObject(u32 Index)
    {
        if(!duk_is_object(this->ctx, Index)) js::ThrowError(this->ctx, Error::TypeError, "Argument at index " + std::to_string(Index) + " is not an object");
        return Object(this->ctx, Index);
    }

    Array FunctionHandler::GetArray(u32 Index)
    {
        if(!duk_is_array(this->ctx, Index)) js::ThrowError(this->ctx, Error::TypeError, "Argument at index " + std::to_string(Index) + " is not an array");
        return Array(this->ctx, Index);
    }

    void FunctionHandler::ReturnString(std::string Value)
    {
        duk_push_string(this->ctx, Value.c_str());
    }

    void FunctionHandler::ReturnInt(int Value)
    {
        duk_push_int(this->ctx, Value);
    }

    void FunctionHandler::ReturnUInt(u32 Value)
    {
        duk_push_uint(this->ctx, Value);
    }

    void FunctionHandler::ReturnDouble(double Value)
    {
        duk_push_number(this->ctx, Value);
    }

    void FunctionHandler::ReturnBoolean(bool Value)
    {
        duk_push_boolean(this->ctx, Value);
    }

    void FunctionHandler::ReturnArray(Array &Value)
    {
        Value.Initialize();
    }

    void FunctionHandler::ReturnObject(Object &Value)
    {
        Value.Initialize();
    }

    void FunctionHandler::ReturnBuffer(void *Pointer, size_t Size)
    {
        void *bufptr = duk_push_fixed_buffer(this->ctx, Size);
        memcpy(bufptr, Pointer, Size);
    }

    void FunctionHandler::ReturnUndefined()
    {
        duk_push_undefined(this->ctx);
    }

    void FunctionHandler::ReturnNull()
    {
        duk_push_null(this->ctx);
    }

    void FunctionHandler::ReturnNaN()
    {
        duk_push_nan(this->ctx);
    }

    NativeContext FunctionHandler::GetContext()
    {
        return this->ctx;
    }

    ClassHandler::ClassHandler(NativeContext Context) : FunctionHandler(Context)
    {
        duk_push_this(Context);
        this->tempid = 0;
    }

    void ClassHandler::SetPropertyString(std::string Name, std::string Value)
    {
        duk_push_string(this->ctx, Value.c_str());
        duk_put_prop_string(this->ctx, -2, Name.c_str());
    }

    void ClassHandler::SetPropertyInt(std::string Name, int Value)
    {
        duk_push_int(this->ctx, Value);
        duk_put_prop_string(this->ctx, -2, Name.c_str());
    }

    void ClassHandler::SetPropertyUInt(std::string Name, u32 Value)
    {
        duk_push_uint(this->ctx, Value);
        duk_put_prop_string(this->ctx, -2, Name.c_str());
    }

    void ClassHandler::SetPropertyBoolean(std::string Name, bool Value)
    {
        duk_push_boolean(this->ctx, Value);
        duk_put_prop_string(this->ctx, -2, Name.c_str());
    }

    void ClassHandler::SetPropertyUndefined(std::string Name)
    {
        duk_push_undefined(this->ctx);
        duk_put_prop_string(this->ctx, -2, Name.c_str());
    }

    void ClassHandler::SetPropertyNull(std::string Name)
    {
        duk_push_null(this->ctx);
        duk_put_prop_string(this->ctx, -2, Name.c_str());
    }

    void ClassHandler::SetPropertyNaN(std::string Name)
    {
        duk_push_nan(this->ctx);
        duk_put_prop_string(this->ctx, -2, Name.c_str());
    }

    void ClassHandler::InitializeSettingPropertyArray(std::string Name, Array &Value)
    {
        Value.Initialize();
        this->tempname = Name;
        this->tempid = Value.GetId();
    }

    void ClassHandler::FinalizeSettingPropertyArray()
    {
        duk_put_prop_string(this->ctx, -2, this->tempname.c_str());
    }

    void ClassHandler::InitializetSettingPropertyObject(std::string Name, Object &Value)
    {
        Value.Initialize();
        this->tempname = Name;
        this->tempid = Value.GetId();
    }

    void ClassHandler::FinalizeSettingPropertyObject()
    {
        duk_put_prop_string(this->ctx, this->tempid, this->tempname.c_str());
    }

    std::string ClassHandler::GetPropertyString(std::string Name)
    {
        duk_get_prop_string(this->ctx, -1, Name.c_str());
        std::string prop = std::string(duk_safe_to_string(this->ctx, -1));
        duk_pop(this->ctx);
        return prop;
    }

    int ClassHandler::GetPropertyInt(std::string Name)
    {
        duk_get_prop_string(this->ctx, -1, Name.c_str());
        int prop = duk_to_int(this->ctx, -1);
        duk_pop(this->ctx);
        return prop;
    }

    u32 ClassHandler::GetPropertyUInt(std::string Name)
    {
        duk_get_prop_string(this->ctx, -1, Name.c_str());
        u32 prop = duk_to_uint(this->ctx, -1);
        duk_pop(this->ctx);
        return prop;
    }

    double ClassHandler::GetPropertyDouble(std::string Name)
    {
        duk_get_prop_string(this->ctx, -1, Name.c_str());
        double prop = duk_to_number(this->ctx, -1);
        duk_pop(this->ctx);
        return prop;
    }

    bool ClassHandler::GetPropertyBoolean(std::string Name)
    {
        duk_get_prop_string(this->ctx, -1, Name.c_str());
        bool prop = duk_to_boolean(this->ctx, -1);
        duk_pop(this->ctx);
        return prop;
    }
}
#include <bjs/js/Handlers.hpp>

namespace bjs::js
{
	FunctionHandler::FunctionHandler(NativeContext Context)
	{
		this->ctx = Context;
		this->count = -1;
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
		Type typ = (Type)duk_get_type(this->ctx, Index);
		return typ;
	}

	bool FunctionHandler::CheckArgumentType(u32 Index, Type ArgType)
	{
		Type typ = GetArgumentType(Index);
		bool check = (typ == ArgType);
		return check;
	}

	bool FunctionHandler::IsConstructorCall()
	{
		bool ctor = duk_is_constructor_call(this->ctx);
		return ctor;
	}

	std::string FunctionHandler::GetString(u32 Index)
	{
		std::string str = "";
		if(duk_get_type(this->ctx, Index) == DUK_TYPE_STRING) str = std::string(duk_safe_to_string(this->ctx, Index));
		count--;
		return str;
	}

	s64 FunctionHandler::GetInt(u32 Index)
	{
		s64 intt = 0;
		if(duk_get_type(this->ctx, Index) == DUK_TYPE_NUMBER) intt = duk_get_int(this->ctx, Index);
		count--;
		return intt;
	}

	u64 FunctionHandler::GetUInt(u32 Index)
	{
		u64 intt = 0;
		if(duk_get_type(this->ctx, Index) == DUK_TYPE_NUMBER) intt = duk_get_uint(this->ctx, Index);
		count--;
		return intt;
	}

	double FunctionHandler::GetDouble(u32 Index)
	{
		double intt = 0.0;
		if(duk_get_type(this->ctx, Index) == DUK_TYPE_NUMBER) intt = duk_get_number(this->ctx, Index);
		count--;
		return intt;
	}

	bool FunctionHandler::GetBoolean(u32 Index)
	{
		bool booln = false;
		if(duk_get_type(this->ctx, Index) == DUK_TYPE_BOOLEAN) booln = duk_get_boolean(this->ctx, Index);
		count--;
		return booln;
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

	void FunctionHandler::ReturnInt(s64 Value)
	{
		duk_push_int(this->ctx, Value);
	}

	void FunctionHandler::ReturnUInt(u64 Value)
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
		this->propcount = -1;
		this->tempid = 0;
	}

	void ClassHandler::SetPropertyString(std::string Name, std::string Value)
	{
		duk_push_string(this->ctx, Value.c_str());
		duk_put_prop_string(this->ctx, -2, Name.c_str());
	}

	void ClassHandler::SetPropertyInt(std::string Name, s64 Value)
	{
		duk_push_int(this->ctx, Value);
		duk_put_prop_string(this->ctx, -2, Name.c_str());
	}

	void ClassHandler::SetPropertyUInt(std::string Name, u64 Value)
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
		duk_get_prop_string(this->ctx, this->propcount, Name.c_str());
		std::string prop = std::string(duk_safe_to_string(this->ctx, -1));
		this->propcount--;
		return prop;
	}

	s64 ClassHandler::GetPropertyInt(std::string Name)
	{
		duk_get_prop_string(this->ctx, this->propcount, Name.c_str());
		s64 prop = duk_to_int(this->ctx, -1);
		this->propcount--;
		return prop;
	}

	u64 ClassHandler::GetPropertyUInt(std::string Name)
	{
		duk_get_prop_string(this->ctx, this->propcount, Name.c_str());
		u64 prop = duk_to_uint(this->ctx, -1);
		this->propcount--;
		return prop;
	}

	double ClassHandler::GetPropertyDouble(std::string Name)
	{
		duk_get_prop_string(this->ctx, this->propcount, Name.c_str());
		double prop = duk_to_number(this->ctx, -1);
		this->propcount--;
		return prop;
	}

	bool ClassHandler::GetPropertyBoolean(std::string Name)
	{
		duk_get_prop_string(this->ctx, this->propcount, Name.c_str());
		bool prop = duk_to_boolean(this->ctx, -1);
		this->propcount--;
		return prop;
	}
}
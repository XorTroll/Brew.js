#include <bjs/js/Types.hpp>

namespace bjs::js
{
    Array::Array(NativeContext Context)
    {
        this->ctx = Context;
        this->start = false;
        this->arridx = 0;
        this->idx = 0;
        this->padd = true;
        this->uidx = 0;
    }

    Array::Array(NativeContext Context, u32 Index)
    {
        this->ctx = Context;
        this->start = false;
        this->arridx = 0;
        this->idx = 0;
        this->padd = false;
        this->uidx = Index;
    }

    void Array::Initialize()
    {
        if(!this->padd) return;
        if(!this->start)
        {
            arridx = duk_push_array(this->ctx);
            idx = 0;
            start = true;
        }
    }

    void Array::AddString(std::string Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_string(this->ctx, Value.c_str());
        duk_put_prop_index(this->ctx, arridx, idx);
        idx++;
    }

    void Array::AddInt(int Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_int(this->ctx, Value);
        duk_put_prop_index(this->ctx, arridx, idx);
        idx++;
    }

    void Array::AddUInt(u32 Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_uint(this->ctx, Value);
        duk_put_prop_index(this->ctx, arridx, idx);
        idx++;
    }

    void Array::AddDouble(double Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_number(this->ctx, Value);
        duk_put_prop_index(this->ctx, arridx, idx);
        idx++;
    }

    void Array::AddBoolean(bool Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_boolean(this->ctx, Value);
        duk_put_prop_index(this->ctx, arridx, idx);
        idx++;
    }

    void Array::AddFunction(NativeFunction Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_c_function(this->ctx, Value, DUK_VARARGS);
        duk_put_prop_index(this->ctx, arridx, idx);
        idx++;
    }

    void Array::AddUndefined()
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_undefined(this->ctx);
        duk_put_prop_index(this->ctx, arridx, idx);
        idx++;
    }

    void Array::AddNull()
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_null(this->ctx);
        duk_put_prop_index(this->ctx, arridx, idx);
        idx++;
    }

    void Array::AddNaN()
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_nan(this->ctx);
        duk_put_prop_index(this->ctx, arridx, idx);
        idx++;
    }

    void Array::Close()
    {
        if(!this->padd) return;
        duk_pop(this->ctx);
    }

    std::string Array::GetString(u32 Index)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Array not accessed as a function argument");
        duk_get_prop_index(this->ctx, this->uidx, Index);
        return std::string(duk_to_string(this->ctx, -1));
    }

    int Array::GetInt(u32 Index)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Array not accessed as a function argument");
        duk_get_prop_index(this->ctx, this->uidx, Index);
        return duk_to_int(this->ctx, -1);
    }

    u32 Array::GetUInt(u32 Index)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Array not accessed as a function argument");
        duk_get_prop_index(this->ctx, this->uidx, Index);
        return duk_to_uint(this->ctx, -1);
    }

    double Array::GetDouble(u32 Index)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Array not accessed as a function argument");
        duk_get_prop_index(this->ctx, this->uidx, Index);
        return duk_to_number(this->ctx, -1);
    }

    bool Array::GetBoolean(u32 Index)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Array not accessed as a function argument");
        duk_get_prop_index(this->ctx, this->uidx, Index);
        return duk_to_boolean(this->ctx, -1);
    }

    Array Array::GetArray(u32 Index)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Array not accessed as a function argument");
        duk_get_prop_index(this->ctx, this->uidx, Index);
        return Array(this->ctx, -1);
    }

    Object Array::GetObject(u32 Index)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Array not accessed as a function argument");
        duk_get_prop_index(this->ctx, this->uidx, Index);
        return Object(this->ctx, -1);
    }

    std::string Array::AsJSON()
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Array not accessed as a function argument");
        duk_json_encode(this->ctx, this->uidx);
        return std::string(duk_to_string(this->ctx, -1));
    }

    int Array::GetId()
    {
        if(!this->padd) js::ThrowError(this->ctx, Error::CommonError, "Array accessed as a function argument");
        return this->arridx;
    }

    NativeContext Array::GetContext()
    {
        return this->ctx;
    }

    Object::Object(NativeContext Context)
    {
        this->ctx = Context;
        this->start = false;
        this->padd = true;
        this->idx = 0;
        this->objidx = 0;
    }

    Object::Object(NativeContext Context, u32 Index)
    {
        this->ctx = Context;
        this->start = false;
        this->padd = false;
        this->idx = Index;
        this->objidx = 0;
    }

    void Object::Initialize()
    {
        if(!this->padd) return;
        if(!this->start)
        {
            this->objidx = duk_push_object(this->ctx);
            start = true;
        }
    }

    void Object::AddString(std::string Name, std::string Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_string(this->ctx, Value.c_str());
        duk_put_prop_string(this->ctx, this->objidx, Name.c_str());
    }

    void Object::AddInt(std::string Name, int Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_int(this->ctx, Value);
        duk_put_prop_string(this->ctx, this->objidx, Name.c_str());
    }

    void Object::AddUInt(std::string Name, u32 Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_uint(this->ctx, Value);
        duk_put_prop_string(this->ctx, this->objidx, Name.c_str());
    }

    void Object::AddDouble(std::string Name, double Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_number(this->ctx, Value);
        duk_put_prop_string(this->ctx, this->objidx, Name.c_str());
    }

    void Object::AddBoolean(std::string Name, bool Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_boolean(this->ctx, Value);
        duk_put_prop_string(this->ctx, this->objidx, Name.c_str());
    }

    void Object::AddFunction(std::string Name, NativeFunction Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_c_function(this->ctx, Value, DUK_VARARGS);
        duk_put_prop_string(this->ctx, this->objidx, Name.c_str());
    }

    void Object::AddUndefined(std::string Name)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_undefined(this->ctx);
        duk_put_prop_string(this->ctx, this->objidx, Name.c_str());
    }

    void Object::AddNull(std::string Name)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_null(this->ctx);
        duk_put_prop_string(this->ctx, this->objidx, Name.c_str());
    }

    void Object::AddNaN(std::string Name)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        duk_push_nan(this->ctx);
        duk_put_prop_string(this->ctx, this->objidx, Name.c_str());
    }

    void Object::InitializeAddingArray(std::string Name, Array &Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        Value.Initialize();
        this->tempname = Name;
    }

    void Object::FinalizeAddingArray()
    {
        duk_put_prop_string(this->ctx, this->objidx, this->tempname.c_str());
    }

    void Object::InitializeAddingObject(std::string Name, Object &Value)
    {
        if(!this->padd) return;
        if(!this->start) this->Initialize();
        Value.Initialize();
        this->tempname = Name;
    }

    void Object::FinalizeAddingObject()
    {
        duk_put_prop_string(this->ctx, this->objidx, this->tempname.c_str());
    }

    void Object::Close()
    {
        if(!this->padd) return;
        duk_pop(this->ctx);
    }

    std::string Object::GetString(std::string Name)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Object not accessed as a function argument");
        duk_get_prop_string(this->ctx, this->idx, Name.c_str());
        return std::string(duk_to_string(this->ctx, -1));
    }

    int Object::GetInt(std::string Name)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Object not accessed as a function argument");
        duk_get_prop_string(this->ctx, this->idx, Name.c_str());
        return duk_to_int(this->ctx, -1);
    }

    u32 Object::GetUInt(std::string Name)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Object not accessed as a function argument");
        duk_get_prop_string(this->ctx, this->idx, Name.c_str());
        return duk_to_uint(this->ctx, -1);
    }

    double Object::GetDouble(std::string Name)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Object not accessed as a function argument");
        duk_get_prop_string(this->ctx, this->idx, Name.c_str());
        return duk_to_number(this->ctx, -1);
    }

    bool Object::GetBoolean(std::string Name)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Object not accessed as a function argument");
        duk_get_prop_string(this->ctx, this->idx, Name.c_str());
        return duk_to_boolean(this->ctx, -1);
    }

    Array Object::GetArray(std::string Name)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Object not accessed as a function argument");
        duk_get_prop_string(this->ctx, this->idx, Name.c_str());
        return Array(this->ctx, -1);
    }

    Object Object::GetObject(std::string Name)
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Object not accessed as a function argument");
        duk_get_prop_string(this->ctx, this->idx, Name.c_str());
        return Object(this->ctx, -1);
    }

    std::string Object::AsJSON()
    {
        if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Object not accessed as a function argument");
        duk_json_encode(this->ctx, this->idx);
        return std::string(duk_to_string(this->ctx, -1));
    }

    int Object::GetId()
    {
        if(!this->padd) js::ThrowError(this->ctx, Error::CommonError, "Object accessed as a function argument");
        return this->objidx;
    }

    NativeContext Object::GetContext()
    {
        return this->ctx;
    }

    Callback::Callback(NativeContext Context, u32 Index)
    {
        this->ctx = Context;
        this->iter = 0;
    }

    void Callback::AddString(std::string Value)
    {
        this->reg.push_back(0);
        this->strs.insert(std::pair<u32, std::string>(this->iter, Value));
        this->iter++;
    }

    void Callback::AddInt(int Value)
    {
        this->reg.push_back(1);
        this->ints.insert(std::pair<u32, int>(this->iter, Value));
        this->iter++;
    }

    void Callback::AddUInt(u32 Value)
    {
        this->reg.push_back(2);
        this->uints.insert(std::pair<u32, u32>(this->iter, Value));
        this->iter++;
    }

    void Callback::AddDouble(double Value)
    {
        this->reg.push_back(3);
        this->dbls.insert(std::pair<u32, double>(this->iter, Value));
        this->iter++;
    }

    void Callback::AddBoolean(bool Value)
    {
        this->reg.push_back(4);
        this->bls.insert(std::pair<u32, bool>(this->iter, Value));
        this->iter++;
    }

    void Callback::AddUndefined()
    {
        this->reg.push_back(5);
        this->udfs.push_back(this->iter);
        this->iter++;
    }

    void Callback::AddNull()
    {
        this->reg.push_back(6);
        this->nlls.push_back(this->iter);
        this->iter++;
    }

    void Callback::AddNaN()
    {
        this->reg.push_back(7);
        this->nans.push_back(this->iter);
        this->iter++;
    }

    void Callback::Call()
    {
        if(!this->reg.empty()) for(u32 i = 0; i < this->reg.size(); i++)
        {
            u32 rtype = this->reg[i];
            switch(rtype)
            {
                case 0:
                {
                    duk_push_string(this->ctx, this->strs[i].c_str());
                    break;
                }
                case 1:
                {
                    duk_push_int(this->ctx, this->ints[i]);
                    break;
                }
                case 2:
                {
                    duk_push_uint(this->ctx, this->uints[i]);
                    break;
                }
                case 3:
                {
                    duk_push_number(this->ctx, this->dbls[i]);
                    break;
                }
                case 4:
                {
                    duk_push_boolean(this->ctx, this->bls[i]);
                    break;
                }
                case 5:
                {
                    duk_push_undefined(this->ctx);
                    break;
                }
                case 6:
                {
                    duk_push_null(this->ctx);
                    break;
                }
                case 7:
                {
                    duk_push_nan(this->ctx);
                    break;
                }
            }
        }
        duk_pcall(this->ctx, this->iter);
    }

    void Callback::CallNew()
    {
        if(!this->reg.empty()) for(u32 i = 0; i < this->reg.size(); i++)
        {
            u32 rtype = this->reg[i];
            switch(rtype)
            {
                case 0:
                {
                    duk_push_string(this->ctx, this->strs[i].c_str());
                    break;
                }
                case 1:
                {
                    duk_push_int(this->ctx, this->ints[i]);
                    break;
                }
                case 2:
                {
                    duk_push_uint(this->ctx, this->uints[i]);
                    break;
                }
                case 3:
                {
                    duk_push_number(this->ctx, this->dbls[i]);
                    break;
                }
                case 4:
                {
                    duk_push_boolean(this->ctx, this->bls[i]);
                    break;
                }
                case 5:
                {
                    duk_push_undefined(this->ctx);
                    break;
                }
                case 6:
                {
                    duk_push_null(this->ctx);
                    break;
                }
                case 7:
                {
                    duk_push_nan(this->ctx);
                    break;
                }
            }
        }
        duk_pnew(this->ctx, this->iter);
    }

    NativeContext Callback::GetContext()
    {
        return this->ctx;
    }

    std::vector<std::string> Callback::GetStringArguments()
    {
        std::vector<std::string> vstrs;
        if(!this->strs.empty()) for(auto const& str : this->strs) vstrs.push_back(str.second);
        return vstrs;
    }

    std::vector<int> Callback::GetIntArguments()
    {
        std::vector<int> vints;
        if(!this->ints.empty()) for(auto const& vint : this->ints) vints.push_back(vint.second);
        return vints;
    }

    std::vector<u32> Callback::GetUIntArguments()
    {
        std::vector<u32> vuints;
        if(!this->uints.empty()) for(auto const& vuint : this->uints) vuints.push_back(vuint.second);
        return vuints;
    }

    std::vector<double> Callback::GetDoubleArguments()
    {
        std::vector<double> vdbls;
        if(!this->dbls.empty()) for(auto const& dbl : this->dbls) vdbls.push_back(dbl.second);
        return vdbls;
    }

    std::vector<bool> Callback::GetBooleanArguments()
    {
        std::vector<bool> vbls;
        if(!this->bls.empty()) for(auto const& bl : this->bls) vbls.push_back(bl.second);
        return vbls;
    }

    u32 Callback::GetUndefinedArgumentCount()
    {
        return this->udfs.size();
    }

    u32 Callback::GetNullArgumentCount()
    {
        return this->nlls.size();
    }

    u32 Callback::GetNaNArgumentCount()
    {
        return this->nans.size();
    }
}
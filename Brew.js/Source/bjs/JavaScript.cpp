#include <bjs/JavaScript.hpp>

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

	void Array::AddInt(s64 Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_int(this->ctx, Value);
		duk_put_prop_index(this->ctx, arridx, idx);
		idx++;
	}

	void Array::AddUInt(u64 Value)
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

	s64 Array::GetInt(u32 Index)
	{
		if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Array not accessed as a function argument");
		duk_get_prop_index(this->ctx, this->uidx, Index);
		return duk_to_int(this->ctx, -1);
	}

	u64 Array::GetUInt(u32 Index)
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

	void Object::AddInt(std::string Name, s64 Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_int(this->ctx, Value);
		duk_put_prop_string(this->ctx, this->objidx, Name.c_str());
	}

	void Object::AddUInt(std::string Name, u64 Value)
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

	s64 Object::GetInt(std::string Name)
	{
		if(this->padd) js::ThrowError(this->ctx, Error::CommonError, "Object not accessed as a function argument");
		duk_get_prop_string(this->ctx, this->idx, Name.c_str());
		return duk_to_int(this->ctx, -1);
	}

	u64 Object::GetUInt(std::string Name)
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

	void Callback::AddInt(s64 Value)
	{
		this->reg.push_back(1);
		this->ints.insert(std::pair<u32, s64>(this->iter, Value));
		this->iter++;
	}

	void Callback::AddUInt(u64 Value)
	{
		this->reg.push_back(2);
		this->uints.insert(std::pair<u32, u64>(this->iter, Value));
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

	std::vector<s64> Callback::GetIntArguments()
	{
		std::vector<s64> vints;
		if(!this->ints.empty()) for(auto const& vint : this->ints) vints.push_back(vint.second);
		return vints;
	}

	std::vector<u64> Callback::GetUIntArguments()
	{
		std::vector<u64> vuints;
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

	Class::Class(std::string Name, NativeFunction Constructor)
	{
		this->name = Name;
		this->ctor = Constructor;
	}

	void Class::AddString(std::string Name, std::string Value)
	{
		this->strs.insert(std::pair<std::string, std::string>(Name, Value));
	}

	void Class::AddInt(std::string Name, s64 Value)
	{
		this->ints.insert(std::pair<std::string, s64>(Name, Value));
	}

	void Class::AddUInt(std::string Name, u64 Value)
	{
		this->uints.insert(std::pair<std::string, u64>(Name, Value));
	}

	void Class::AddDouble(std::string Name, double Value)
	{
		this->dbls.insert(std::pair<std::string, double>(Name, Value));
	}

	void Class::AddBoolean(std::string Name, bool Value)
	{
		this->bls.insert(std::pair<std::string, bool>(Name, Value));
	}

	void Class::AddFunction(std::string Name, NativeFunction Value)
	{
		this->fncs.insert(std::pair<std::string, NativeFunction>(Name, Value));
	}

	void Class::AddUndefined(std::string Name)
	{
		this->udfs.push_back(Name);
	}

	void Class::AddNull(std::string Name)
	{
		this->nlls.push_back(Name);
	}

	void Class::AddNaN(std::string Name)
	{
		this->nans.push_back(Name);
	}

	NativeFunction Class::GetConstructorFunction()
	{
		return this->ctor;
	}

	std::map<std::string, std::string> Class::GetStrings()
	{
		return this->strs;
	}

	std::map<std::string, s64> Class::GetInts()
	{
		return this->ints;
	}

	std::map<std::string, u64> Class::GetUInts()
	{
		return this->uints;
	}

	std::map<std::string, double> Class::GetDoubles()
	{
		return this->dbls;
	}

	std::map<std::string, bool> Class::GetBooleans()
	{
		return this->bls;
	}

	std::map<std::string, NativeFunction> Class::GetFunctions()
	{
		return this->fncs;
	}

	std::vector<std::string> Class::GetUndefineds()
	{
		return this->udfs;
	}

	std::vector<std::string> Class::GetNulls()
	{
		return this->nlls;
	}

	std::vector<std::string> Class::GetNaNs()
	{
		return this->nans;
	}

	std::string Class::GetName()
	{
		return this->name;
	}

	Module::Module(std::string Module)
	{
		this->name = Module;
	}

	void Module::PushString(std::string Name, std::string Value)
	{
		this->strs.insert(std::pair<std::string, std::string>(Name, Value));
	}

	void Module::PushInt(std::string Name, s64 Value)
	{
		this->ints.insert(std::pair<std::string, s64>(Name, Value));
	}

	void Module::PushUInt(std::string Name, u64 Value)
	{
		this->uints.insert(std::pair<std::string, u64>(Name, Value));
	}

	void Module::PushDouble(std::string Name, double Value)
	{
		this->dbls.insert(std::pair<std::string, double>(Name, Value));
	}

	void Module::PushBoolean(std::string Name, bool Value)
	{
		this->bls.insert(std::pair<std::string, bool>(Name, Value));
	}

	void Module::PushFunction(std::string Name, NativeFunction Value)
	{
		this->fncs.insert(std::pair<std::string, NativeFunction>(Name, Value));
	}

	void Module::PushUndefined(std::string Name)
	{
		this->udfs.push_back(Name);
	}

	void Module::PushNull(std::string Name)
	{
		this->nlls.push_back(Name);
	}

	void Module::PushNaN(std::string Name)
	{
		this->nans.push_back(Name);
	}

	void Module::PushClass(Class &Value)
	{
		this->clss.push_back(Value);
	}

	std::map<std::string, std::string> Module::GetStrings()
	{
		return this->strs;
	}

	std::map<std::string, s64> Module::GetInts()
	{
		return this->ints;
	}

	std::map<std::string, u64> Module::GetUInts()
	{
		return this->uints;
	}

	std::map<std::string, double> Module::GetDoubles()
	{
		return this->dbls;
	}

	std::map<std::string, bool> Module::GetBooleans()
	{
		return this->bls;
	}

	std::map<std::string, NativeFunction> Module::GetFunctions()
	{
		return this->fncs;
	}

	std::vector<std::string> Module::GetUndefineds()
	{
		return this->udfs;
	}

	std::vector<std::string> Module::GetNulls()
	{
		return this->nlls;
	}

	std::vector<std::string> Module::GetNaNs()
	{
		return this->nans;
	}

	std::vector<Class> Module::GetClasses()
	{
		return this->clss;
	}

	std::string Module::GetName()
	{
		return this->name;
	}

	GlobalObject::GlobalObject(NativeContext Context)
	{
		this->ctx = Context;
	}

	void GlobalObject::PushString(std::string Name, std::string Value)
	{
		duk_push_string(this->ctx, Value.c_str());
		duk_put_global_string(this->ctx, Name.c_str());
	}

	void GlobalObject::PushInt(std::string Name, s64 Value)
	{
		duk_push_int(this->ctx, Value);
		duk_put_global_string(this->ctx, Name.c_str());
	}

	void GlobalObject::PushUInt(std::string Name, u64 Value)
	{
		duk_push_uint(this->ctx, Value);
		duk_put_global_string(this->ctx, Name.c_str());
	}

	void GlobalObject::PushDouble(std::string Name, double Value)
	{
		duk_push_number(this->ctx, Value);
		duk_put_global_string(this->ctx, Name.c_str());
	}

	void GlobalObject::PushBoolean(std::string Name, bool Value)
	{
		duk_push_boolean(this->ctx, Value);
		duk_put_global_string(this->ctx, Name.c_str());
	}

	void GlobalObject::PushFunction(std::string Name, NativeFunction Value)
	{
		duk_push_c_function(this->ctx, (duk_c_function)Value, DUK_VARARGS);
		duk_put_global_string(this->ctx, Name.c_str());
	}

	void GlobalObject::PushUndefined(std::string Name)
	{
		duk_push_undefined(this->ctx);
		duk_put_global_string(this->ctx, Name.c_str());
	}

	void GlobalObject::PushNull(std::string Name)
	{
		duk_push_null(this->ctx);
		duk_put_global_string(this->ctx, Name.c_str());
	}

	void GlobalObject::PushNaN(std::string Name)
	{
		duk_push_nan(this->ctx);
		duk_put_global_string(this->ctx, Name.c_str());
	}

	void GlobalObject::PushModule(Module &Module)
	{
		duk_idx_t objidx = duk_push_object(this->ctx);
		std::map<std::string, std::string> strs = Module.GetStrings();
		if(!strs.empty()) for(auto const& str : strs)
		{
			duk_push_string(this->ctx, str.second.c_str());
			duk_put_prop_string(this->ctx, objidx, str.first.c_str());
		}
		std::map<std::string, s64> ints = Module.GetInts();
		if(!ints.empty()) for(auto const& intt : ints)
		{
			duk_push_int(this->ctx, intt.second);
			duk_put_prop_string(this->ctx, objidx, intt.first.c_str());
		}
		std::map<std::string, u64> uints = Module.GetUInts();
		if(!uints.empty()) for(auto const& uint : uints)
		{
			duk_push_uint(this->ctx, uint.second);
			duk_put_prop_string(this->ctx, objidx, uint.first.c_str());
		}
		std::map<std::string, double> dbls = Module.GetDoubles();
		if(!dbls.empty()) for(auto const& dbl : dbls)
		{
			duk_push_number(this->ctx, dbl.second);
			duk_put_prop_string(this->ctx, objidx, dbl.first.c_str());
		}
		std::map<std::string, bool> bls = Module.GetBooleans();
		if(!bls.empty()) for(auto const& bol : bls)
		{
			duk_push_boolean(this->ctx, bol.second);
			duk_put_prop_string(this->ctx, objidx, bol.first.c_str());
		}
		std::map<std::string, NativeFunction> fncs = Module.GetFunctions();
		if(!fncs.empty()) for(auto const& func : fncs)
		{
			duk_push_c_function(this->ctx, func.second, DUK_VARARGS);
			duk_put_prop_string(this->ctx, objidx, func.first.c_str());
		}
		std::vector<std::string> udfs = Module.GetUndefineds();
		if(!udfs.empty()) for(u32 i = 0; i < udfs.size(); i++)
		{
			duk_push_undefined(this->ctx);
			duk_put_prop_string(this->ctx, objidx, udfs[i].c_str());
		}
		std::vector<std::string> nlls = Module.GetNulls();
		if(!nlls.empty()) for(u32 i = 0; i < nlls.size(); i++)
		{
			duk_push_null(this->ctx);
			duk_put_prop_string(this->ctx, objidx, nlls[i].c_str());
		}
		std::vector<std::string> nans = Module.GetNaNs();
		if(!nans.empty()) for(u32 i = 0; i < nans.size(); i++)
		{
			duk_push_nan(this->ctx);
			duk_put_prop_string(this->ctx, objidx, nans[i].c_str());
		}
		duk_put_global_string(this->ctx, Module.GetName().c_str());
	}

	void GlobalObject::InitializePushingObject(std::string Name, Object &Value)
	{
		Value.Initialize();
		this->tempname = Name;
	}

	void GlobalObject::FinalizePushingObject()
	{
		duk_put_global_string(this->ctx, this->tempname.c_str());
	}

	NativeContext GlobalObject::GetContext()
	{
		return this->ctx;
	}

	NativeContext CreateContext()
	{
		return duk_create_heap_default();
	}

	void ThrowError(NativeContext Context, Error ErrorType = Error::CommonError, std::string Message = "An error was thrown.")
	{
		duk_error(Context, static_cast<duk_errcode_t>(ErrorType), Message.c_str());
	}
}
#include <Brew.js/API.hpp>

namespace Brew::API
{
	Array::Array(NativeContext Context)
	{
		this->Context = Context;
		this->start = false;
		this->arridx = 0;
		this->idx = 0;
		this->padd = true;
		this->uidx = 0;
	}

	Array::Array(NativeContext Context, u32 Index)
	{
		this->Context = Context;
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
			arridx = duk_push_array(this->Context);
			idx = 0;
			start = true;
		}
	}

	void Array::AddString(string Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_string(this->Context, Value.c_str());
		duk_put_prop_index(this->Context, arridx, idx);
		idx++;
	}

	void Array::AddInt(s64 Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_int(this->Context, Value);
		duk_put_prop_index(this->Context, arridx, idx);
		idx++;
	}

	void Array::AddUInt(u64 Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_uint(this->Context, Value);
		duk_put_prop_index(this->Context, arridx, idx);
		idx++;
	}

	void Array::AddDouble(double Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_number(this->Context, Value);
		duk_put_prop_index(this->Context, arridx, idx);
		idx++;
	}

	void Array::AddBoolean(bool Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_boolean(this->Context, Value);
		duk_put_prop_index(this->Context, arridx, idx);
		idx++;
	}

	void Array::AddFunction(NativeFunction Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_c_function(this->Context, Value, DUK_VARARGS);
		duk_put_prop_index(this->Context, arridx, idx);
		idx++;
	}

	void Array::AddUndefined()
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_undefined(this->Context);
		duk_put_prop_index(this->Context, arridx, idx);
		idx++;
	}

	void Array::AddNull()
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_null(this->Context);
		duk_put_prop_index(this->Context, arridx, idx);
		idx++;
	}

	void Array::AddNaN()
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_nan(this->Context);
		duk_put_prop_index(this->Context, arridx, idx);
		idx++;
	}

	void Array::Close()
	{
		if(!this->padd) return;
		duk_pop(this->Context);
	}

	string Array::GetString(u32 Index)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Array not accessed as a function argument");
		duk_get_prop_index(this->Context, this->uidx, Index);
		return string(duk_to_string(this->Context, -1));
	}

	s64 Array::GetInt(u32 Index)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Array not accessed as a function argument");
		duk_get_prop_index(this->Context, this->uidx, Index);
		return duk_to_int(this->Context, -1);
	}

	u64 Array::GetUInt(u32 Index)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Array not accessed as a function argument");
		duk_get_prop_index(this->Context, this->uidx, Index);
		return duk_to_uint(this->Context, -1);
	}

	double Array::GetDouble(u32 Index)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Array not accessed as a function argument");
		duk_get_prop_index(this->Context, this->uidx, Index);
		return duk_to_number(this->Context, -1);
	}

	bool Array::GetBoolean(u32 Index)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Array not accessed as a function argument");
		duk_get_prop_index(this->Context, this->uidx, Index);
		return duk_to_boolean(this->Context, -1);
	}

	Array Array::GetArray(u32 Index)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Array not accessed as a function argument");
		duk_get_prop_index(this->Context, this->uidx, Index);
		return Array(this->Context, -1);
	}

	Object Array::GetObject(u32 Index)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Array not accessed as a function argument");
		duk_get_prop_index(this->Context, this->uidx, Index);
		return Object(this->Context, -1);
	}

	string Array::AsJSON()
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Array not accessed as a function argument");
		duk_json_encode(this->Context, this->uidx);
		return string(duk_to_string(this->Context, -1));
	}

	int Array::GetId()
	{
		if(!this->padd) API::ThrowError(this->Context, Error::CommonError, "Array accessed as a function argument");
		return this->arridx;
	}

	Object::Object(NativeContext Context)
	{
		this->Context = Context;
		this->start = false;
		this->padd = true;
		this->idx = 0;
		this->objidx = 0;
	}

	Object::Object(NativeContext Context, u32 Index)
	{
		this->Context = Context;
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
			this->objidx = duk_push_object(this->Context);
			start = true;
		}
	}

	void Object::AddString(string Name, string Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_string(this->Context, Value.c_str());
		duk_put_prop_string(this->Context, this->objidx, Name.c_str());
	}

	void Object::AddInt(string Name, s64 Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_int(this->Context, Value);
		duk_put_prop_string(this->Context, this->objidx, Name.c_str());
	}

	void Object::AddUInt(string Name, u64 Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_uint(this->Context, Value);
		duk_put_prop_string(this->Context, this->objidx, Name.c_str());
	}

	void Object::AddDouble(string Name, double Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_number(this->Context, Value);
		duk_put_prop_string(this->Context, this->objidx, Name.c_str());
	}

	void Object::AddBoolean(string Name, bool Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_boolean(this->Context, Value);
		duk_put_prop_string(this->Context, this->objidx, Name.c_str());
	}

	void Object::AddFunction(string Name, NativeFunction Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_c_function(this->Context, Value, DUK_VARARGS);
		duk_put_prop_string(this->Context, this->objidx, Name.c_str());
	}

	void Object::AddUndefined(string Name)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_undefined(this->Context);
		duk_put_prop_string(this->Context, this->objidx, Name.c_str());
	}

	void Object::AddNull(string Name)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_null(this->Context);
		duk_put_prop_string(this->Context, this->objidx, Name.c_str());
	}

	void Object::AddNaN(string Name)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		duk_push_nan(this->Context);
		duk_put_prop_string(this->Context, this->objidx, Name.c_str());
	}

	void Object::InitializeAddingArray(string Name, Array &Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		Value.Initialize();
		this->tempname = Name;
	}

	void Object::FinalizeAddingArray()
	{
		duk_put_prop_string(this->Context, this->objidx, this->tempname.c_str());
	}

	void Object::InitializeAddingObject(string Name, Object &Value)
	{
		if(!this->padd) return;
		if(!this->start) this->Initialize();
		Value.Initialize();
		this->tempname = Name;
	}

	void Object::FinalizeAddingObject()
	{
		duk_put_prop_string(this->Context, this->objidx, this->tempname.c_str());
	}

	void Object::Close()
	{
		if(!this->padd) return;
		duk_pop(this->Context);
	}

	string Object::GetString(string Name)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Object not accessed as a function argument");
		duk_get_prop_string(this->Context, this->idx, Name.c_str());
		return string(duk_to_string(this->Context, -1));
	}

	s64 Object::GetInt(string Name)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Object not accessed as a function argument");
		duk_get_prop_string(this->Context, this->idx, Name.c_str());
		return duk_to_int(this->Context, -1);
	}

	u64 Object::GetUInt(string Name)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Object not accessed as a function argument");
		duk_get_prop_string(this->Context, this->idx, Name.c_str());
		return duk_to_uint(this->Context, -1);
	}

	double Object::GetDouble(string Name)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Object not accessed as a function argument");
		duk_get_prop_string(this->Context, this->idx, Name.c_str());
		return duk_to_number(this->Context, -1);
	}

	bool Object::GetBoolean(string Name)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Object not accessed as a function argument");
		duk_get_prop_string(this->Context, this->idx, Name.c_str());
		return duk_to_boolean(this->Context, -1);
	}

	Array Object::GetArray(string Name)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Object not accessed as a function argument");
		duk_get_prop_string(this->Context, this->idx, Name.c_str());
		return Array(this->Context, -1);
	}

	Object Object::GetObject(string Name)
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Object not accessed as a function argument");
		duk_get_prop_string(this->Context, this->idx, Name.c_str());
		return Object(this->Context, -1);
	}

	string Object::AsJSON()
	{
		if(this->padd) API::ThrowError(this->Context, Error::CommonError, "Object not accessed as a function argument");
		duk_json_encode(this->Context, this->idx);
		return string(duk_to_string(this->Context, -1));
	}

	int Object::GetId()
	{
		if(!this->padd) API::ThrowError(this->Context, Error::CommonError, "Object accessed as a function argument");
		return this->objidx;
	}

	Callback::Callback(NativeContext Context, u32 Index)
	{
		this->Context = Context;
		this->iter = 0;
	}

	void Callback::AddString(string Value)
	{
		this->reg.push_back(0);
		this->Strings.insert(pair<u32, string>(this->iter, Value));
		this->iter++;
	}

	void Callback::AddInt(s64 Value)
	{
		this->reg.push_back(1);
		this->Ints.insert(pair<u32, s64>(this->iter, Value));
		this->iter++;
	}

	void Callback::AddUInt(u64 Value)
	{
		this->reg.push_back(2);
		this->UInts.insert(pair<u32, u64>(this->iter, Value));
		this->iter++;
	}

	void Callback::AddDouble(double Value)
	{
		this->reg.push_back(3);
		this->Doubles.insert(pair<u32, double>(this->iter, Value));
		this->iter++;
	}

	void Callback::AddBoolean(bool Value)
	{
		this->reg.push_back(4);
		this->Booleans.insert(pair<u32, bool>(this->iter, Value));
		this->iter++;
	}

	void Callback::AddUndefined()
	{
		this->reg.push_back(5);
		this->Undefineds.push_back(this->iter);
		this->iter++;
	}

	void Callback::AddNull()
	{
		this->reg.push_back(6);
		this->Nulls.push_back(this->iter);
		this->iter++;
	}

	void Callback::AddNaN()
	{
		this->reg.push_back(7);
		this->NaNs.push_back(this->iter);
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
					duk_push_string(this->Context, this->Strings[i].c_str());
					break;
				}
				case 1:
				{
					duk_push_int(this->Context, this->Ints[i]);
					break;
				}
				case 2:
				{
					duk_push_uint(this->Context, this->UInts[i]);
					break;
				}
				case 3:
				{
					duk_push_number(this->Context, this->Doubles[i]);
					break;
				}
				case 4:
				{
					duk_push_boolean(this->Context, this->Booleans[i]);
					break;
				}
				case 5:
				{
					duk_push_undefined(this->Context);
					break;
				}
				case 6:
				{
					duk_push_null(this->Context);
					break;
				}
				case 7:
				{
					duk_push_nan(this->Context);
					break;
				}
			}
		}
		duk_pcall(this->Context, this->iter);
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
					duk_push_string(this->Context, this->Strings[i].c_str());
					break;
				}
				case 1:
				{
					duk_push_int(this->Context, this->Ints[i]);
					break;
				}
				case 2:
				{
					duk_push_uint(this->Context, this->UInts[i]);
					break;
				}
				case 3:
				{
					duk_push_number(this->Context, this->Doubles[i]);
					break;
				}
				case 4:
				{
					duk_push_boolean(this->Context, this->Booleans[i]);
					break;
				}
				case 5:
				{
					duk_push_undefined(this->Context);
					break;
				}
				case 6:
				{
					duk_push_null(this->Context);
					break;
				}
				case 7:
				{
					duk_push_nan(this->Context);
					break;
				}
			}
		}
		duk_pnew(this->Context, this->iter);
	}

	FunctionHandler::FunctionHandler(NativeContext Context)
	{
		this->Context = Context;
		this->count = -1;
	}

	u32 FunctionHandler::GetArgc()
	{
		u32 argc = duk_get_top(this->Context);
		return argc;
	}

	bool FunctionHandler::CheckArgcEqualOrBigger(u32 Argc)
	{
		bool ok = ((u32)duk_get_top(this->Context) >= Argc);
		return ok;
	}

	bool FunctionHandler::CheckArgcEqual(u32 Argc)
	{
		bool ok = ((u32)duk_get_top(this->Context) == Argc);
		return ok;
	}

	Type FunctionHandler::GetArgumentType(u32 Index)
	{
		Type typ = (Type)duk_get_type(this->Context, Index);
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
		bool ctor = duk_is_constructor_call(this->Context);
		return ctor;
	}

	string FunctionHandler::GetString(u32 Index)
	{
		string str = "";
		if(duk_get_type(this->Context, Index) == DUK_TYPE_STRING) str = string(duk_safe_to_string(this->Context, Index));
		count--;
		return str;
	}

	s64 FunctionHandler::GetInt(u32 Index)
	{
		s64 intt = 0;
		if(duk_get_type(this->Context, Index) == DUK_TYPE_NUMBER) intt = duk_get_int(this->Context, Index);
		count--;
		return intt;
	}

	u64 FunctionHandler::GetUInt(u32 Index)
	{
		u64 intt = 0;
		if(duk_get_type(this->Context, Index) == DUK_TYPE_NUMBER) intt = duk_get_uint(this->Context, Index);
		count--;
		return intt;
	}

	double FunctionHandler::GetDouble(u32 Index)
	{
		double intt = 0.0;
		if(duk_get_type(this->Context, Index) == DUK_TYPE_NUMBER) intt = duk_get_number(this->Context, Index);
		count--;
		return intt;
	}

	bool FunctionHandler::GetBoolean(u32 Index)
	{
		bool booln = false;
		if(duk_get_type(this->Context, Index) == DUK_TYPE_BOOLEAN) booln = duk_get_boolean(this->Context, Index);
		count--;
		return booln;
	}

	Callback FunctionHandler::GetCallback(u32 Index)
	{
		if(!duk_is_function(this->Context, Index)) API::ThrowError(this->Context, Error::TypeError, "Argument at index " + to_string(Index) + " is not a callable function");
		return Callback(this->Context, Index);
	}

	Object FunctionHandler::GetObject(u32 Index)
	{
		if(!duk_is_object(this->Context, Index)) API::ThrowError(this->Context, Error::TypeError, "Argument at index " + to_string(Index) + " is not an object");
		return Object(this->Context, Index);
	}

	Array FunctionHandler::GetArray(u32 Index)
	{
		if(!duk_is_array(this->Context, Index)) API::ThrowError(this->Context, Error::TypeError, "Argument at index " + to_string(Index) + " is not an array");
		return Array(this->Context, Index);
	}

	void FunctionHandler::ReturnString(string Value)
	{
		duk_push_string(this->Context, Value.c_str());
	}

	void FunctionHandler::ReturnInt(s64 Value)
	{
		duk_push_int(this->Context, Value);
	}

	void FunctionHandler::ReturnUInt(u64 Value)
	{
		duk_push_uint(this->Context, Value);
	}

	void FunctionHandler::ReturnDouble(double Value)
	{
		duk_push_number(this->Context, Value);
	}

	void FunctionHandler::ReturnBoolean(bool Value)
	{
		duk_push_boolean(this->Context, Value);
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
		duk_push_undefined(this->Context);
	}

	void FunctionHandler::ReturnNull()
	{
		duk_push_null(this->Context);
	}

	void FunctionHandler::ReturnNaN()
	{
		duk_push_nan(this->Context);
	}

	ClassHandler::ClassHandler(NativeContext Context) : FunctionHandler(Context)
	{
		duk_push_this(Context);
		this->propcount = -1;
		this->tempid = 0;
	}

	void ClassHandler::SetPropertyString(string Name, string Value)
	{
		duk_push_string(this->Context, Value.c_str());
		duk_put_prop_string(this->Context, -2, Name.c_str());
	}

	void ClassHandler::SetPropertyInt(string Name, s64 Value)
	{
		duk_push_int(this->Context, Value);
		duk_put_prop_string(this->Context, -2, Name.c_str());
	}

	void ClassHandler::SetPropertyUInt(string Name, u64 Value)
	{
		duk_push_uint(this->Context, Value);
		duk_put_prop_string(this->Context, -2, Name.c_str());
	}

	void ClassHandler::SetPropertyBoolean(string Name, bool Value)
	{
		duk_push_boolean(this->Context, Value);
		duk_put_prop_string(this->Context, -2, Name.c_str());
	}

	void ClassHandler::SetPropertyUndefined(string Name)
	{
		duk_push_undefined(this->Context);
		duk_put_prop_string(this->Context, -2, Name.c_str());
	}

	void ClassHandler::SetPropertyNull(string Name)
	{
		duk_push_null(this->Context);
		duk_put_prop_string(this->Context, -2, Name.c_str());
	}

	void ClassHandler::SetPropertyNaN(string Name)
	{
		duk_push_nan(this->Context);
		duk_put_prop_string(this->Context, -2, Name.c_str());
	}

	void ClassHandler::InitializeSettingPropertyArray(string Name, Array &Value)
	{
		Value.Initialize();
		this->tempname = Name;
		this->tempid = Value.GetId();
	}

	void ClassHandler::FinalizeSettingPropertyArray()
	{
		duk_put_prop_string(this->Context, -2, this->tempname.c_str());
	}

	void ClassHandler::InitializetSettingPropertyObject(string Name, Object &Value)
	{
		Value.Initialize();
		this->tempname = Name;
		this->tempid = Value.GetId();
	}

	void ClassHandler::FinalizeSettingPropertyObject()
	{
		duk_put_prop_string(this->Context, this->tempid, this->tempname.c_str());
	}

	string ClassHandler::GetPropertyString(string Name)
	{
		duk_get_prop_string(this->Context, this->propcount, Name.c_str());
		string prop = string(duk_safe_to_string(this->Context, -1));
		this->propcount--;
		return prop;
	}

	s64 ClassHandler::GetPropertyInt(string Name)
	{
		duk_get_prop_string(this->Context, this->propcount, Name.c_str());
		s64 prop = duk_to_int(this->Context, -1);
		this->propcount--;
		return prop;
	}

	u64 ClassHandler::GetPropertyUInt(string Name)
	{
		duk_get_prop_string(this->Context, this->propcount, Name.c_str());
		u64 prop = duk_to_uint(this->Context, -1);
		this->propcount--;
		return prop;
	}

	double ClassHandler::GetPropertyDouble(string Name)
	{
		duk_get_prop_string(this->Context, this->propcount, Name.c_str());
		double prop = duk_to_number(this->Context, -1);
		this->propcount--;
		return prop;
	}

	bool ClassHandler::GetPropertyBoolean(string Name)
	{
		duk_get_prop_string(this->Context, this->propcount, Name.c_str());
		bool prop = duk_to_boolean(this->Context, -1);
		this->propcount--;
		return prop;
	}

	Class::Class(string Name, NativeFunction Constructor)
	{
		this->Name = Name;
		this->Constructor = Constructor;
	}

	void Class::AddString(string Name, string Value)
	{
		Strings.insert(pair<string, string>(Name, Value));
	}

	void Class::AddInt(string Name, s64 Value)
	{
		Ints.insert(pair<string, s64>(Name, Value));
	}

	void Class::AddUInt(string Name, u64 Value)
	{
		UInts.insert(pair<string, u64>(Name, Value));
	}

	void Class::AddDouble(string Name, double Value)
	{
		Doubles.insert(pair<string, double>(Name, Value));
	}

	void Class::AddBoolean(string Name, bool Value)
	{
		Booleans.insert(pair<string, bool>(Name, Value));
	}

	void Class::AddFunction(string Name, NativeFunction Value)
	{
		Functions.insert(pair<string, NativeFunction>(Name, Value));
	}

	void Class::AddUndefined(string Name)
	{
		Undefineds.push_back(Name);
	}

	void Class::AddNull(string Name)
	{
		Nulls.push_back(Name);
	}

	void Class::AddNaN(string Name)
	{
		NaNs.push_back(Name);
	}

	Module::Module(string Module)
	{
		this->Name = Module;
	}

	void Module::PushString(string Name, string Value)
	{
		Strings.insert(pair<string, string>(Name, Value));
	}

	void Module::PushInt(string Name, s64 Value)
	{
		Ints.insert(pair<string, s64>(Name, Value));
	}

	void Module::PushUInt(string Name, u64 Value)
	{
		UInts.insert(pair<string, u64>(Name, Value));
	}

	void Module::PushDouble(string Name, double Value)
	{
		Doubles.insert(pair<string, double>(Name, Value));
	}

	void Module::PushBoolean(string Name, bool Value)
	{
		Booleans.insert(pair<string, bool>(Name, Value));
	}

	void Module::PushFunction(string Name, NativeFunction Value)
	{
		Functions.insert(pair<string, NativeFunction>(Name, Value));
	}

	void Module::PushUndefined(string Name)
	{
		Undefineds.push_back(Name);
	}

	void Module::PushNull(string Name)
	{
		Nulls.push_back(Name);
	}

	void Module::PushNaN(string Name)
	{
		NaNs.push_back(Name);
	}

	void Module::PushClass(Class &Value)
	{
		Classes.push_back(Value);
	}

	GlobalObject::GlobalObject(NativeContext Context)
	{
		this->Context = Context;
	}

	void GlobalObject::PushString(string Name, string Value)
	{
		duk_push_string(this->Context, Value.c_str());
		duk_put_global_string(this->Context, Name.c_str());
	}

	void GlobalObject::PushInt(string Name, s64 Value)
	{
		duk_push_int(this->Context, Value);
		duk_put_global_string(this->Context, Name.c_str());
	}

	void GlobalObject::PushUInt(string Name, u64 Value)
	{
		duk_push_uint(this->Context, Value);
		duk_put_global_string(this->Context, Name.c_str());
	}

	void GlobalObject::PushDouble(string Name, double Value)
	{
		duk_push_number(this->Context, Value);
		duk_put_global_string(this->Context, Name.c_str());
	}

	void GlobalObject::PushBoolean(string Name, bool Value)
	{
		duk_push_boolean(this->Context, Value);
		duk_put_global_string(this->Context, Name.c_str());
	}

	void GlobalObject::PushFunction(string Name, NativeFunction Value)
	{
		duk_push_c_function(this->Context, (duk_c_function)Value, DUK_VARARGS);
		duk_put_global_string(this->Context, Name.c_str());
	}

	void GlobalObject::PushUndefined(string Name)
	{
		duk_push_undefined(this->Context);
		duk_put_global_string(this->Context, Name.c_str());
	}

	void GlobalObject::PushNull(string Name)
	{
		duk_push_null(this->Context);
		duk_put_global_string(this->Context, Name.c_str());
	}

	void GlobalObject::PushNaN(string Name)
	{
		duk_push_nan(this->Context);
		duk_put_global_string(this->Context, Name.c_str());
	}

	void GlobalObject::PushModule(Module &Module)
	{
		duk_idx_t objidx = duk_push_object(Context);
		if(!Module.Strings.empty()) for(auto const& str : Module.Strings)
		{
			duk_push_string(Context, str.second.c_str());
			duk_put_prop_string(Context, objidx, str.first.c_str());
		}
		if(!Module.Ints.empty()) for(auto const& intt : Module.Ints)
		{
			duk_push_int(Context, intt.second);
			duk_put_prop_string(Context, objidx, intt.first.c_str());
		}
		if(!Module.UInts.empty()) for(auto const& uint : Module.UInts)
		{
			duk_push_uint(Context, uint.second);
			duk_put_prop_string(Context, objidx, uint.first.c_str());
		}
		if(!Module.Doubles.empty()) for(auto const& dbl : Module.Doubles)
		{
			duk_push_number(Context, dbl.second);
			duk_put_prop_string(Context, objidx, dbl.first.c_str());
		}
		if(!Module.Booleans.empty()) for(auto const& bol : Module.Booleans)
		{
			duk_push_boolean(Context, bol.second);
			duk_put_prop_string(Context, objidx, bol.first.c_str());
		}
		if(!Module.Functions.empty()) for(auto const& func : Module.Functions)
		{
			duk_push_c_function(Context, func.second, DUK_VARARGS);
			duk_put_prop_string(Context, objidx, func.first.c_str());
		}
		if(!Module.Undefineds.empty()) for(u32 i = 0; i < Module.Undefineds.size(); i++)
		{
			duk_push_undefined(Context);
			duk_put_prop_string(Context, objidx, Module.Undefineds[i].c_str());
		}
		if(!Module.Nulls.empty()) for(u32 i = 0; i < Module.Nulls.size(); i++)
		{
			duk_push_null(Context);
			duk_put_prop_string(Context, objidx, Module.Nulls[i].c_str());
		}
		if(!Module.NaNs.empty()) for(u32 i = 0; i < Module.NaNs.size(); i++)
		{
			duk_push_nan(Context);
			duk_put_prop_string(Context, objidx, Module.NaNs[i].c_str());
		}
		duk_put_global_string(Context, Module.Name.c_str());
	}

	void GlobalObject::InitializePushingObject(string Name, Object &Value)
	{
		Value.Initialize();
		this->tempname = Name;
	}

	void GlobalObject::FinalizePushingObject()
	{
		duk_put_global_string(this->Context, this->tempname.c_str());
	}

	NativeContext CreateContext()
	{
		return duk_create_heap_default();
	}

	void AddModule(Module &Module)
	{
		transform(Module.Name.begin(), Module.Name.end(), Module.Name.begin(), ::tolower);
		if(!Modules.empty()) for(u32 i = 0; i < Modules.size(); i++) if(Modules[i].Name == Module.Name) return;
		Modules.push_back(Module);
	}

	void ThrowError(NativeContext Context, Error ErrorType = Error::CommonError, string Message = "An error was thrown.")
	{
		duk_error(Context, static_cast<duk_errcode_t>(ErrorType), Message.c_str());
	}

	Function require(NativeContext Context)
	{
		Module mmod("");
		bool found = false;
		FunctionHandler handler(Context);
		if(handler.CheckArgcEqualOrBigger(1))
		{
			string xmodname = handler.GetString(0);
			string modname = xmodname;
			if(xmodname.substr(0, 2) == "./") modname = xmodname.substr(2);
			struct stat st;
			if(stat(modname.c_str(), &st) == 0)
			{
				if(st.st_mode & S_IFREG)
				{
					ifstream ifs(modname);
					stringstream strm;
					strm << ifs.rdbuf();
					ifs.close();
					/*
					Object module(Context);
					Global.InitializePushingObject("module", module);
					Object moduleexports(Context);
					module.InitializeAddingObject("exports", moduleexports);
					module.FinalizeAddingObject();
					Global.FinalizePushingObject();
					*/
					string base = strm.str();
					string exports = base + ";exports";
					int err = duk_peval_string(Context, exports.c_str());
					if(err != 0)
					{
						string moduleexports = base + ";module.exports";
						err = duk_peval_string(Context, moduleexports.c_str());
						if(err != 0) API::ThrowError(Context, Error::CommonError, "Failed importing source file: " + string(duk_safe_to_string(Context, -1)));
					}
				}
				else API::ThrowError(Context, Error::CommonError, "Attempted to import a folder (or something which is not a file?)");
			}
			else
			{
				transform(modname.begin(), modname.end(), modname.begin(), ::tolower);
				if(!Modules.empty()) for(u32 i = 0; i < Modules.size(); i++) if(Modules[i].Name == modname)
				{
					mmod = Modules[i];
					found = true;
				}
				if(found)
				{
					duk_idx_t objidx = duk_push_object(Context);
					if(!mmod.Strings.empty()) for(auto const& str : mmod.Strings)
					{
						duk_push_string(Context, str.second.c_str());
						duk_put_prop_string(Context, objidx, str.first.c_str());
					}
					if(!mmod.Ints.empty()) for(auto const& intt : mmod.Ints)
					{
						duk_push_int(Context, intt.second);
						duk_put_prop_string(Context, objidx, intt.first.c_str());
					}
					if(!mmod.UInts.empty()) for(auto const& uint : mmod.UInts)
					{
						duk_push_uint(Context, uint.second);
						duk_put_prop_string(Context, objidx, uint.first.c_str());
					}
					if(!mmod.Doubles.empty()) for(auto const& dbl : mmod.Doubles)
					{
						duk_push_number(Context, dbl.second);
						duk_put_prop_string(Context, objidx, dbl.first.c_str());
					}
					if(!mmod.Booleans.empty()) for(auto const& bol : mmod.Booleans)
					{
						duk_push_boolean(Context, bol.second);
						duk_put_prop_string(Context, objidx, bol.first.c_str());
					}
					if(!mmod.Functions.empty()) for(auto const& func : mmod.Functions)
					{
						duk_push_c_function(Context, func.second, DUK_VARARGS);
						duk_put_prop_string(Context, objidx, func.first.c_str());
					}
					if(!mmod.Undefineds.empty()) for(u32 i = 0; i < mmod.Undefineds.size(); i++)
					{
						duk_push_undefined(Context);
						duk_put_prop_string(Context, objidx, mmod.Undefineds[i].c_str());
					}
					if(!mmod.Nulls.empty()) for(u32 i = 0; i < mmod.Nulls.size(); i++)
					{
						duk_push_null(Context);
						duk_put_prop_string(Context, objidx, mmod.Nulls[i].c_str());
					}
					if(!mmod.NaNs.empty()) for(u32 i = 0; i < mmod.NaNs.size(); i++)
					{
						duk_push_nan(Context);
						duk_put_prop_string(Context, objidx, mmod.NaNs[i].c_str());
					}
					if(!mmod.Classes.empty()) for(u32 i = 0; i < mmod.Classes.size(); i++)
					{
						Class cls = mmod.Classes[i];
						duk_push_c_function(Context, cls.Constructor, DUK_VARARGS);
						duk_push_object(Context);
						if(!cls.Strings.empty()) for(auto const& str : cls.Strings)
						{
							duk_push_string(Context, str.second.c_str());
							duk_put_prop_string(Context, -2, str.first.c_str());
						}
						if(!cls.Ints.empty()) for(auto const& intt : cls.Ints)
						{
							duk_push_int(Context, intt.second);
							duk_put_prop_string(Context, -2, intt.first.c_str());
						}
						if(!cls.UInts.empty()) for(auto const& uint : cls.UInts)
						{
							duk_push_uint(Context, uint.second);
							duk_put_prop_string(Context, -2, uint.first.c_str());
						}
						if(!cls.Doubles.empty()) for(auto const& dbl : cls.Doubles)
						{
							duk_push_number(Context, dbl.second);
							duk_put_prop_string(Context, -2, dbl.first.c_str());
						}
						if(!cls.Booleans.empty()) for(auto const& bol : cls.Booleans)
						{
							duk_push_boolean(Context, bol.second);
							duk_put_prop_string(Context, -2, bol.first.c_str());
						}
						if(!cls.Functions.empty()) for(auto const& func : cls.Functions)
						{
							duk_push_c_function(Context, func.second, DUK_VARARGS);
							duk_put_prop_string(Context, -2, func.first.c_str());
						}
						if(!cls.Undefineds.empty()) for(u32 i = 0; i < cls.Undefineds.size(); i++)
						{
							duk_push_undefined(Context);
							duk_put_prop_string(Context, -2, cls.Undefineds[i].c_str());
						}
						if(!cls.Nulls.empty()) for(u32 i = 0; i < cls.Nulls.size(); i++)
						{
							duk_push_null(Context);
							duk_put_prop_string(Context, -2, cls.Nulls[i].c_str());
						}
						if(!cls.NaNs.empty()) for(u32 i = 0; i < cls.NaNs.size(); i++)
						{
							duk_push_nan(Context);
							duk_put_prop_string(Context, -2, cls.NaNs[i].c_str());
						}
						duk_put_prop_string(Context, -2, "prototype");
						duk_put_prop_string(Context, objidx, cls.Name.c_str());
					}
				}
				else API::ThrowError(Context, Error::CommonError, "Cannot find module \'" + modname + "\'");
			}
		}
		return Return::Variable;
	}

	Function evalFile(NativeContext Context)
	{
		FunctionHandler handler(Context);
		if(handler.CheckArgcEqualOrBigger(1))
		{
			string file = handler.GetString(0);
			ifstream ifs(file);
			if(ifs.good())
			{
				stringstream strm;
				strm << ifs.rdbuf();
				duk_get_global_string(Context, "eval");
				duk_push_string(Context, strm.str().c_str());
				duk_call(Context, 1);
			}
		}
		return Return::Void;
	}

	Function randRange(NativeContext Context)
	{
		FunctionHandler handler(Context);
		if(handler.CheckArgcEqualOrBigger(2))
		{
			int min = handler.GetInt(0);
			int max = handler.GetInt(1);
			int rnd = (min + rand() % (max - min));
			handler.ReturnInt(rnd);
		}
		return Return::Variable;
	}

	void InitializeGlobalObject(NativeContext Context)
	{
		Global = GlobalObject(Context);
		Object exports(Context);
		Global.InitializePushingObject("exports", exports);
		Global.FinalizePushingObject();
		Object module(Context);
		Global.InitializePushingObject("module", module);
		Object moduleexports(Context);
		module.InitializeAddingObject("exports", moduleexports);
		module.FinalizeAddingObject();
		Global.FinalizePushingObject();
		Global.PushFunction("require", require);
		Global.PushFunction("evalFile", evalFile);
		Global.PushFunction("randRange", randRange);
		Module console = Brew::BuiltIn::Console::Initialize();
		Global.PushModule(console);
		Module process = Brew::BuiltIn::Process::Initialize();
		Global.PushModule(process);
	}
}
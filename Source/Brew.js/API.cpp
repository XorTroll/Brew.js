#include "API.hpp"

Brew::API::Array::Array(Brew::API::NativeContext Context)
{
	this->Context = Context;
	this->start = false;
	this->arridx = 0;
	this->idx = 0;
	this->padd = true;
	this->uidx = 0;
}

Brew::API::Array::Array(Brew::API::NativeContext Context, u32 Index)
{
	this->Context = Context;
	this->start = false;
	this->arridx = 0;
	this->idx = 0;
	this->padd = false;
	this->uidx = Index;
}

void Brew::API::Array::init()
{
	if(!this->padd) return;
	if(!this->start)
	{
		arridx = duk_push_array(this->Context);
		idx = 0;
		start = true;
	}
}

void Brew::API::Array::addString(string Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_string(this->Context, Value.c_str());
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addInt(s64 Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_int(this->Context, Value);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addUInt(u64 Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_uint(this->Context, Value);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addDouble(double Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_number(this->Context, Value);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addBoolean(bool Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_boolean(this->Context, Value);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addFunction(NativeFunction Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_c_function(this->Context, Value, DUK_VARARGS);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addUndefined()
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_undefined(this->Context);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addNull()
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_null(this->Context);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addNaN()
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_nan(this->Context);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::end()
{
	if(!this->padd) return;
	duk_pop(this->Context);
}

string Brew::API::Array::getString(u32 Index)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Array not accessed as a function argument");
	duk_get_prop_index(this->Context, this->uidx, Index);
	return string(duk_to_string(this->Context, -1));
}

s64 Brew::API::Array::getInt(u32 Index)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Array not accessed as a function argument");
	duk_get_prop_index(this->Context, this->uidx, Index);
	return duk_to_int(this->Context, -1);
}

u64 Brew::API::Array::getUInt(u32 Index)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Array not accessed as a function argument");
	duk_get_prop_index(this->Context, this->uidx, Index);
	return duk_to_uint(this->Context, -1);
}

double Brew::API::Array::getDouble(u32 Index)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Array not accessed as a function argument");
	duk_get_prop_index(this->Context, this->uidx, Index);
	return duk_to_number(this->Context, -1);
}

bool Brew::API::Array::getBoolean(u32 Index)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Array not accessed as a function argument");
	duk_get_prop_index(this->Context, this->uidx, Index);
	return duk_to_boolean(this->Context, -1);
}

Brew::API::Array Brew::API::Array::getArray(u32 Index)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Array not accessed as a function argument");
	duk_get_prop_index(this->Context, this->uidx, Index);
	return Brew::API::Array(this->Context, -1);
}

Brew::API::Object Brew::API::Array::getObject(u32 Index)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Array not accessed as a function argument");
	duk_get_prop_index(this->Context, this->uidx, Index);
	return Brew::API::Object(this->Context, -1);
}

string Brew::API::Array::asJSON()
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Array not accessed as a function argument");
	duk_json_encode(this->Context, this->uidx);
	return string(duk_to_string(this->Context, -1));
}

int Brew::API::Array::getID()
{
	if(!this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Array accessed as a function argument");
	return this->arridx;
}

Brew::API::Object::Object(Brew::API::NativeContext Context)
{
	this->Context = Context;
	this->start = false;
	this->padd = true;
	this->idx = 0;
	this->objidx = 0;
}

Brew::API::Object::Object(Brew::API::NativeContext Context, u32 Index)
{
	this->Context = Context;
	this->start = false;
	this->padd = false;
	this->idx = Index;
	this->objidx = 0;
}

void Brew::API::Object::init()
{
	if(!this->padd) return;
	if(!this->start)
	{
		this->objidx = duk_push_object(this->Context);
		start = true;
	}
}

void Brew::API::Object::addString(string Name, string Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_string(this->Context, Value.c_str());
	duk_put_prop_string(this->Context, this->objidx, Name.c_str());
}

void Brew::API::Object::addInt(string Name, s64 Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_int(this->Context, Value);
	duk_put_prop_string(this->Context, this->objidx, Name.c_str());
}

void Brew::API::Object::addUInt(string Name, u64 Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_uint(this->Context, Value);
	duk_put_prop_string(this->Context, this->objidx, Name.c_str());
}

void Brew::API::Object::addDouble(string Name, double Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_number(this->Context, Value);
	duk_put_prop_string(this->Context, this->objidx, Name.c_str());
}

void Brew::API::Object::addBoolean(string Name, bool Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_boolean(this->Context, Value);
	duk_put_prop_string(this->Context, this->objidx, Name.c_str());
}

void Brew::API::Object::addFunction(string Name, NativeFunction Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_c_function(this->Context, Value, DUK_VARARGS);
	duk_put_prop_string(this->Context, this->objidx, Name.c_str());
}

void Brew::API::Object::addUndefined(string Name)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_undefined(this->Context);
	duk_put_prop_string(this->Context, this->objidx, Name.c_str());
}

void Brew::API::Object::addNull(string Name)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_null(this->Context);
	duk_put_prop_string(this->Context, this->objidx, Name.c_str());
}

void Brew::API::Object::addNaN(string Name)
{
	if(!this->padd) return;
	if(!this->start) init();
	duk_push_nan(this->Context);
	duk_put_prop_string(this->Context, this->objidx, Name.c_str());
}

void Brew::API::Object::startAddArray(string Name, Brew::API::Array Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	Value.init();
	this->tempname = Name;
}

void Brew::API::Object::endAddArray()
{
	duk_put_prop_string(this->Context, this->objidx, this->tempname.c_str());
}

void Brew::API::Object::startAddObject(string Name, Brew::API::Object Value)
{
	if(!this->padd) return;
	if(!this->start) init();
	Value.init();
	this->tempname = Name;
}

void Brew::API::Object::endAddObject()
{
	duk_put_prop_string(this->Context, this->objidx, this->tempname.c_str());
}

void Brew::API::Object::end()
{
	if(!this->padd) return;
	duk_pop(this->Context);
}

string Brew::API::Object::getString(string Name)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Object not accessed as a function argument");
	duk_get_prop_string(this->Context, this->idx, Name.c_str());
	return string(duk_to_string(this->Context, -1));
}

s64 Brew::API::Object::getInt(string Name)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Object not accessed as a function argument");
	duk_get_prop_string(this->Context, this->idx, Name.c_str());
	return duk_to_int(this->Context, -1);
}

u64 Brew::API::Object::getUInt(string Name)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Object not accessed as a function argument");
	duk_get_prop_string(this->Context, this->idx, Name.c_str());
	return duk_to_uint(this->Context, -1);
}

double Brew::API::Object::getDouble(string Name)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Object not accessed as a function argument");
	duk_get_prop_string(this->Context, this->idx, Name.c_str());
	return duk_to_number(this->Context, -1);
}

bool Brew::API::Object::getBoolean(string Name)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Object not accessed as a function argument");
	duk_get_prop_string(this->Context, this->idx, Name.c_str());
	return duk_to_boolean(this->Context, -1);
}

Brew::API::Array Brew::API::Object::getArray(string Name)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Object not accessed as a function argument");
	duk_get_prop_string(this->Context, this->idx, Name.c_str());
	return Brew::API::Array(this->Context, -1);
}

Brew::API::Object Brew::API::Object::getObject(string Name)
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Object not accessed as a function argument");
	duk_get_prop_string(this->Context, this->idx, Name.c_str());
	return Brew::API::Object(this->Context, -1);
}

string Brew::API::Object::asJSON()
{
	if(this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Object not accessed as a function argument");
	duk_json_encode(this->Context, this->idx);
	return string(duk_to_string(this->Context, -1));
}

int Brew::API::Object::getID()
{
	if(!this->padd) throwError(this->Context, Brew::API::Error::CommonError, "Object accessed as a function argument");
	return this->objidx;
}

Brew::API::Callback::Callback(Brew::API::NativeContext Context, u32 Index)
{
	this->Context = Context;
	this->iter = 0;
}

void Brew::API::Callback::addArgumentString(string Value)
{
	this->reg.push_back(0);
	this->Strings.insert(pair<u32, string>(this->iter, Value));
	this->iter++;
}

void Brew::API::Callback::addArgumentInt(s64 Value)
{
	this->reg.push_back(1);
	this->Ints.insert(pair<u32, s64>(this->iter, Value));
	this->iter++;
}

void Brew::API::Callback::addArgumentUInt(u64 Value)
{
	this->reg.push_back(2);
	this->UInts.insert(pair<u32, u64>(this->iter, Value));
	this->iter++;
}

void Brew::API::Callback::addArgumentDouble(double Value)
{
	this->reg.push_back(3);
	this->Doubles.insert(pair<u32, double>(this->iter, Value));
	this->iter++;
}

void Brew::API::Callback::addArgumentBoolean(bool Value)
{
	this->reg.push_back(4);
	this->Booleans.insert(pair<u32, bool>(this->iter, Value));
	this->iter++;
}

void Brew::API::Callback::addArgumentUndefined()
{
	this->reg.push_back(5);
	this->Undefineds.push_back(this->iter);
	this->iter++;
}

void Brew::API::Callback::addArgumentNull()
{
	this->reg.push_back(6);
	this->Nulls.push_back(this->iter);
	this->iter++;
}

void Brew::API::Callback::addArgumentNaN()
{
	this->reg.push_back(7);
	this->NaNs.push_back(this->iter);
	this->iter++;
}

void Brew::API::Callback::callFunction()
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

void Brew::API::Callback::callFunctionNew()
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

Brew::API::FunctionHandler::FunctionHandler(Brew::API::NativeContext Context)
{
	this->Context = Context;
	this->count = -1;
}

u32 Brew::API::FunctionHandler::getArgc()
{
	u32 argc = duk_get_top(this->Context);
	return argc;
}

bool Brew::API::FunctionHandler::checkArgc(u32 Argc)
{
	bool ok = (duk_get_top(this->Context) >= Argc);
	return ok;
}

bool Brew::API::FunctionHandler::checkArgcEqual(u32 Argc)
{
	bool ok = (duk_get_top(this->Context) == Argc);
	return ok;
}

Brew::API::Type Brew::API::FunctionHandler::getArgType(u32 Index)
{
	Type typ = (Type)duk_get_type(this->Context, Index);
	return typ;
}

bool Brew::API::FunctionHandler::checkArgType(u32 Index, Type ArgType)
{
	Type typ = getArgType(Index);
	bool check = (typ == ArgType);
	return check;
}

bool Brew::API::FunctionHandler::isConstructorCall()
{
	bool ctor = duk_is_constructor_call(this->Context);
	return ctor;
}

string Brew::API::FunctionHandler::getString(u32 Index)
{
	string str = "";
	if(duk_get_type(this->Context, Index) == DUK_TYPE_STRING) str = string(duk_safe_to_string(this->Context, Index));
	count--;
	return str;
}

s64 Brew::API::FunctionHandler::getInt(u32 Index)
{
	s64 intt = 0;
	if(duk_get_type(this->Context, Index) == DUK_TYPE_NUMBER) intt = duk_get_int(this->Context, Index);
	count--;
	return intt;
}

u64 Brew::API::FunctionHandler::getUInt(u32 Index)
{
	u64 intt = 0;
	if(duk_get_type(this->Context, Index) == DUK_TYPE_NUMBER) intt = duk_get_uint(this->Context, Index);
	count--;
	return intt;
}

double Brew::API::FunctionHandler::getDouble(u32 Index)
{
	double intt = 0.0;
	if(duk_get_type(this->Context, Index) == DUK_TYPE_NUMBER) intt = duk_get_number(this->Context, Index);
	count--;
	return intt;
}

bool Brew::API::FunctionHandler::getBoolean(u32 Index)
{
	bool booln = false;
	if(duk_get_type(this->Context, Index) == DUK_TYPE_BOOLEAN) booln = duk_get_boolean(this->Context, Index);
	count--;
	return booln;
}

Brew::API::Callback Brew::API::FunctionHandler::getCallback(u32 Index)
{
	if(!duk_is_function(this->Context, Index)) throwError(this->Context, Brew::API::Error::TypeError, "Argument at index " + to_string(Index) + " is not a callable function");
	return Brew::API::Callback(this->Context, Index);
}

Brew::API::Object Brew::API::FunctionHandler::getObject(u32 Index)
{
	if(!duk_is_object(this->Context, Index)) throwError(this->Context, Brew::API::Error::TypeError, "Argument at index " + to_string(Index) + " is not an object");
	return Brew::API::Object(this->Context, Index);
}

Brew::API::Array Brew::API::FunctionHandler::getArray(u32 Index)
{
	if(!duk_is_array(this->Context, Index)) throwError(this->Context, Brew::API::Error::TypeError, "Argument at index " + to_string(Index) + " is not an array");
	return Brew::API::Array(this->Context, Index);
}

void Brew::API::FunctionHandler::pushString(string Value)
{
	duk_push_string(this->Context, Value.c_str());
}

void Brew::API::FunctionHandler::pushInt(s64 Value)
{
	duk_push_int(this->Context, Value);
}

void Brew::API::FunctionHandler::pushUInt(u64 Value)
{
	duk_push_uint(this->Context, Value);
}

void Brew::API::FunctionHandler::pushDouble(double Value)
{
	duk_push_number(this->Context, Value);
}

void Brew::API::FunctionHandler::pushBoolean(bool Value)
{
	duk_push_boolean(this->Context, Value);
}

void Brew::API::FunctionHandler::pushArray(Brew::API::Array Value)
{
	Value.init();
}

void Brew::API::FunctionHandler::pushObject(Brew::API::Object Value)
{
	Value.init();
}

void Brew::API::FunctionHandler::pushUndefined()
{
	duk_push_undefined(this->Context);
}

void Brew::API::FunctionHandler::pushNull()
{
	duk_push_null(this->Context);
}

void Brew::API::FunctionHandler::pushNaN()
{
	duk_push_nan(this->Context);
}

Brew::API::ClassHandler::ClassHandler(Brew::API::NativeContext Context) : Brew::API::FunctionHandler(Context)
{
	duk_push_this(Context);
	this->propcount = -1;
	this->tempid = 0;
}

void Brew::API::ClassHandler::setPropertyString(string Name, string Value)
{
	duk_push_string(this->Context, Value.c_str());
	duk_put_prop_string(this->Context, -2, Name.c_str());
}

void Brew::API::ClassHandler::setPropertyInt(string Name, s64 Value)
{
	duk_push_int(this->Context, Value);
	duk_put_prop_string(this->Context, -2, Name.c_str());
}

void Brew::API::ClassHandler::setPropertyUInt(string Name, u64 Value)
{
	duk_push_uint(this->Context, Value);
	duk_put_prop_string(this->Context, -2, Name.c_str());
}

void Brew::API::ClassHandler::setPropertyBoolean(string Name, bool Value)
{
	duk_push_boolean(this->Context, Value);
	duk_put_prop_string(this->Context, -2, Name.c_str());
}

void Brew::API::ClassHandler::setPropertyUndefined(string Name)
{
	duk_push_undefined(this->Context);
	duk_put_prop_string(this->Context, -2, Name.c_str());
}

void Brew::API::ClassHandler::setPropertyNull(string Name)
{
	duk_push_null(this->Context);
	duk_put_prop_string(this->Context, -2, Name.c_str());
}

void Brew::API::ClassHandler::setPropertyNaN(string Name)
{
	duk_push_nan(this->Context);
	duk_put_prop_string(this->Context, -2, Name.c_str());
}

void Brew::API::ClassHandler::startSetPropertyArray(string Name, Brew::API::Array Value)
{
	Value.init();
	this->tempname = Name;
	this->tempid = Value.getID();
}

void Brew::API::ClassHandler::endSetPropertyArray()
{
	duk_put_prop_string(this->Context, -2, this->tempname.c_str());
}

void Brew::API::ClassHandler::startSetPropertyObject(string Name, Brew::API::Object Value)
{
	Value.init();
	this->tempname = Name;
	this->tempid = Value.getID();
}

void Brew::API::ClassHandler::endSetPropertyObject()
{
	duk_put_prop_string(this->Context, this->tempid, this->tempname.c_str());
}

string Brew::API::ClassHandler::getPropertyString(string Name)
{
	duk_get_prop_string(this->Context, this->propcount, Name.c_str());
	string prop = string(duk_safe_to_string(this->Context, -1));
	this->propcount--;
	return prop;
}

s64 Brew::API::ClassHandler::getPropertyInt(string Name)
{
	duk_get_prop_string(this->Context, this->propcount, Name.c_str());
	s64 prop = duk_to_int(this->Context, -1);
	this->propcount--;
	return prop;
}

u64 Brew::API::ClassHandler::getPropertyUInt(string Name)
{
	duk_get_prop_string(this->Context, this->propcount, Name.c_str());
	u64 prop = duk_to_uint(this->Context, -1);
	this->propcount--;
	return prop;
}

double Brew::API::ClassHandler::getPropertyDouble(string Name)
{
	duk_get_prop_string(this->Context, this->propcount, Name.c_str());
	double prop = duk_to_number(this->Context, -1);
	this->propcount--;
	return prop;
}

bool Brew::API::ClassHandler::getPropertyBoolean(string Name)
{
	duk_get_prop_string(this->Context, this->propcount, Name.c_str());
	bool prop = duk_to_boolean(this->Context, -1);
	this->propcount--;
	return prop;
}

Brew::API::Class::Class(string Name, Brew::API::NativeFunction Constructor)
{
	this->Name = Name;
	this->Constructor = Constructor;
}

void Brew::API::Class::addString(string Name, string Value)
{
	Strings.insert(pair<string, string>(Name, Value));
}

void Brew::API::Class::addInt(string Name, s64 Value)
{
	Ints.insert(pair<string, s64>(Name, Value));
}

void Brew::API::Class::addUInt(string Name, u64 Value)
{
	UInts.insert(pair<string, u64>(Name, Value));
}

void Brew::API::Class::addDouble(string Name, double Value)
{
	Doubles.insert(pair<string, double>(Name, Value));
}

void Brew::API::Class::addBoolean(string Name, bool Value)
{
	Booleans.insert(pair<string, bool>(Name, Value));
}

void Brew::API::Class::addFunction(string Name, NativeFunction Value)
{
	Functions.insert(pair<string, NativeFunction>(Name, Value));
}

void Brew::API::Class::addUndefined(string Name)
{
	Undefineds.push_back(Name);
}

void Brew::API::Class::addNull(string Name)
{
	Nulls.push_back(Name);
}

void Brew::API::Class::addNaN(string Name)
{
	NaNs.push_back(Name);
}

Brew::API::Module::Module(string Module)
{
	this->Name = Module;
}

void Brew::API::Module::pushString(string Name, string Value)
{
	Strings.insert(pair<string, string>(Name, Value));
}

void Brew::API::Module::pushInt(string Name, s64 Value)
{
	Ints.insert(pair<string, s64>(Name, Value));
}

void Brew::API::Module::pushUInt(string Name, u64 Value)
{
	UInts.insert(pair<string, u64>(Name, Value));
}

void Brew::API::Module::pushDouble(string Name, double Value)
{
	Doubles.insert(pair<string, double>(Name, Value));
}

void Brew::API::Module::pushBoolean(string Name, bool Value)
{
	Booleans.insert(pair<string, bool>(Name, Value));
}

void Brew::API::Module::pushFunction(string Name, NativeFunction Value)
{
	Functions.insert(pair<string, NativeFunction>(Name, Value));
}

void Brew::API::Module::pushUndefined(string Name)
{
	Undefineds.push_back(Name);
}

void Brew::API::Module::pushNull(string Name)
{
	Nulls.push_back(Name);
}

void Brew::API::Module::pushNaN(string Name)
{
	NaNs.push_back(Name);
}

void Brew::API::Module::pushClass(Brew::API::Class Value)
{
	Classes.push_back(Value);
}

Brew::API::GlobalObject::GlobalObject(Brew::API::NativeContext Context)
{
	this->Context = Context;
}

void Brew::API::GlobalObject::pushString(string Name, string Value)
{
	duk_push_string(this->Context, Value.c_str());
	duk_put_global_string(this->Context, Name.c_str());
}

void Brew::API::GlobalObject::pushInt(string Name, s64 Value)
{
	duk_push_int(this->Context, Value);
	duk_put_global_string(this->Context, Name.c_str());
}

void Brew::API::GlobalObject::pushUInt(string Name, u64 Value)
{
	duk_push_uint(this->Context, Value);
	duk_put_global_string(this->Context, Name.c_str());
}

void Brew::API::GlobalObject::pushDouble(string Name, double Value)
{
	duk_push_number(this->Context, Value);
	duk_put_global_string(this->Context, Name.c_str());
}

void Brew::API::GlobalObject::pushBoolean(string Name, bool Value)
{
	duk_push_boolean(this->Context, Value);
	duk_put_global_string(this->Context, Name.c_str());
}

void Brew::API::GlobalObject::pushFunction(string Name, NativeFunction Value)
{
	duk_push_c_function(this->Context, (duk_c_function)Value, DUK_VARARGS);
	duk_put_global_string(this->Context, Name.c_str());
}

void Brew::API::GlobalObject::pushUndefined(string Name)
{
	duk_push_undefined(this->Context);
	duk_put_global_string(this->Context, Name.c_str());
}

void Brew::API::GlobalObject::pushNull(string Name)
{
	duk_push_null(this->Context);
	duk_put_global_string(this->Context, Name.c_str());
}

void Brew::API::GlobalObject::pushNaN(string Name)
{
	duk_push_nan(this->Context);
	duk_put_global_string(this->Context, Name.c_str());
}

void Brew::API::GlobalObject::pushModule(Brew::API::Module Module)
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

void Brew::API::GlobalObject::startPushObject(string Name, Brew::API::Object Value)
{
	Value.init();
	this->tempname = Name;
}

void Brew::API::GlobalObject::endPushObject()
{
	duk_put_global_string(this->Context, this->tempname.c_str());
}

Brew::API::NativeContext Brew::API::createContext()
{
	return duk_create_heap_default();
}

void Brew::API::addModule(Brew::API::Module Module)
{
	transform(Module.Name.begin(), Module.Name.end(), Module.Name.begin(), ::tolower);
	if(!Brew::API::Modules.empty()) for(int i = 0; i < Brew::API::Modules.size(); i++) if(Brew::API::Modules[i].Name == Module.Name) return;
	Brew::API::Modules.push_back(Module);
}

void Brew::API::throwError(Brew::API::NativeContext Context, Brew::API::Error ErrorType = Brew::API::Error::CommonError, string Message = "An error was thrown.")
{
	duk_error(Context, (duk_errcode_t)ErrorType, Message.c_str());
}

Brew::API::Function Brew::API::require(Brew::API::NativeContext Context)
{
	Brew::API::Module mmod("");
	bool found = false;
	Brew::API::FunctionHandler handler(Context);
	if(handler.checkArgc(1))
	{
		string modname = handler.getString(0);
		struct stat st;
		if(stat(modname.c_str(), &st) == 0)
		{
			if(st.st_mode & S_IFREG)
			{
				ifstream ifs(modname);
				stringstream strm;
				strm << ifs.rdbuf();
				ifs.close();
				Brew::API::Object module(Context);
				Brew::API::Global.startPushObject("module", module);
				Brew::API::Object moduleexports(Context);
				module.startAddObject("exports", moduleexports);
				module.endAddObject();
				Brew::API::Global.endPushObject();
				string base = strm.str();
				string exports = base + ";exports";
				int err = duk_peval_string(Context, exports.c_str());
				if(err != 0)
				{
					string moduleexports = base + ";module.exports";
					err = duk_peval_string(Context, moduleexports.c_str());
					if(err != 0) throwError(Context, Brew::API::Error::CommonError, "Failed importing source file: " + string(duk_safe_to_string(Context, -1)));
				}
			}
			else throwError(Context, Brew::API::Error::CommonError, "Attempted to import a folder (or something which is not a file?)");
		}
		else
		{
			transform(modname.begin(), modname.end(), modname.begin(), ::tolower);
			if(!Brew::API::Modules.empty()) for(int i = 0; i < Brew::API::Modules.size(); i++) if(Brew::API::Modules[i].Name == modname)
			{
				mmod = Brew::API::Modules[i];
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
					Brew::API::Class cls = mmod.Classes[i];
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
			else throwError(Context, Brew::API::Error::CommonError, "Cannot find module \'" + modname + "\'");
		}
	}
	return Brew::API::Return::Variable;
}

Brew::API::Function Brew::API::evalFile(Brew::API::NativeContext Context)
{
	Brew::API::FunctionHandler handler(Context);
	if(handler.checkArgc(1))
	{
		string file = handler.getString(0);
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
	return Brew::API::Return::Void;
}

Brew::API::Function Brew::API::randRange(Brew::API::NativeContext Context)
{
	Brew::API::FunctionHandler handler(Context);
	if(handler.checkArgc(2))
	{
		int min = handler.getInt(0);
		int max = handler.getInt(1);
		int rnd = (min + rand() % (max - min));
		handler.pushInt(rnd);
	}
	return Brew::API::Return::Variable;
}

void Brew::API::initGlobal(Brew::API::NativeContext Context)
{
	Brew::API::Global = Brew::API::GlobalObject(Context);
	Brew::API::Object exports(Context);
	Brew::API::Global.startPushObject("exports", exports);
	Brew::API::Global.endPushObject();
	Brew::API::Object module(Context);
	Brew::API::Global.startPushObject("module", module);
	Brew::API::Object moduleexports(Context);
	module.startAddObject("exports", moduleexports);
	module.endAddObject();
	Brew::API::Global.endPushObject();
	Brew::API::Global.pushFunction("require", require);
	Brew::API::Global.pushFunction("evalFile", evalFile);
	Brew::API::Global.pushFunction("randRange", randRange);
	Brew::API::Global.pushModule(Brew::BuiltIn::Console::initModule());
    Brew::API::Global.pushModule(Brew::BuiltIn::Process::initModule());
}
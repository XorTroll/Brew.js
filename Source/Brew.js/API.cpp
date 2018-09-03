#include "API.hpp"

Brew::API::Array::Array(Brew::API::NativeContext Context)
{
	this->Context = Context;
	this->start = false;
}

void Brew::API::Array::init()
{
	if(!this->start)
	{
		arridx = duk_push_array(this->Context);
		idx = 0;
		start = true;
	}
}

void Brew::API::Array::addString(string Value)
{
	if(!this->start) init();
	duk_push_string(this->Context, Value.c_str());
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addInt(s64 Value)
{
	if(!this->start) init();
	duk_push_int(this->Context, Value);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addUInt(u64 Value)
{
	if(!this->start) init();
	duk_push_uint(this->Context, Value);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addDouble(double Value)
{
	if(!this->start) init();
	duk_push_number(this->Context, Value);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addBoolean(bool Value)
{
	if(!this->start) init();
	duk_push_boolean(this->Context, Value);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addFunction(NativeFunction Value)
{
	if(!this->start) init();
	duk_push_c_function(this->Context, Value, DUK_VARARGS);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addUndefined()
{
	if(!this->start) init();
	duk_push_undefined(this->Context);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addNull()
{
	if(!this->start) init();
	duk_push_null(this->Context);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::addNaN()
{
	if(!this->start) init();
	duk_push_nan(this->Context);
	duk_put_prop_index(this->Context, arridx, idx);
	idx++;
}

void Brew::API::Array::end()
{
	duk_pop(this->Context);
}

Brew::API::Object::Object(Brew::API::NativeContext Context)
{
	this->Context = Context;
	this->start = false;
}

void Brew::API::Object::init()
{
	if(!this->start)
	{
		objidx = duk_push_object(this->Context);
		start = true;
	}
}

void Brew::API::Object::addString(string Name, string Value)
{
	if(!this->start) init();
	duk_push_string(this->Context, Value.c_str());
	duk_put_prop_string(this->Context, objidx, Name.c_str());
}

void Brew::API::Object::addInt(string Name, s64 Value)
{
	if(!this->start) init();
	duk_push_int(this->Context, Value);
	duk_put_prop_string(this->Context, objidx, Name.c_str());
}

void Brew::API::Object::addUInt(string Name, u64 Value)
{
	if(!this->start) init();
	duk_push_uint(this->Context, Value);
	duk_put_prop_string(this->Context, objidx, Name.c_str());
}

void Brew::API::Object::addDouble(string Name, double Value)
{
	if(!this->start) init();
	duk_push_number(this->Context, Value);
	duk_put_prop_string(this->Context, objidx, Name.c_str());
}

void Brew::API::Object::addBoolean(string Name, bool Value)
{
	if(!this->start) init();
	duk_push_boolean(this->Context, Value);
	duk_put_prop_string(this->Context, objidx, Name.c_str());
}

void Brew::API::Object::addFunction(string Name, NativeFunction Value)
{
	if(!this->start) init();
	duk_push_c_function(this->Context, Value, DUK_VARARGS);
	duk_put_prop_string(this->Context, objidx, Name.c_str());
}

void Brew::API::Object::addUndefined(string Name)
{
	if(!this->start) init();
	duk_push_undefined(this->Context);
	duk_put_prop_string(this->Context, objidx, Name.c_str());
}

void Brew::API::Object::addNull(string Name)
{
	if(!this->start) init();
	duk_push_null(this->Context);
	duk_put_prop_string(this->Context, objidx, Name.c_str());
}

void Brew::API::Object::addNaN(string Name)
{
	if(!this->start) init();
	duk_push_nan(this->Context);
	duk_put_prop_string(this->Context, objidx, Name.c_str());
}

void Brew::API::Object::end()
{
	duk_pop(this->Context);
}

Brew::API::FunctionHandler::FunctionHandler(Brew::API::NativeContext Context)
{
	this->Context = Context;
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

string Brew::API::FunctionHandler::getString(u32 Index)
{
	string str = "";
	if(duk_get_type(this->Context, Index) == DUK_TYPE_STRING) str = string(duk_get_string_default(this->Context, Index, ""));
	return str;
}

s64 Brew::API::FunctionHandler::getInt(u32 Index)
{
	s64 intt = 0;
	if(duk_get_type(this->Context, Index) == DUK_TYPE_NUMBER) intt = duk_get_int(this->Context, Index);
	return intt;
}

u64 Brew::API::FunctionHandler::getUInt(u32 Index)
{
	u64 intt = 0;
	if(duk_get_type(this->Context, Index) == DUK_TYPE_NUMBER) intt = duk_get_uint(this->Context, Index);
	return intt;
}

double Brew::API::FunctionHandler::getDouble(u32 Index)
{
	double intt = 0.0;
	if(duk_get_type(this->Context, Index) == DUK_TYPE_NUMBER) intt = duk_get_number(this->Context, Index);
	return intt;
}

bool Brew::API::FunctionHandler::getBoolean(u32 Index)
{
	bool booln = false;
	if(duk_get_type(this->Context, Index) == DUK_TYPE_BOOLEAN) booln = duk_get_boolean(this->Context, Index);
	return booln;
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

void Brew::API::FunctionHandler::throwError(Brew::API::Error ErrorType = Brew::API::Error::CommonError, string Message = "An error was thrown.")
{
	duk_error(this->Context, (duk_errcode_t)ErrorType, Message.c_str());
}

Brew::API::Module::Module(string Module)
{
	Name = Module;
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

Brew::API::Function Brew::API::require(Brew::API::NativeContext Context)
{
	Brew::API::Module Module("");
	bool found = false;
	Brew::API::FunctionHandler handler(Context);
	if(handler.checkArgc(1))
	{
		string modname = handler.getString(0);
		transform(modname.begin(), modname.end(), modname.begin(), ::tolower);
		if(!Brew::API::Modules.empty()) for(int i = 0; i < Brew::API::Modules.size(); i++) if(Brew::API::Modules[i].Name == modname)
		{
			Module = Brew::API::Modules[i];
			found = true;
		}
		if(found)
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
		}
		else handler.throwError(Brew::API::Error::CommonError, "Cannot find module \'" + modname + "\'");
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
			duk_safe_to_string(Context, -1);
			string res = string(duk_get_string(Context, -1));
			handler.pushString(res);
		}
	}
	return Brew::API::Return::Variable;
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

void Brew::API::initializeGlobalObject(Brew::API::NativeContext Context)
{
	Brew::API::Global = Brew::API::GlobalObject(Context);
	Brew::API::Global.pushFunction("require", require);
	Brew::API::Global.pushFunction("evalFile", evalFile);
	Brew::API::Global.pushFunction("randRange", randRange);
	Brew::API::Global.pushModule(Brew::BuiltIn::Console::initModule());
}
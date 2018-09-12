#include "Global.hpp"

Brew::API::Function Brew::BuiltIn::require(Brew::API::NativeContext Context)
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
			if(!Module.Classes.empty()) for(u32 i = 0; i < Module.Classes.size(); i++)
			{
				Brew::API::Class cls = Module.Classes[i];
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
	return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::evalFile(Brew::API::NativeContext Context)
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

Brew::API::Function Brew::BuiltIn::randRange(Brew::API::NativeContext Context)
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

void Brew::BuiltIn::initGlobal(Brew::API::NativeContext Context)
{
	Brew::API::Global = Brew::API::GlobalObject(Context);
	Brew::API::Global.pushFunction("require", require);
	Brew::API::Global.pushFunction("evalFile", evalFile);
	Brew::API::Global.pushFunction("randRange", randRange);
	Brew::API::Global.pushModule(Brew::BuiltIn::Console::initModule());
    Brew::API::Global.pushModule(Brew::BuiltIn::Process::initModule());
}
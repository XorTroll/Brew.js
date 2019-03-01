#include <bjs/js/Global.hpp>
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>

std::vector<bjs::js::Module> mods;

namespace bjs::js
{
    GlobalObject::GlobalObject(NativeContext Context)
    {
        this->ctx = Context;
    }

    void GlobalObject::PushString(std::string Name, std::string Value)
    {
        duk_push_string(this->ctx, Value.c_str());
        duk_put_global_string(this->ctx, Name.c_str());
    }

    void GlobalObject::PushInt(std::string Name, int Value)
    {
        duk_push_int(this->ctx, Value);
        duk_put_global_string(this->ctx, Name.c_str());
    }

    void GlobalObject::PushUInt(std::string Name, u32 Value)
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
        std::map<std::string, int> ints = Module.GetInts();
        if(!ints.empty()) for(auto const& intt : ints)
        {
            duk_push_int(this->ctx, intt.second);
            duk_put_prop_string(this->ctx, objidx, intt.first.c_str());
        }
        std::map<std::string, u32> uints = Module.GetUInts();
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

    void AddModule(js::Module &NewModule)
    {
        if(!mods.empty()) for(u32 i = 0; i < mods.size(); i++) if(mods[i].GetName() == NewModule.GetName()) return;
        mods.push_back(NewModule);
    }

    std::vector<js::Module> GetModules()
    {
        return mods;
    }

    void RemoveModule(std::string Name)
    {
        if(!mods.empty()) for(u32 i = 0; i < mods.size(); i++) if(mods[i].GetName() == Name)
        {
            mods.erase(mods.begin() + i);
            break;
        }
    }

    void ClearModules()
    {
        mods.clear();
    }

    js::Function require(js::NativeContext Context)
    {
        js::Module mmod("");
        bool found = false;
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            std::string xmodname = handler.GetString(0);
            std::string modname = xmodname;
            if(xmodname.substr(0, 2) == "./") modname = xmodname.substr(2);
            struct stat st;
            if(stat(modname.c_str(), &st) == 0)
            {
                if(st.st_mode & S_IFREG)
                {
                    std::ifstream ifs(modname);
                    std::stringstream strm;
                    strm << ifs.rdbuf();
                    ifs.close();
                    std::string base = strm.str();
                    std::string exports = base + ";exports";
                    int err = duk_peval_string(Context, exports.c_str());
                    if(err != 0)
                    {
                        std::string moduleexports = base + ";module.exports";
                        err = duk_peval_string(Context, moduleexports.c_str());
                        if(err != 0) js::ThrowError(Context, js::Error::CommonError, "Failed importing source file: " + std::string(duk_safe_to_string(Context, -1)));
                    }
                }
                else js::ThrowError(Context, js::Error::CommonError, "Attempted to import a folder (or something which is not a file?)");
            }
            else
            {
                std::transform(modname.begin(), modname.end(), modname.begin(), ::tolower);
                if(!mods.empty()) for(u32 i = 0; i < mods.size(); i++) if(mods[i].GetName() == modname)
                {
                    mmod = mods[i];
                    found = true;
                }
                if(found)
                {
                    duk_idx_t objidx = duk_push_object(Context);
                    std::map<std::string, std::string> strs = mmod.GetStrings();
                    if(!strs.empty()) for(auto const& str : strs)
                    {
                        duk_push_string(Context, str.second.c_str());
                        duk_put_prop_string(Context, objidx, str.first.c_str());
                    }
                    std::map<std::string, int> ints = mmod.GetInts();
                    if(!ints.empty()) for(auto const& intt : ints)
                    {
                        duk_push_int(Context, intt.second);
                        duk_put_prop_string(Context, objidx, intt.first.c_str());
                    }
                    std::map<std::string, u32> uints = mmod.GetUInts();
                    if(!uints.empty()) for(auto const& uint : uints)
                    {
                        duk_push_uint(Context, uint.second);
                        duk_put_prop_string(Context, objidx, uint.first.c_str());
                    }
                    std::map<std::string, double> dbls = mmod.GetDoubles();
                    if(!dbls.empty()) for(auto const& dbl : dbls)
                    {
                        duk_push_number(Context, dbl.second);
                        duk_put_prop_string(Context, objidx, dbl.first.c_str());
                    }
                    std::map<std::string, bool> bls = mmod.GetBooleans();
                    if(!bls.empty()) for(auto const& bol : bls)
                    {
                        duk_push_boolean(Context, bol.second);
                        duk_put_prop_string(Context, objidx, bol.first.c_str());
                    }
                    std::map<std::string, js::NativeFunction> fncs = mmod.GetFunctions();
                    if(!fncs.empty()) for(auto const& func : fncs)
                    {
                        duk_push_c_function(Context, func.second, DUK_VARARGS);
                        duk_put_prop_string(Context, objidx, func.first.c_str());
                    }
                    std::vector<std::string> udfs = mmod.GetUndefineds();
                    if(!udfs.empty()) for(u32 i = 0; i < udfs.size(); i++)
                    {
                        duk_push_undefined(Context);
                        duk_put_prop_string(Context, objidx, udfs[i].c_str());
                    }
                    std::vector<std::string> nlls = mmod.GetNulls();
                    if(!nlls.empty()) for(u32 i = 0; i < nlls.size(); i++)
                    {
                        duk_push_null(Context);
                        duk_put_prop_string(Context, objidx, nlls[i].c_str());
                    }
                    std::vector<std::string> nans = mmod.GetNaNs();
                    if(!nans.empty()) for(u32 i = 0; i < nans.size(); i++)
                    {
                        duk_push_nan(Context);
                        duk_put_prop_string(Context, objidx, nans[i].c_str());
                    }
                    std::vector<js::Class> clss = mmod.GetClasses();
                    if(!clss.empty()) for(u32 i = 0; i < clss.size(); i++)
                    {
                        js::Class cls = clss[i];
                        duk_push_c_function(Context, cls.GetConstructorFunction(), DUK_VARARGS);
                        duk_push_object(Context);
                        std::map<std::string, std::string> cstrs = cls.GetStrings();
                        if(!cstrs.empty()) for(auto const& str : cstrs)
                        {
                            duk_push_string(Context, str.second.c_str());
                            duk_put_prop_string(Context, -2, str.first.c_str());
                        }
                        std::map<std::string, int> cints = cls.GetInts();
                        if(!cints.empty()) for(auto const& intt : cints)
                        {
                            duk_push_int(Context, intt.second);
                            duk_put_prop_string(Context, -2, intt.first.c_str());
                        }
                        std::map<std::string, u32> cuints = cls.GetUInts();
                        if(!cuints.empty()) for(auto const& uint : cuints)
                        {
                            duk_push_uint(Context, uint.second);
                            duk_put_prop_string(Context, -2, uint.first.c_str());
                        }
                        std::map<std::string, double> cdbls = cls.GetDoubles();
                        if(!cdbls.empty()) for(auto const& dbl : cdbls)
                        {
                            duk_push_number(Context, dbl.second);
                            duk_put_prop_string(Context, -2, dbl.first.c_str());
                        }
                        std::map<std::string, bool> cbls = cls.GetBooleans();
                        if(!cbls.empty()) for(auto const& bol : cbls)
                        {
                            duk_push_boolean(Context, bol.second);
                            duk_put_prop_string(Context, -2, bol.first.c_str());
                        }
                        std::map<std::string, js::NativeFunction> cfncs = cls.GetFunctions();
                        if(!cfncs.empty()) for(auto const& func : cfncs)
                        {
                            duk_push_c_function(Context, func.second, DUK_VARARGS);
                            duk_put_prop_string(Context, -2, func.first.c_str());
                        }
                        std::vector<std::string> cudfs = cls.GetUndefineds();
                        if(!cudfs.empty()) for(u32 i = 0; i < cudfs.size(); i++)
                        {
                            duk_push_undefined(Context);
                            duk_put_prop_string(Context, -2, cudfs[i].c_str());
                        }
                        std::vector<std::string> cnlls = cls.GetNulls();
                        if(!cnlls.empty()) for(u32 i = 0; i < cnlls.size(); i++)
                        {
                            duk_push_null(Context);
                            duk_put_prop_string(Context, -2, cnlls[i].c_str());
                        }
                        std::vector<std::string> cnans = cls.GetNaNs();
                        if(!cnans.empty()) for(u32 i = 0; i < cnans.size(); i++)
                        {
                            duk_push_nan(Context);
                            duk_put_prop_string(Context, -2, cnans[i].c_str());
                        }
                        duk_put_prop_string(Context, -2, "prototype");
                        duk_put_prop_string(Context, objidx, cls.GetName().c_str());
                    }
                }
                else js::ThrowError(Context, js::Error::CommonError, "Cannot find module \'" + modname + "\'");
            }
        }
        return js::Return::Variable;
    }

    js::Function evalFile(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            std::string file = handler.GetString(0);
            std::ifstream ifs(file);
            if(ifs.good())
            {
                std::stringstream strm;
                strm << ifs.rdbuf();
                duk_get_global_string(Context, "eval");
                duk_push_string(Context, strm.str().c_str());
                duk_call(Context, 1);
            }
            ifs.close();
        }
        return js::Return::Void;
    }

    js::Function randRange(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(2))
        {
            int min = handler.GetInt(0);
            int max = handler.GetInt(1);
            int rnd = (min + rand() % (max - min));
            handler.ReturnInt(rnd);
        }
        return js::Return::Variable;
    }

    js::GlobalObject *InitializeGlobal(js::NativeContext Context)
    {
        js::GlobalObject *globl = new js::GlobalObject(Context);
        js::Object exports(Context);
        globl->InitializePushingObject("exports", exports);
        globl->FinalizePushingObject();
        js::Object module(Context);
        globl->InitializePushingObject("module", module);
        js::Object moduleexports(Context);
        module.InitializeAddingObject("exports", moduleexports);
        module.FinalizeAddingObject();
        globl->FinalizePushingObject();
        globl->PushFunction("require", require);
        globl->PushFunction("evalFile", evalFile);
        globl->PushFunction("randRange", randRange);
        return globl;
    }
}
#include <bjs/js/Module.hpp>

namespace bjs::js
{
    Module::Module(std::string Module)
    {
        this->name = Module;
    }

    void Module::PushString(std::string Name, std::string Value)
    {
        this->strs.insert(std::pair<std::string, std::string>(Name, Value));
    }

    void Module::PushInt(std::string Name, int Value)
    {
        this->ints.insert(std::pair<std::string, int>(Name, Value));
    }

    void Module::PushUInt(std::string Name, u32 Value)
    {
        this->uints.insert(std::pair<std::string, u32>(Name, Value));
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

    std::map<std::string, int> Module::GetInts()
    {
        return this->ints;
    }

    std::map<std::string, u32> Module::GetUInts()
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
}
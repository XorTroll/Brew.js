#include <bjs/js/Class.hpp>

namespace bjs::js
{
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
}
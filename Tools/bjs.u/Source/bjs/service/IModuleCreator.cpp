#include <bjs/service/IModuleCreator.hpp>

namespace bjs::service
{
    extern js::NativeContext ctx;
    extern Environment *env;

    Result IModuleCreator::Initialize(InBuffer<char> Name)
    {
        Result rc = 0;
        if(this->mod != NULL) rc = ResultAlreadyInitialized;
        if(rc == 0) this->mod = new js::Module(std::string(Name.buffer));
        return rc;
    }

    Result IModuleCreator::GetName(OutBuffer<char> Name)
    {
        Result rc = 0;
        if(this->mod == NULL) rc = ResultNotInitialized;
        if(rc == 0) strcpy(Name.buffer, this->mod->GetName().c_str());
        return rc;
    }

    Result IModuleCreator::PushString(InBuffer<char> Name, InBuffer<char> Value)
    {
        Result rc = 0;
        if(this->mod == NULL) rc = ResultNotInitialized;
        if(rc == 0) this->mod->PushString(std::string(Name.buffer), std::string(Value.buffer));
        return rc;
    }

    Result IModuleCreator::PushNumber(InBuffer<char> Name, u64 Value)
    {
        Result rc = 0;
        if(this->mod == NULL) rc = ResultNotInitialized;
        if(rc == 0) this->mod->PushUInt(std::string(Name.buffer), Value);
        return rc;
    }

    Result IModuleCreator::PushBoolean(InBuffer<char> Name, bool Value)
    {
        Result rc = 0;
        if(this->mod == NULL) rc = ResultNotInitialized;
        if(rc == 0) this->mod->PushBoolean(std::string(Name.buffer), Value);
        return rc;
    }
    
    Result IModuleCreator::Add()
    {
        Result rc = 0;
        if(this->mod == NULL) rc = ResultNotInitialized;
        if(rc == 0) js::AddModule(*this->mod);
        return rc;
    }
}
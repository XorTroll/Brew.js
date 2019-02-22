#include <bjs/service/IContextEditor.hpp>

namespace bjs::service
{
    extern js::NativeContext ctx;
    extern Environment *env;

    Result IContextEditor::PushGlobalString(InBuffer<char> Name, InBuffer<char> Value)
    {
        Result rc = 0;
        if(env == NULL) rc = ResultNotInitialized;
        if(rc == 0)
        {
            js::GlobalObject *gobj = env->GetGlobalObject();
            gobj->PushString(std::string(Name.buffer), std::string(Value.buffer));
        }
        return 0;
    }

    Result IContextEditor::PushGlobalNumber(InBuffer<char> Name, u64 Value)
    {
        Result rc = 0;
        if(env == NULL) rc = ResultNotInitialized;
        if(rc == 0)
        {
            js::GlobalObject *gobj = env->GetGlobalObject();
            gobj->PushUInt(std::string(Name.buffer), Value);
        }
        return rc;
    }

    Result IContextEditor::PushGlobalBoolean(InBuffer<char> Name, bool Value)
    {
        Result rc = 0;
        if(env == NULL) rc = ResultNotInitialized;
        if(rc == 0)
        {
            js::GlobalObject *gobj = env->GetGlobalObject();
            gobj->PushBoolean(std::string(Name.buffer), Value);
        }
        return rc;
    }

    Result IContextEditor::CreateModule(Out<std::shared_ptr<IModuleCreator>> OutService)
    {
        OutService.SetValue(std::move(std::make_shared<IModuleCreator>()));
        return 0;
    }
}
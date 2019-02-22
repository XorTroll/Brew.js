#include <bjs/service/IJavaScriptService.hpp>

namespace bjs::service
{
    js::NativeContext ctx;
    Environment *env;

    Result IJavaScriptService::Initialize(bool DefaultModules)
    {
        Result rc = 0;
        if(env != NULL) rc = ResultAlreadyInitialized;
        if(rc == 0)
        {
            ctx = js::CreateContext();
            if(ctx == NULL) rc = ResultBadNativeContext;
            if(rc == 0) env = new Environment(ctx, DefaultModules);
        }
        return rc;
    }

    Result IJavaScriptService::ResetContext(bool DefaultModules)
    {
        if(env != NULL) delete env;
        return this->Initialize(DefaultModules);
    }

    Result IJavaScriptService::IsInitialized(Out<bool> Initialized)
    {
        Initialized.SetValue(env != NULL);
        return 0;
    }

    Result IJavaScriptService::Evaluate(InBuffer<char> Code, OutBuffer<char> Output)
    {
        Result rc = 0;
        if(env == NULL) rc = ResultNotInitialized;
        if(rc == 0)
        {
            EvaluationResult evr = env->EvaluateSourceCode(std::string(Code.buffer));
            if(evr.IsFailure()) rc = ResultJavaScriptError;
            strcpy(Output.buffer, evr.AsString().c_str());
        }
        return rc;
    }

    Result IJavaScriptService::EditContext(Out<std::shared_ptr<IContextEditor>> OutService)
    {
        OutService.SetValue(std::move(std::make_shared<IContextEditor>()));
        return 0;
    }

    Result IJavaScriptService::Finalize()
    {
        if(env != NULL)
        {
            delete env;
            env = NULL;
        }
        return 0;
    }

    Result IJavaScriptService::RemoveModule(InBuffer<char> Name)
    {
        js::RemoveModule(std::string(Name.buffer));
        return 0;
    }
}
#include <bjs/sysmodule/JavaScriptService.hpp>

namespace bjs::sysmodule
{
    Result JavaScriptService::Initialize()
    {
        this->ctx = js::CreateContext();
        if(this->ctx == NULL) return ResultBadNativeContext;
        this->env = new Environment(this->ctx, true);
        return 0;
    }

    Result JavaScriptService::HasInitialized(Out<bool> Initialized)
    {
        bool init = false;
        // if(this->env != NULL) init = this->env->HasInitialized();
        Initialized.SetValue(init);
        return 0;
    }
    
    Result JavaScriptService::Evaluate(InPointer<const char> Source)
    {
        EvaluationResult res = this->env->EvaluateSourceCode(Source.pointer);
        if(res.IsFailure()) return ResultJavaScriptError;
        return 0;
    }
}
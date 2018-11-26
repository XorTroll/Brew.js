
#pragma once
#include <bjs/bjs.hpp>
#include <stratosphere.hpp>

namespace bjs::sysmodule
{
    class JavaScriptService final : public IServiceObject
    {   
        public:
            Result Initialize();
            Result HasInitialized(Out<bool> Initialized);
            Result Evaluate(InPointer<const char> Source);
            DEFINE_SERVICE_DISPATCH_TABLE
            {
                MakeServiceCommandMeta<0, &JavaScriptService::Initialize>(),
                MakeServiceCommandMeta<1, &JavaScriptService::HasInitialized>(),
                MakeServiceCommandMeta<2, &JavaScriptService::Evaluate>(),
            };
            js::NativeContext ctx;
            Environment *env = NULL;
    };

    static const u32 Module = 716;
    static const Result ResultBadNativeContext = MAKERESULT(Module, 1);
    static const Result ResultNotInitialized = MAKERESULT(Module, 2);
    static const Result ResultJavaScriptError = MAKERESULT(Module, 3);
}
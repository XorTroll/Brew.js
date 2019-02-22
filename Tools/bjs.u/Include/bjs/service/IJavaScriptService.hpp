
#pragma once
#include <bjs/service/Results.hpp>
#include <bjs/service/IContextEditor.hpp>

namespace bjs::service
{
    class IJavaScriptService : public IServiceObject
    {
        private:
            Result Initialize(bool DefaultModules);
            Result ResetContext(bool DefaultModules);
            Result IsInitialized(Out<bool> Initialized);
            Result Evaluate(InBuffer<char> Code, OutBuffer<char> Output);
            Result EditContext(Out<std::shared_ptr<IContextEditor>> OutService);
            Result Finalize();
            Result RemoveModule(InBuffer<char> Name);
        public:
            DEFINE_SERVICE_DISPATCH_TABLE
            {
                MakeServiceCommandMeta<0, &IJavaScriptService::Initialize>(),
                MakeServiceCommandMeta<1, &IJavaScriptService::ResetContext>(),
                MakeServiceCommandMeta<2, &IJavaScriptService::IsInitialized>(),
                MakeServiceCommandMeta<3, &IJavaScriptService::Evaluate>(),
                MakeServiceCommandMeta<4, &IJavaScriptService::EditContext>(),
                MakeServiceCommandMeta<5, &IJavaScriptService::Finalize>(),
                MakeServiceCommandMeta<6, &IJavaScriptService::RemoveModule>(),
            };
    };
}
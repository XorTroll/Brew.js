
#pragma once
#include <bjs/service/Results.hpp>
#include <bjs/service/IModuleCreator.hpp>

namespace bjs::service
{
    class IContextEditor : public IServiceObject
    {
        private:
            Result PushGlobalString(InBuffer<char> Name, InBuffer<char> Value);
            Result PushGlobalNumber(InBuffer<char> Name, u64 Value);
            Result PushGlobalBoolean(InBuffer<char> Name, bool Value);
            Result CreateModule(Out<std::shared_ptr<IModuleCreator>> OutService);
        public:
            DEFINE_SERVICE_DISPATCH_TABLE
            {
                MakeServiceCommandMeta<0, &IContextEditor::PushGlobalString>(),
                MakeServiceCommandMeta<1, &IContextEditor::PushGlobalNumber>(),
                MakeServiceCommandMeta<2, &IContextEditor::PushGlobalBoolean>(),
                MakeServiceCommandMeta<3, &IContextEditor::CreateModule>(),
            };
    };
}
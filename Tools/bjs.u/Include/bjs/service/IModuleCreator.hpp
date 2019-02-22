
#pragma once
#include <bjs/service/Results.hpp>

namespace bjs::service
{
    class IModuleCreator : public IServiceObject
    {
        private:
            Result Initialize(InBuffer<char> Name);
            Result GetName(OutBuffer<char> Name);
            Result PushString(InBuffer<char> Name, InBuffer<char> Value);
            Result PushNumber(InBuffer<char> Name, u64 Value);
            Result PushBoolean(InBuffer<char> Name, bool Value);
            Result Add();
        public:
            DEFINE_SERVICE_DISPATCH_TABLE
            {
                MakeServiceCommandMeta<0, &IModuleCreator::Initialize>(),
                MakeServiceCommandMeta<1, &IModuleCreator::GetName>(),
                MakeServiceCommandMeta<2, &IModuleCreator::PushString>(),
                MakeServiceCommandMeta<3, &IModuleCreator::PushNumber>(),
                MakeServiceCommandMeta<4, &IModuleCreator::PushBoolean>(),
                MakeServiceCommandMeta<5, &IModuleCreator::Add>(),
            };
        private:
            js::Module *mod = NULL;
    };
}
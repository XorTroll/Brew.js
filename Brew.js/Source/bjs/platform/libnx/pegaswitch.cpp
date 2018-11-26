#include <bjs/platform/libnx/pegaswitch.hpp>

namespace bjs::libnx::pegaswitch
{
    js::Module CreateModule()
    {
        js::Module pegaswitch("pegaswitch");
        js::Class cService = Service::CreateClass();
        pegaswitch.PushClass(cService);
        return pegaswitch;
    }
}
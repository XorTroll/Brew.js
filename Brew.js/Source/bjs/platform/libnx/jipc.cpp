#include <bjs/platform/libnx/jipc.hpp>

namespace bjs::libnx::jipc
{
    js::Module CreateModule()
    {
        js::Module jipc("jipc");
        js::Class cObject = Object::CreateClass();
        jipc.PushClass(cObject);
        jipc.PushFunction("getService", getService);
        return jipc;
    }
}
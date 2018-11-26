#include <bjs/platform/libnx/nx/Power.hpp>

namespace bjs::libnx::nx
{
    js::Function reboot(js::NativeContext Context)
    {
        bpcInitialize();
        bpcRebootSystem();
        bpcExit();
        return js::Return::Void;
    }

    js::Function shutDown(js::NativeContext Context)
    {
        bpcInitialize();
        bpcShutdownSystem();
        bpcExit();
        return js::Return::Void;
    }
}
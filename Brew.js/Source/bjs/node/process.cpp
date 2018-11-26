#include <bjs/node/process.hpp>

namespace bjs::node::process
{
    js::Module CreateModule()
    {
        js::Module process("process");
        std::string arch = "Unknown";
        #if defined bjsPlatformLibNX || defined bjsPlatformBiosphere
            arch = "ARM64";
        #elif defined bjsPlatformLibCTRU
            arch = "ARM";
        #endif
        process.PushString("arch", arch);
        std::string pl = "Unknown";
        #if defined bjsPlatformLibNX || defined bjsPlatformBiosphere
            pl = "NX";
        #elif defined bjsPlatformLibCTRU
            pl = "CTR";
        #endif
        process.PushString("platform", pl);
        process.PushString("version", js::Version);
        process.PushFunction("cwd", cwd);
        process.PushFunction("exit", exit);
        return process;
    }
}
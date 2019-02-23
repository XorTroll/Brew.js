#include <bjs/node/os.hpp>

namespace bjs::node::os
{
    js::Module CreateModule()
    {
        js::Module os("os");
        os.PushString("EOL", "\n");
        os.PushFunction("arch", arch);
        os.PushFunction("platform", platform);
        os.PushFunction("homedir", homedir);
        os.PushFunction("endianess", endianess);
        os.PushFunction("type", type);
        os.PushFunction("release", release);
        return os;       
    }
}
#include <Brew.js/Modules/Node/OS.hpp>

namespace Brew::BuiltIn::OS
{
    API::Function arch(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        string osarch;
        #if __curOS == 0
            osarch = "ARM64";
        #elif (__curOS == 1) || (__curOS == 2)
            osarch = "ARM";
        #endif
        handler.ReturnString(osarch);
        return API::Return::Variable;
    }

    API::Function platform(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        string ospl;
        #if __curOS == 0
            ospl = "NX";
        #elif __curOS == 1
            ospl = "CTR";
        #elif __curOS == 2
            ospl = "NTR";
        #endif
        handler.ReturnString(ospl);
        return API::Return::Variable;
    }

    API::Function homedir(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        string hdir;
        #if (__curOS == 0) || (__curOS == 1)
            hdir = "sdmc:/";
        #elif __curOS == 2
            hdir = "fat:/";
        #endif
        handler.ReturnString(hdir);
        return API::Return::Variable;
    }

    API::Function endianess(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        handler.ReturnString("LE");
        return API::Return::Variable;
    }

    API::Function type(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        string ost;
        #if __curOS == 0
            ost = "Nintendo Switch";
        #elif __curOS == 1
            ost = "Nintendo 3DS / 2DS";
        #elif __curOS == 2
            ost = "Nintendo DS / DSi";
        #endif
        handler.ReturnString(ost);
        return API::Return::Variable;
    }

    API::Module Initialize()
    {
        API::Module os("os");
        os.PushString("EOL", "\n");
        os.PushFunction("arch", arch);
        os.PushFunction("platform", platform);
        os.PushFunction("homedir", homedir);
        os.PushFunction("endianess", endianess);
        os.PushFunction("type", type);
        return os;
    }
}
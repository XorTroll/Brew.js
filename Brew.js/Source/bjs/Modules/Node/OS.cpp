#include <bjs/Modules/Node/OS.hpp>

namespace bjs::modules::OS
{
    js::Function arch(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        std::string osarch = "Unknown";
        #if __curOS == 0
            osarch = "ARM64";
        #elif (__curOS == 1) || (__curOS == 2)
            osarch = "ARM";
        #endif
        handler.ReturnString(osarch);
        return js::Return::Variable;
    }

    js::Function platform(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        std::string ospl = "Unknown";
        #if __curOS == 0
            ospl = "NX";
        #elif __curOS == 1
            ospl = "CTR";
        #elif __curOS == 2
            ospl = "NTR";
        #endif
        handler.ReturnString(ospl);
        return js::Return::Variable;
    }

    js::Function homedir(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        std::string hdir = "/";
        #if (__curOS == 0) || (__curOS == 1)
            hdir = "sdmc:/";
        #elif __curOS == 2
            hdir = "fat:/";
        #endif
        handler.ReturnString(hdir);
        return js::Return::Variable;
    }

    js::Function endianess(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        handler.ReturnString("LE");
        return js::Return::Variable;
    }

    js::Function type(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        std::string ost = "Unknown";
        #if __curOS == 0
            ost = "Nintendo Switch";
        #elif __curOS == 1
            ost = "Nintendo 3DS / 2DS";
        #elif __curOS == 2
            ost = "Nintendo DS / DSi";
        #endif
        handler.ReturnString(ost);
        return js::Return::Variable;
    }

    js::Module Initialize()
    {
        js::Module os("os");
        os.PushString("EOL", "\n");
        os.PushFunction("arch", arch);
        os.PushFunction("platform", platform);
        os.PushFunction("homedir", homedir);
        os.PushFunction("endianess", endianess);
        os.PushFunction("type", type);
        return os;
    }
}
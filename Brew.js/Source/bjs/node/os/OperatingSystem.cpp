#include <bjs/node/os/OperatingSystem.hpp>

namespace bjs::node::os
{
    js::Function arch(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        std::string osarch = "Unknown";
        #if defined bjsPlatformLibNX || defined bjsPlatformBiosphere
            osarch = "ARM64";
        #elif defined bjsPlatformLibCTRU
            osarch = "ARM";
        #endif
        handler.ReturnString(osarch);
        return js::Return::Variable;
    }

    js::Function platform(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        std::string ospl = "Unknown";
        #if defined bjsPlatformLibNX || defined bjsPlatformBiosphere
            ospl = "nx";
        #elif defined bjsPlatformLibCTRU
            ospl = "ctr";
        #endif
        handler.ReturnString(ospl);
        return js::Return::Variable;
    }

    js::Function homedir(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        std::string hdir = "sdmc:/";
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
        #if defined bjsPlatformLibNX || defined bjsPlatformBiosphere
            ost = "Nintendo Switch";
        #elif defined bjsPlatformLibCTRU
            ost = "Nintendo 3DS";
        #endif
        handler.ReturnString(ost);
        return js::Return::Variable;
    }

    js::Function release(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        Result rc = setsysInitialize();
        if(rc == 0)
        {
            SetSysFirmwareVersion fw;
            rc = setsysGetFirmwareVersion(&fw);
            if(rc == 0) handler.ReturnString(std::string(fw.display_version));
            setsysExit();
        }
        return js::Return::Variable;
    }
}
#include <bjs/Modules/Node/Process.hpp>

namespace bjs::modules::Process
{
    js::Function cwd(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        std::string cwd = "/";
        #if (__curOS == 0) || (__curOS == 1)
            cwd = "sdmc/:";
        #elif __curOS == 2
            cwd = "fat:/";
        #endif
        handler.ReturnString(cwd);
        return js::Return::Variable;
    }

    js::Function exit(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        int code = 0;
        if(handler.CheckArgcEqualOrBigger(1)) code = handler.GetInt(0);
        ::exit(code);
        return js::Return::Void;
    }

    js::Module Initialize()
    {
        js::Module process("process");
        std::string arch = "Unknown";
        #if __curOS == 0
            arch = "ARM64";
        #elif (__curOS == 1) || (__curOS == 2)
            arch = "ARM";
        #endif
        process.PushString("arch", arch);
        /*
        u64 pid = 0;
        #if __curOS == 0
            pmdmntInitialize();
            pmdmntGetApplicationPid(&pid);
            pmdmntExit();
        #endif
        process.PushUInt("pid", pid);
        */
        std::string pl = "Unknown";
        #if __curOS == 0
            pl = "NX";
        #elif __curOS == 1
            pl = "CTR";
        #elif __curOS == 2
            pl = "NTR";
        #endif
        process.PushString("platform", pl);
        process.PushString("version", js::Version);
        process.PushFunction("cwd", cwd);
        process.PushFunction("exit", exit);
        return process;
    }
}
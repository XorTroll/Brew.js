#include <Brew.js/Modules/Node/Process.hpp>

namespace Brew::BuiltIn::Process
{
    API::Function cwd(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        string cwd;
        #if (__curOS == 0) || (__curOS == 1)
            cwd = "sdmc/:";
        #elif __curOS == 2
            cwd = "fat:/";
        #endif
        handler.ReturnString(cwd);
        return API::Return::Variable;
    }

    API::Function exit(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        int code = 0;
        if(handler.CheckArgcEqualOrBigger(1)) code = handler.GetInt(0);
        ::exit(code);
        return API::Return::Void;
    }

    API::Module Initialize()
    {
        API::Module process("process");
        string arch;
        #if __curOS == 0
            arch = "ARM64";
        #elif (__curOS == 1) || (__curOS == 2)
            arch = "ARM";
        #endif
        process.PushString("arch", arch);
        u64 pid = 0;
        #if __curOS == 0
            pmdmntInitialize();
            pmdmntGetApplicationPid(&pid);
            pmdmntExit();
        #endif
        process.PushUInt("pid", pid);
        string pl;
        #if __curOS == 0
            pl = "NX";
        #elif __curOS == 1
            pl = "CTR";
        #elif __curOS == 2
            pl = "NTR";
        #endif
        process.PushString("platform", pl);
        process.PushString("version", API::Version);
        process.PushFunction("cwd", cwd);
        process.PushFunction("exit", exit);
        return process;
    }
}
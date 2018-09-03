#include "Process.hpp"

Brew::API::Module process("process");

Brew::API::Function Brew::BuiltIn::Process::cwd(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    string cwd;
    #if __curOS == 0 || __curOS == 1

        cwd = "sdmc:";

    #elif __curOS == 2

        cwd = "/";

    #endif;
    handler.pushString(cwd);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::Process::exit(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    int code = 0;
    if(handler.checkArgc(1)) code = handler.getInt(0);
    ::exit(code);
    return Brew::API::Return::Void;
}

Brew::API::Module Brew::BuiltIn::Process::initModule()
{
    string arch;
    #if __curOS == 0

        arch = "ARM64";

    #elif __curOS == 1 || __curOS == 2

        arch = "ARM";

    #endif
    process.pushString("arch", arch);
    u64 pid = 0;
    #if __curOS == 0

        pmdmntInitialize();
        pmdmntGetApplicationPid(&pid);
        pmdmntExit();
        
    #endif
    process.pushUInt("pid", pid);
    string pl;
    #if __curOS == 0

        pl = "NX";

    #elif __curOS == 1

        pl = "CTR";

    #elif __curOS == 2

        pl = "NTR";

    #endif
    process.pushString("platform", pl);
    process.pushString("version", Brew::API::Version);
    process.pushFunction("cwd", cwd);
    process.pushFunction("exit", exit);
    return process;
}
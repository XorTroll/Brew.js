#include "OS.hpp"

Brew::API::Module os("os");

Brew::API::Function Brew::BuiltIn::OS::arch(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    string osarch;
    #if __curOS == 0

        osarch = "ARM64";

    #elif __curOS == 1 || __curOS == 2

        osarch = "ARM";

    #endif
    handler.pushString(osarch);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::OS::platform(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    string ospl;
    #if __curOS == 0

        ospl = "NX";

    #elif __curOS == 1

        ospl = "CTR";

    #elif __curOS == 2

        ospl = "NTR";

    #endif
    handler.pushString(ospl);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::OS::homedir(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    string hdir;
    #if __curOS == 0 || __curOS == 1

        hdir = "sdmc:";

    #elif __curOS == 2

        hdir = "/";

    #endif
    handler.pushString(hdir);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::OS::endianess(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    string osarch = "LE";
    handler.pushString(osarch);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::OS::type(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    string oss;
    #if __curOS == 0

        oss = "Nintendo Switch";

    #elif __curOS == 1

        oss = "Nintendo 3DS";

    #elif __curOS == 2

        oss = "Nintendo DS/DSi";

    #endif
    handler.pushString(oss);
    return Brew::API::Return::Variable;
}

Brew::API::Module Brew::BuiltIn::OS::initModule()
{
    os.pushString("EOL", "\n");
    os.pushFunction("arch", arch);
    os.pushFunction("platform", platform);
    os.pushFunction("homedir", homedir);
    os.pushFunction("endianess", endianess);
    os.pushFunction("type", type);
    return os;
}
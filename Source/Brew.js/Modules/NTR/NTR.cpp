#include "NTR.hpp"

Brew::API::Module ntr("ntr");

Brew::API::Function Brew::BuiltIn::NTR::isDSi(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    handler.pushBoolean(isDSiMode());
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::NTR::shutDown(Brew::API::NativeContext Context)
{
    systemShutDown();
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::NTR::sleep(Brew::API::NativeContext Context)
{
    systemSleep();
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::NTR::batteryLevel(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    handler.pushUInt(getBatteryLevel());
    return Brew::API::Return::Variable;
}

Brew::API::Module Brew::BuiltIn::NTR::initModule()
{
    string username;
    for(int i = 0; i < PersonalData->nameLen; i++) username += (char)PersonalData->name[i];
    ntr.pushString("UserName", username);
    string message;
    for(int i = 0; i < PersonalData->messageLen; i++) message += (char)PersonalData->message[i];
    ntr.pushString("UserMessage", message);
    ntr.pushString("ROMTitle", string(__NDSHeader->gameTitle));
    ntr.pushString("ROMCode", string(__NDSHeader->gameCode));
    ntr.pushFunction("isDSi", isDSi);
    ntr.pushFunction("shutDown", shutDown);
    ntr.pushFunction("sleep", sleep);
    ntr.pushFunction("batteryLevel", batteryLevel);
    return ntr;
}
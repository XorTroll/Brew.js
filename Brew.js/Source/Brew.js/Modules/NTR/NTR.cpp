#include <Brew.js/Modules/NTR/NTR.hpp>

namespace Brew::BuiltIn::NTR
{
    API::Function isDSi(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        handler.ReturnBoolean(isDSiMode());
        return API::Return::Variable;
    }

    API::Function shutDown(API::NativeContext Context)
    {
        systemShutDown();
        return API::Return::Void;
    }

    API::Function sleep(API::NativeContext Context)
    {
        systemSleep();
        return API::Return::Void;
    }

    API::Function batteryLevel(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        handler.ReturnUInt(getBatteryLevel());
        return API::Return::Variable;
    }

    API::Module Initialize()
    {
        API::Module ntr("ntr");
        string username;
        for(int i = 0; i < PersonalData->nameLen; i++) username += (char)PersonalData->name[i];
        ntr.PushString("UserName", username);
        string message;
        for(int i = 0; i < PersonalData->messageLen; i++) message += (char)PersonalData->message[i];
        ntr.PushString("UserMessage", message);
        ntr.PushString("GameTitle", string(__NDSHeader->gameTitle));
        ntr.PushString("GameCode", string(__NDSHeader->gameCode));
        ntr.PushFunction("isDSi", isDSi);
        ntr.PushFunction("shutDown", shutDown);
        ntr.PushFunction("sleep", sleep);
        ntr.PushFunction("batteryLevel", batteryLevel);
        return ntr;
    }
}
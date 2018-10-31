#include <bjs/Modules/NTR/NTR.hpp>

namespace bjs::modules::NTR
{
    js::Function isDSi(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        handler.ReturnBoolean(isDSiMode());
        return js::Return::Variable;
    }

    js::Function shutDown(js::NativeContext Context)
    {
        systemShutDown();
        return js::Return::Void;
    }

    js::Function sleep(js::NativeContext Context)
    {
        systemSleep();
        return js::Return::Void;
    }

    js::Function batteryLevel(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        handler.ReturnUInt(getBatteryLevel());
        return js::Return::Variable;
    }

    js::Module Initialize()
    {
        js::Module ntr("ntr");
        std::string username;
        for(int i = 0; i < PersonalData->nameLen; i++) username += (char)PersonalData->name[i];
        ntr.PushString("UserName", username);
        std::string message;
        for(int i = 0; i < PersonalData->messageLen; i++) message += (char)PersonalData->message[i];
        ntr.PushString("UserMessage", message);
        ntr.PushString("GameTitle", std::string(__NDSHeader->gameTitle));
        ntr.PushString("GameCode", std::string(__NDSHeader->gameCode));
        ntr.PushFunction("isDSi", isDSi);
        ntr.PushFunction("shutDown", shutDown);
        ntr.PushFunction("sleep", sleep);
        ntr.PushFunction("batteryLevel", batteryLevel);
        return ntr;
    }
}
#include "CTR.hpp"

Brew::API::Module ctr("ctr");

Brew::API::Function Brew::BuiltIn::CTR::isNew3DS(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    bool n3d = false;
    APT_CheckNew3DS(&n3d);
    handler.pushBoolean(n3d);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::CTR::is2DS(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    u8 val;
    cfguInit();
    CFGU_GetModelNintendo2DS(&val);
    cfguExit();
    bool is2d = (val == 0);
    handler.pushBoolean(is2d);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::CTR::launchLibraryApplet(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(handler.checkArgc(1))
    {
        u64 id = handler.getUInt(0);
        u32 buff[0x100];
        memset(buff, 0, sizeof(buff));
        aptLaunchLibraryApplet((NS_APPID)id, buff, sizeof(buff), 0);
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::CTR::batteryLevel(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    u8 batt = 255;
    mcuHwcInit();
    MCUHWC_GetBatteryLevel(&batt);
    mcuHwcExit();
    handler.pushUInt(batt);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::CTR::reboot(Brew::API::NativeContext Context)
{
    Handle serviceHandle = 0;
    srvGetServiceHandle(&serviceHandle, "ptm:sysm");
    u32 *commandBuffer = getThreadCommandBuffer();
    commandBuffer[0] = 0x040700C0;
    commandBuffer[1] = 0x00000000;
    commandBuffer[2] = 0x00000000;
    commandBuffer[3] = 0x00000000;
    svcSendSyncRequest(serviceHandle);
    svcCloseHandle(serviceHandle);
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::CTR::shutDown(Brew::API::NativeContext Context)
{
    Handle serviceHandle = 0;
    srvGetServiceHandle(&serviceHandle, "ptm:sysm");
    u32 *commandBuffer = getThreadCommandBuffer();
    commandBuffer[0] = 0x040700C0;
    commandBuffer[1] = 0x00000000;
    commandBuffer[2] = 0x00000000;
    commandBuffer[3] = 0x00000000;
    svcSendSyncRequest(serviceHandle);
    svcCloseHandle(serviceHandle);
    exit(0);
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::CTR::isCharging(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    u8 data = 0;
    PTMU_GetBatteryChargeState(&data);
    bool chg = (data == 1);
    handler.pushBoolean(chg);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::CTR::sendNotification(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(handler.checkArgc(2))
    {
        string title = handler.getString(0);
        string message = handler.getString(1);
        vector<u16> ititle;
        for(int i = 0; i < title.length(); i++) ititle.push_back((u16)title[i]);
        ititle.push_back('\0');
        vector<u16> imessage;
        for(int i = 0; i < message.length(); i++) imessage.push_back((u16)message[i]);
        imessage.push_back('\0');
        newsInit();
        NEWS_AddNotification(ititle.data(), ititle.size(), imessage.data(), imessage.size(), NULL, NULL, false);
        newsExit();
    }
    return Brew::API::Return::Void;
}

Brew::API::Module Brew::BuiltIn::CTR::initModule()
{
    u8 val = 0;
    cfguInit();
    CFGU_GetSystemModel(&val);
    cfguExit();
    string model;
    switch(val)
    {
        case 0:
            model = "3DS";
            break;
        case 1:
            model = "3DS XL";
            break;
        case 2:
            model = "New 3DS";
            break;
        case 3:
            model = "2DS";
            break;
        case 4:
            model = "New 3DS XL";
            break;
        case 5:
            model = "2DS XL";
            break;
        default:
            model = "Unknown";
            break;
    }
    ctr.pushString("ModelName", model);
    u8 lang = 1;
    string slang = "Unknown";
    cfguInit();
    CFGU_GetSystemLanguage(&lang);
    cfguExit();
    switch(lang)
    {
        case 0:
            slang = "Japanese";
            break;
        case 1:
            slang = "English";
            break;
        case 2:
            slang = "French";
            break;
        case 3:
            slang = "German";
            break;
        case 4:
            slang = "Italian";
            break;
        case 5:
            slang = "Spanish";
            break;
        case 6:
            slang = "Simplified Chinese";
            break;
        case 7:
            slang = "Korean";
            break;
        case 8:
            slang = "Dutch";
            break;
        case 9:
            slang = "Portuguese";
            break;
        case 10:
            slang = "Russian";
            break;
        case 11:
            slang = "Traditional Chinese";
            break;
        default:
            slang = "Unknown";
            break;
    }
    ctr.pushString("Language", slang);
    ctr.pushFunction("isNew3DS", isNew3DS);
    ctr.pushFunction("is2DS", is2DS);
    ctr.pushFunction("launchLibraryApplet", launchLibraryApplet);
    ctr.pushFunction("batteryLevel", batteryLevel);
    ctr.pushFunction("reboot", reboot);
    ctr.pushFunction("shutDown", shutDown);
    ctr.pushFunction("isCharging", isCharging);
    ctr.pushFunction("sendNotification", sendNotification);
    return ctr;
}
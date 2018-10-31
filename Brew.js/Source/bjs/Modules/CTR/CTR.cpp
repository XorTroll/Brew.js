#include <bjs/Modules/CTR/CTR.hpp>

namespace bjs::modules::CTR
{
    js::Function isNew3DS(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        bool n3d = false;
        APT_CheckNew3DS(&n3d);
        handler.ReturnBoolean(n3d);
        return js::Return::Variable;
    }

    js::Function is2DS(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        u8 val;
        cfguInit();
        CFGU_GetModelNintendo2DS(&val);
        cfguExit();
        bool is2d = (val == 0);
        handler.ReturnBoolean(is2d);
        return js::Return::Variable;
    }

    js::Function launchLibraryApplet(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            u64 id = handler.GetUInt(0);
            u32 buff[0x100];
            memset(buff, 0, sizeof(buff));
            aptLaunchLibraryApplet((NS_APPID)id, buff, sizeof(buff), 0);
        }
        return js::Return::Void;
    }

    js::Function batteryLevel(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        u8 batt = 255;
        mcuHwcInit();
        MCUHWC_GetBatteryLevel(&batt);
        mcuHwcExit();
        handler.ReturnUInt(batt);
        return js::Return::Variable;
    }

    js::Function reboot(js::NativeContext Context)
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
        return js::Return::Void;
    }

    js::Function shutDown(js::NativeContext Context)
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
        return js::Return::Void;
    }

    js::Function isCharging(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        u8 data = 0;
        PTMU_GetBatteryChargeState(&data);
        bool chg = (data == 1);
        handler.ReturnBoolean(chg);
        return js::Return::Variable;
    }

    js::Function sendNotification(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(2))
        {
            std::string title = handler.GetString(0);
            std::string message = handler.GetString(1);
            std::vector<u16> ititle;
            for(u32 i = 0; i < title.length(); i++) ititle.push_back((u16)title[i]);
            ititle.push_back('\0');
            std::vector<u16> imessage;
            for(u32 i = 0; i < message.length(); i++) imessage.push_back((u16)message[i]);
            imessage.push_back('\0');
            newsInit();
            NEWS_AddNotification(ititle.data(), ititle.size(), imessage.data(), imessage.size(), NULL, 0, false);
            newsExit();
        }
        return js::Return::Void;
    }

    js::Module Initialize()
    {
        js::Module ctr("ctr");
        u8 val = 0;
        cfguInit();
        CFGU_GetSystemModel(&val);
        cfguExit();
        std::string model = "Unknown";
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
        }
        ctr.PushString("ModelName", model);
        u8 lang = 1;
        std::string slang = "Unknown";
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
        }
        ctr.PushString("Language", slang);
        ctr.PushFunction("isNew3DS", isNew3DS);
        ctr.PushFunction("is2DS", is2DS);
        ctr.PushFunction("launchLibraryApplet", launchLibraryApplet);
        ctr.PushFunction("batteryLevel", batteryLevel);
        ctr.PushFunction("reboot", reboot);
        ctr.PushFunction("shutDown", shutDown);
        ctr.PushFunction("isCharging", isCharging);
        ctr.PushFunction("sendNotification", sendNotification);
        return ctr;
    }
}
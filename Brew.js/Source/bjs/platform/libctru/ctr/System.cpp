#include <bjs/platform/libctru/ctr/System.hpp>

namespace bjs::libctru::ctr
{
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

    js::Function getBatteryLevel(js::NativeContext Context)
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
}
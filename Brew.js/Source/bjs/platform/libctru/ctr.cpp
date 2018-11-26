#include <bjs/platform/libctru/ctr.hpp>

namespace bjs::libctru::ctr
{
    js::Module CreateModule()
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
        ctr.PushFunction("getBatteryLevel", getBatteryLevel);
        ctr.PushFunction("reboot", reboot);
        ctr.PushFunction("shutDown", shutDown);
        ctr.PushFunction("isCharging", isCharging);
        ctr.PushFunction("sendNotification", sendNotification);
        return ctr;
    }
}
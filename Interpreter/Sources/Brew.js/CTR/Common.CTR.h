#include "Includes.CTR.h"

namespace game
{
    // game.flush()
    inline duk_ret_t CFUNC_flush(duk_context *ctx)
    {
        gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
        return 1;
    }
}

namespace device
{
    // device.reboot()
    inline duk_ret_t CFUNC_reboot(duk_context *ctx)
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
        return 1;
    }

    // device.powerOff()
    inline duk_ret_t CFUNC_powerOff(duk_context *ctx)
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
        return 1;
    }

    // device.batteryLevel() -> number
    inline duk_ret_t CFUNC_batteryLevel(duk_context *ctx)
    {
        u8 data = 0;
        PTMU_GetBatteryLevel(&data);
        duk_push_number(ctx, (int)data * 20);
        return 1;
    }

    // device.isCharging() -> boolean
    inline duk_ret_t CFUNC_isCharging(duk_context *ctx)
    {
        u8 data = 0;
        PTMU_GetBatteryChargeState(&data);
        bool chg = (data == 1) ? true : false;
        duk_push_boolean(ctx, chg);
        return 1;
    }

    //device.language() -> string
    inline duk_ret_t CFUNC_language(duk_context *ctx)
    {
        u8 lang;
        string slang;
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
        duk_push_string(ctx, slang.c_str());
        return 1;
    }
}

namespace input
{
    // input.checkPressed() -> key
    inline duk_ret_t CFUNC_checkPressed(duk_context *ctx)
    {
        hidScanInput();
        int key = hidKeysDown();
        duk_push_number(ctx, key);
        return 1;
    }

    // input.checkReleased() -> key
    inline duk_ret_t CFUNC_checkReleased(duk_context *ctx)
    {
        hidScanInput();
        int key = hidKeysUp();
        duk_push_number(ctx, key);
        return 1;
    }

    inline duk_ret_t CFUNC_checkHeld(duk_context *ctx)
    {
        // input.checkHeld() -> key
        hidScanInput();
        int key = hidKeysHeld();
        duk_push_number(ctx, key);
        return 1;
    }

    // [partial] input.touchData().X -> number
    inline duk_ret_t PART_touchData_x(duk_context *ctx)
    {
        hidScanInput();
        touchPosition tch;
        hidTouchRead(&tch);
        int x = tch.px;
        duk_push_number(ctx, x);
        return 1;
    }

    // [partial] input.touchData().Y -> number
    inline duk_ret_t PART_touchData_y(duk_context *ctx)
    {
        hidScanInput();
        touchPosition tch;
        hidTouchRead(&tch);
        int y = tch.py;
        duk_push_number(ctx, y);
        return 1;
    }
}

namespace fs
{
    // fs.pathSDMC(Path) -> string
    inline duk_ret_t CFUNC_pathSDMC(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            string path = string(duk_to_string(ctx, 0));
            string spath = "sdmc:/" + path;
            duk_push_string(ctx, spath.c_str());
        }
        return 1;
    }

    // fs.pathRomFS(Path) -> string
    inline duk_ret_t CFUNC_pathRomFS(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            string path = string(duk_to_string(ctx, 0));
            string spath = "romfs:/" + path;
            duk_push_string(ctx, spath.c_str());
        }
        return 1;
    }
}

namespace console
{
    inline gfxScreen_t NOJS_parsescreen(int screen)
    {
        return (screen == 0) ? GFX_TOP : GFX_BOTTOM;
    }

    static PrintConsole top, bottom;

    // console.init()
    // console.init(Screen)
    inline duk_ret_t CFUNC_init(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int iscreen = duk_to_number(ctx, 0);
            gfxScreen_t screen = NOJS_parsescreen(iscreen);
            if(iscreen == 0) consoleInit(screen, &top);
            else if(iscreen == 1) consoleInit(screen, &bottom);
        }
        else if(duk_get_top(ctx) == 0)
        {
            consoleInit(GFX_TOP, &top);
        }
        return 1;
    }

    // console.setScreen(Screen)
    inline duk_ret_t CFUNC_setScreen(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int screen = duk_to_number(ctx, 0);
            if(screen == 0) consoleSelect(&top);
            else if(screen == 1) consoleSelect(&bottom);
        }
        return 1;
    }
}

void CTX_CommonCTR(duk_context *ctx)
{
    duk_push_c_function(ctx, game::CFUNC_flush, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__game_flush");
    duk_push_c_function(ctx, device::CFUNC_reboot, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__device_reboot");
    duk_push_c_function(ctx, device::CFUNC_powerOff, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__device_powerOff");
    duk_push_c_function(ctx, device::CFUNC_batteryLevel, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__device_batteryLevel");
    duk_push_c_function(ctx, device::CFUNC_isCharging, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__device_isCharging");
    duk_push_c_function(ctx, device::CFUNC_language, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__device_language");
    duk_push_c_function(ctx, input::CFUNC_checkPressed, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__input_checkPressed");
    duk_push_c_function(ctx, input::CFUNC_checkReleased, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__input_checkReleased");
    duk_push_c_function(ctx, input::CFUNC_checkHeld, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__input_checkHeld");
    duk_push_c_function(ctx, input::PART_touchData_x, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__input_touchData_x");
    duk_push_c_function(ctx, input::PART_touchData_y, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__input_touchData_y");
    duk_push_c_function(ctx, fs::CFUNC_pathSDMC, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__fs_pathSDMC");
    duk_push_c_function(ctx, fs::CFUNC_pathRomFS, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__fs_pathRomFS");
    duk_push_c_function(ctx, console::CFUNC_init, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__console_init");
    duk_push_c_function(ctx, console::CFUNC_setScreen, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__console_setScreen");
}

string JS_CommonCTR()
{
    string js = "";
    js += "game.flush=function(){__CFUNC__game_flush();};";
    js += "device.Platform='CTR';";
    js += "device.PlatformName='Nintendo 3DS';";
    js += "device.reboot=function(){__CFUNC__device_reboot();};";
    js += "device.powerOff=function(){__CFUNC__device_powerOff();};";
    js += "device.batteryLevel=function(){return __CFUNC__device_batteryLevel();};";
    js += "device.isCharging=function(){return __CFUNC__device_isCharging();};";
    js += "device.language=function(){return __CFUNC__device_language();};";
    js += "device.screen.TopWidth=400;";
    js += "device.screen.BottomWidth=320;";
    js += "device.screen.Height=240;";
    js += "input.None=0;";
	js += "input.A=1;";
	js += "input.B=2;";
	js += "input.Select=4;";
	js += "input.Start=8;";
	js += "input.DPadRight=16;";
	js += "input.DPadLeft=32;";
	js += "input.DPadUp=64;";
	js += "input.DPadDown=128;";
	js += "input.R=256;";
	js += "input.L=512;";
	js += "input.X=1024;";
	js += "input.Y=2048;";
	js += "input.ZL=16384;";
	js += "input.ZR=32768;";
	js += "input.Touch=1048576;";
	js += "input.CStickRight=16777216;";
	js += "input.CStickLeft=33554432;";
	js += "input.CStickUp=67108864;";
	js += "input.CStickDown=134217728;";
	js += "input.CPadRight=268435456;";
	js += "input.CPadLeft=536870912;";
	js += "input.CPadUp=1073741824;";
	js += "input.CPadDown=2147483648;";
    js += "input.Up=input.DPadUp|input.CPadUp;";
    js += "input.Down=input.DPadDown|input.CPadDown;";
    js += "input.Left=input.DPadLeft|input.CPadLeft;";
    js += "input.Right=input.DPadRight|input.CPadRight;";
    js += "input.checkHeld=function(){return __CFUNC__input_checkHeld();};";
    js += "input.checkPressed=function(){return __CFUNC__input_checkPressed();};";
    js += "input.checkReleased=function(){return __CFUNC__input_checkReleased();};";
    js += "input.touchData=function(){var tch=[];tch.X=__CFUNC__PART__input_touchData_x();tch.Y=__CFUNC__PART__input_touchData_y();return tch;};";
    js += "fs.pathSDMC=function(Path){return __CFUNC__fs_pathSDMC(Path);};";
    js += "fs.pathRomFS=function(Path){return __CFUNC__fs_pathRomFS(Path);};";
    js += "console.init=function(Screen){__CFUNC__console_init(Screen);};";
    js += "console.setScreen=function(Screen){__CFUNC__console_setScreen(Screen);};";
    return js;
}
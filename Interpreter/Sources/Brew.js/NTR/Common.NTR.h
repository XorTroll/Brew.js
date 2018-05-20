#include "Includes.NTR.h"

namespace game
{
    // game.flush()
    inline duk_ret_t CFUNC_flush(duk_context *ctx)
    {
        swiWaitForVBlank();
        return 1;
    }
}

namespace device
{
    // device.powerOff()
    inline duk_ret_t CFUNC_powerOff(duk_context *ctx)
    {
        systemShutDown();
        return 1;
    }

    // device.batteryLevel() -> number
    inline duk_ret_t CFUNC_batteryLevel(duk_context *ctx)
    {
        duk_push_number(ctx, getBatteryLevel());
        return 1;
    }

    // device.language() -> string
    inline duk_ret_t CFUNC_language(duk_context *ctx)
    {
        int lang = PersonalData->language;
        string slang;
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
                slang = "Chinese";
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
        scanKeys();
        int key = keysDown();
        duk_push_number(ctx, key);
        return 1;
    }

    // input.checkReleased() -> key
    inline duk_ret_t CFUNC_checkReleased(duk_context *ctx)
    {
        scanKeys();
        int key = keysUp();
        duk_push_number(ctx, key);
        return 1;
    }

    // input.checkHeld() -> key
    inline duk_ret_t CFUNC_checkHeld(duk_context *ctx)
    {
        scanKeys();
        int key = keysCurrent();
        duk_push_number(ctx, key);
        return 1;
    }

    // [partial] input.touchData().X -> number
    inline duk_ret_t PART_touchData_x(duk_context *ctx)
    {
        scanKeys();
        touchPosition tch;
        touchRead(&tch);
        int x = tch.px;
        duk_push_number(ctx, x);
        return 1;
    }

    // [partial] input.touchData().Y -> number
    inline duk_ret_t PART_touchData_y(duk_context *ctx)
    {
        scanKeys();
        touchPosition tch;
        touchRead(&tch);
        int y = tch.py;
        duk_push_number(ctx, y);
        return 1;
    }
}

namespace fs
{
    // fs.pathFAT(Path) -> string
    inline duk_ret_t CFUNC_pathFAT(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            string path = string(duk_to_string(ctx, 0));
            string spath = "fat:/" + path;
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
            string spath = "nitro:/" + path;
            duk_push_string(ctx, spath.c_str());
        }
        return 1;
    }
}

void CTX_CommonNTR(duk_context *ctx)
{
    duk_push_c_function(ctx, game::CFUNC_flush, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__game_flush");
    duk_push_c_function(ctx, device::CFUNC_powerOff, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__device_powerOff");
    duk_push_c_function(ctx, device::CFUNC_batteryLevel, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__device_batteryLevel");
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
    duk_push_c_function(ctx, fs::CFUNC_pathFAT, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__fs_pathFAT");
    duk_push_c_function(ctx, fs::CFUNC_pathRomFS, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__fs_pathRomFS");
}

string JS_CommonNTR()
{
    string js = "";

    js += "game.flush=function(){__CFUNC__game_flush();};";
    js += "device.Platform='NTR';";
    js += "device.PlatformName='Nintendo DS';";
    js += "device.powerOff=function(){__CFUNC__device_powerOff();};";
    js += "device.batteryLevel=function(){return __CFUNC__device_batteryLevel();};";
    js += "device.language=function(){return __CFUNC__device_language();};";
    js += "device.screen.Width=256;";
    js += "device.screen.Height=192;";
    js += "input.None=0;";
	js += "input.A=1;";
	js += "input.B=2;";
	js += "input.Select=4;";
	js += "input.Start=8;";
	js += "input.Right=16;";
	js += "input.Left=32;";
	js += "input.Up=64;";
	js += "input.Down=128;";
	js += "input.R=256;";
	js += "input.L=512;";
	js += "input.X=1024;";
	js += "input.Y=2048;";
    js += "input.Touch=4096;";
    js += "input.Lid=8192;";
    js += "input.checkHeld=function(){return __CFUNC__input_checkHeld();};";
    js += "input.checkPressed=function(){return __CFUNC__input_checkPressed();};";
    js += "input.checkReleased=function(){return __CFUNC__input_checkReleased();};";
    js += "input.touchData=function(){var tch=[];tch.X=__CFUNC__PART__input_touchData_x();tch.Y=__CFUNC__PART__input_touchData_y();return tch;};";
    js += "fs.pathFAT=function(Path){return __CFUNC__fs_pathFAT(Path);};";
    js += "fs.pathRomFS=function(Path){return __CFUNC__fs_pathRomFS(Path);};";
    return js;
}
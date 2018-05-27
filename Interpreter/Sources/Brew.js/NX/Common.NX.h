#include "Includes.NX.h"

namespace game
{
    // <internally called from mainLoop>
    inline duk_ret_t INTERNAL_flush(duk_context *ctx)
    {
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
        gfx::__gfx_render();
        return 1;
    }
}

namespace device
{
    //device.language() -> string
    inline duk_ret_t CFUNC_language(duk_context *ctx)
    {
        Result res;
        res = setInitialize();
        if(R_SUCCEEDED(res))
        {
            u64 lang;
            res = setGetSystemLanguage(&lang);
            if(R_SUCCEEDED(res))
            {
                string slang = string((const char*)&lang);
                string rlang;
                if(slang == "ja") rlang = "Japanese";
                else if(slang == "en-US") rlang = "American English";
                else if(slang == "fr") rlang = "French";
                else if(slang == "de") rlang = "German";
                else if(slang == "it") rlang = "Italian";
                else if(slang == "es") rlang = "Spanish";
                else if(slang == "zh-CN") rlang = "Chinese";
                else if(slang == "ko") rlang = "Korean";
                else if(slang == "nl") rlang = "Dutch";
                else if(slang == "pt") rlang = "Portuguese";
                else if(slang == "ru") rlang = "Russian";
                else if(slang == "zh-TW") rlang = "Taiwanese";
                else if(slang == "en-GB") rlang = "British English";
                else if(slang == "fr-CA") rlang = "Canadian French";
                else if(slang == "es-419") rlang = "Latin American Spanish";
                else if(slang == "zh-Hans") rlang = "Simplified Chinese";
                else if(slang == "zh-Hant") rlang = "Traditional Chinese";
                else rlang = "Unknown";
                duk_push_string(ctx, rlang.c_str());
            }
        }
        return 1;
    }
}

namespace input
{
    // input.checkPressed() -> key (default player)  ---  input.checkPressed(Player) -> key
    inline duk_ret_t CFUNC_checkPressed(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int player = duk_to_number(ctx, 0);
            hidScanInput();
            int key = hidKeysDown((HidControllerID)player);
            duk_push_number(ctx, key);
        }
        else
        {
            hidScanInput();
            int key = hidKeysDown(CONTROLLER_P1_AUTO);
            duk_push_number(ctx, key);
        }
        return 1;
    }

    // input.checkReleased() -> key
    inline duk_ret_t CFUNC_checkReleased(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int player = duk_to_number(ctx, 0);
            hidScanInput();
            int key = hidKeysUp((HidControllerID)player);
            duk_push_number(ctx, key);
        }
        else
        {
            hidScanInput();
            int key = hidKeysUp(CONTROLLER_P1_AUTO);
            duk_push_number(ctx, key);
        }
        return 1;
    }

    inline duk_ret_t CFUNC_checkHeld(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int player = duk_to_number(ctx, 0);
            hidScanInput();
            int key = hidKeysHeld((HidControllerID)player);
            duk_push_number(ctx, key);
        }
        else
        {
            hidScanInput();
            int key = hidKeysHeld(CONTROLLER_P1_AUTO);
            duk_push_number(ctx, key);
        }
        return 1;
    }

    // [partial] input.touchData().X -> number
    inline duk_ret_t PART_touchData_x(duk_context *ctx)
    {
        hidScanInput();
        touchPosition tch;
        hidTouchRead(&tch, 0);
        int x = tch.px;
        duk_push_number(ctx, x);
        return 1;
    }

    // [partial] input.touchData().Y -> number
    inline duk_ret_t PART_touchData_y(duk_context *ctx)
    {
        hidScanInput();
        touchPosition tch;
        hidTouchRead(&tch, 0);
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

inline void CTX_CommonNX(duk_context *ctx)
{
    duk_push_c_function(ctx, game::INTERNAL_flush, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__INTERNAL__flush");
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
}

inline string JS_CommonNX()
{
    string js = "";
    js += "game.__loop__=true;";
    js += "game.exitLoop=function(){game.__loop__=false;};";
    js += "game.mainLoop=function(){while(game.__loop__){for(var i=0;i<arguments.length;i++){var arg=arguments[i];arg();}__CFUNC__INTERNAL__flush();}};";
    js += "device.Platform='NX';";
    js += "device.PlatformName='Nintendo Switch';";
    js += "device.screen.Width=1280;";
    js += "device.screen.Height=720;";
    js += "device.language=function(){return __CFUNC__device_language();};";
    js += "input.None=0;";
	js += "input.A=1;";
	js += "input.B=2;";
	js += "input.X=4;";
	js += "input.Y=8;";
	js += "input.LStick=16;";
	js += "input.RStick=32;";
	js += "input.L=64;";
	js += "input.R=128;";
	js += "input.ZL=256;";
	js += "input.ZR=512;";
	js += "input.Plus=1024;";
	js += "input.Minus=2048;";
	js += "input.DPadLeft=4096;";
	js += "input.DPadUp=8192;";
	js += "input.DPadRight=16384;";
	js += "input.DPadDown=32768;";
	js += "input.LStickLeft=65536;";
	js += "input.LStickUp=131072;";
	js += "input.LStickRight=262144;";
	js += "input.LStickDown=524288;";
    js += "input.RStickLeft=1048576;";
	js += "input.RStickUp=2097152;";
	js += "input.RStickRight=4194304;";
	js += "input.RStickDown=8388608;";
	js += "input.SL=16777216;";
	js += "input.SR=33554432;";
    js += "input.Touch=67108864;";
    js += "input.JoyConRight=1;";
    js += "input.JoyConDown=2;";
    js += "input.JoyConUp=4;";
    js += "input.JoyConLeft=8;";
    js += "input.Up=input.DPadUp|input.LStickUp|input.RStickUp;";
    js += "input.Down=input.DPadDown|input.LStickDown|input.RStickDown;";
    js += "input.Left=input.DPadLeft|input.LStickLeft|input.RStickLeft;";
    js += "input.Right=input.DPadRight|input.LStickRight|input.RStickRight;";
    js += "input.checkHeld=function(){return __CFUNC__input_checkHeld();};";
    js += "input.checkPressed=function(){return __CFUNC__input_checkPressed();};";
    js += "input.checkReleased=function(){return __CFUNC__input_checkReleased();};";
    js += "input.touchData=function(){var tch=[];tch.X=__CFUNC__PART__input_touchData_x();tch.Y=__CFUNC__PART__input_touchData_y();return tch;};";
    js += "fs.pathSDMC=function(Path){return __CFUNC__fs_pathSDMC(Path);};";
    js += "fs.pathRomFS=function(Path){return __CFUNC__fs_pathRomFS(Path);};";
    return js;
}
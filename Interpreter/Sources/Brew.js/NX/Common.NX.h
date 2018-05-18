#include "Includes.NX.h"
#include "gfx/bmp.h"
#include "gfx/draw.hpp"

namespace game
{
    // game.flush()
    inline duk_ret_t CFUNC_flush(duk_context *ctx)
    {
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
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
    // input.checkPressed() -> key (default, player 1)  ---  input.checkPressed(Player) -> key
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

namespace console
{
    // console.init()
    inline duk_ret_t CFUNC_init(duk_context *ctx)
    {
        consoleInit(NULL);
        return 1;
    }
}

namespace gfx
{
    struct Pixel
    {
        u8 r;
        u8 g;
        u8 b;
    };

    struct Sprite
    {
        vector<Pixel> pixdata;
        int width;
        int height;
        int x;
        int y;
        int id;
    };

    struct Text
    {
        int x;
        int y;
        string text;
        int id;
    };

    static vector<Sprite> sprdata;
    static vector<Text> txtdata;

    // gfx.loadBitmap(Path) -> number
    inline duk_ret_t CFUNC_loadBitmap(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            string path = string(duk_to_string(ctx, 0));
            ifstream ifs(path);
            if(ifs.good())
            {
                int cid = rand();
                vector<Pixel> data;
                Sprite meta;
                meta.id = cid;
                bitmap_image bitmap(path);
                int w = bitmap.width();
                int h = bitmap.height();
                meta.width = w;
                meta.height = h;
                meta.x = 0;
                meta.y = 0;
                for(int i = 0; i < w; i++)
                {
                    for(int j = 0; j < h; j++)
                    {
                        u8 r, g, b;
                        Pixel curp;
                        bitmap.get_pixel(i, j, r, g, b);
                        curp.r = r;
                        curp.g = g;
                        curp.b = b;
                        data.push_back(curp);
                    }
                }
                meta.pixdata = data;
                sprdata.push_back(meta);
                duk_push_number(ctx, cid);
            }
            ifs.close();
        }
        return 1;
    }

    // gfx.showBitmap(Bitmap)
    inline duk_ret_t CFUNC_showBitmap(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int id = duk_to_number(ctx, 0);
            for(int i = 0; i < sprdata.size(); i++)
            {
                Sprite scur = sprdata[i];
                if(scur.id == id)
                {
                    int iter = 0;
                    for(int a = 0; a < scur.width; a++)
                    {
                        for(int b = 0; b < scur.height; b++)
                        {
                            Pixel pix = scur.pixdata[iter];
                            DrawPixel(scur.x + a, scur.y + b, MakeColor(pix.r, pix.g, pix.b, 255));
                            iter++;
                        }
                    }
                }
            }
        }
        return 1;
    }

    // gfx.setBitmapX(Bitmap, X)
    inline duk_ret_t CFUNC_setBitmapX(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 2)
        {
            int id = duk_to_number(ctx, 0);
            int px = duk_to_number(ctx, 1);
            for(int i = 0; i < sprdata.size(); i++)
            {
                Sprite scur = sprdata[i];
                if(scur.id == id)
                {
                    sprdata[i].x = px;
                }
            }
        }
        return 1;
    }

    // gfx.setBitmapY(Bitmap, Y)
    inline duk_ret_t CFUNC_setBitmapY(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 2)
        {
            int id = duk_to_number(ctx, 0);
            int py = duk_to_number(ctx, 1);
            for(int i = 0; i < sprdata.size(); i++)
            {
                Sprite scur = sprdata[i];
                if(scur.id == id)
                {
                    sprdata[i].y = py;
                }
            }
        }
        return 1;
    }

    // gfx.setBitmapXY(Bitmap, X, Y)
    inline duk_ret_t CFUNC_setBitmapXY(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 3)
        {
            int id = duk_to_number(ctx, 0);
            int px = duk_to_number(ctx, 1);
            int py = duk_to_number(ctx, 2);
            for(int i = 0; i < sprdata.size(); i++)
            {
                Sprite scur = sprdata[i];
                if(scur.id == id)
                {
                    sprdata[i].x = px;
                    sprdata[i].y = py;
                }
            }
        }
        return 1;
    }
}

void CTX_CommonNX(duk_context *ctx)
{
    duk_push_c_function(ctx, game::CFUNC_flush, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__game_flush");
    duk_push_c_function(ctx, device::CFUNC_language, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__device_language");
    duk_push_c_function(ctx, input::CFUNC_checkPressed, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__input_checkPressed");
    duk_push_c_function(ctx, input::CFUNC_checkReleased, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__input_checkReleased");
    duk_push_c_function(ctx, input::CFUNC_checkHeld, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__input_checkHeld");
    duk_push_c_function(ctx, fs::CFUNC_pathSDMC, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__fs_pathSDMC");
    duk_push_c_function(ctx, fs::CFUNC_pathRomFS, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__fs_pathRomFS");
    duk_push_c_function(ctx, input::PART_touchData_x, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__input_touchData_x");
    duk_push_c_function(ctx, input::PART_touchData_y, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__input_touchData_y");
    duk_push_c_function(ctx, console::CFUNC_init, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__console_init");
    duk_push_c_function(ctx, gfx::CFUNC_loadBitmap, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_loadBitmap");
    duk_push_c_function(ctx, gfx::CFUNC_showBitmap, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_showBitmap");
    duk_push_c_function(ctx, gfx::CFUNC_setBitmapX, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setBitmapX");
    duk_push_c_function(ctx, gfx::CFUNC_setBitmapY, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setBitmapY");
    duk_push_c_function(ctx, gfx::CFUNC_setBitmapXY, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setBitmapXY");
}

string JS_CommonNX()
{
    string js = "";
    js += "game.flush=function(){__CFUNC__game_flush();};";
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
    js += "console.init=function(){__CFUNC__console_init();};";
    js += "gfx.loadBitmap=function(Path){return __CFUNC__gfx_loadBitmap(Path);};";
    js += "gfx.showBitmap=function(Bitmap){__CFUNC__gfx_showBitmap(Bitmap);};";
    js += "gfx.setBitmapX=function(Bitmap,X){__CFUNC__gfx_setBitmapX(Bitmap,X);};";
    js += "gfx.setBitmapY=function(Bitmap,Y){__CFUNC__gfx_setBitmapY(Bitmap,Y);};";
    js += "gfx.setBitmapXY=function(Bitmap,X,Y){__CFUNC__gfx_setBitmapXY(Bitmap,X,Y);};";
    return js;
}
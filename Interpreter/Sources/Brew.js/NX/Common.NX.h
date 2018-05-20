#include "Includes.NX.h"
#include "gfx/bmp.h"
#include "gfx/draw.hpp"
#include "gfx/nanojpeg.c"
#include "gfx/lodepng.h"

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
        u8 *rawdata;
        int type; // bmp = 0, jpg = 1, png = 2
        int width;
        int height;
        int x;
        int y;
        int id;
        int size;
    };

    static vector<Sprite> sprdata;

    // gfx.spriteFromBMP(Path) -> number
    inline duk_ret_t CFUNC_spriteFromBMP(duk_context *ctx)
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
                meta.type = 0;
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

    // gfx.spriteFromJPEG(Path) -> number
    inline duk_ret_t CFUNC_spriteFromJPEG(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            const char *path = duk_to_string(ctx, 0);
            FILE *fle = fopen(path, "rb");
            if(fle)
            {
                u8 *buf;
                int size;
                fseek(fle, 0, SEEK_END);
                size = (int)ftell(fle);
                buf = (u8*)malloc(size);
                fseek(fle, 0, SEEK_SET);
                size = (int)fread(buf, 1, size, fle);
                njInit();
                if(njDecode(buf, size) == NJ_OK)
                {
                    u8 *raw = njGetImage();
                    int w = njGetWidth();
                    int h = njGetHeight();
                    int cid = rand();
                    Sprite meta;
                    meta.id = cid;
                    meta.type = 1;
                    meta.x = 0;
                    meta.y = 0;
                    meta.width = w;
                    meta.height = h;
                    meta.rawdata = raw;
                    sprdata.push_back(meta);
                    duk_push_number(ctx, cid);
                }
                njDone();
            }
            fclose(fle);
        }
        return 1;
    }

    // gfx.spriteFromPNG(Path) -> number
    inline duk_ret_t CFUNC_spriteFromPNG(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            const char* path = duk_to_string(ctx, 0);
            unsigned res;
            u8 *raw;
            unsigned w;
            unsigned h;
            res = lodepng_decode32_file(&raw, &w, &h, path);
            if(!res)
            {
                int cid = rand();
                Sprite meta;
                meta.id = cid;
                meta.type = 2;
                meta.x = 0;
                meta.y = 0;
                meta.width = (int)w;
                meta.height = (int)h;
                meta.rawdata = raw;
                sprdata.push_back(meta);
                duk_push_number(ctx, cid);
            }
        }
        return 1;
    }

    // gfx.showSprite(Sprite)
    inline duk_ret_t CFUNC_showSprite(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int id = duk_to_number(ctx, 0);
            for(int i = 0; i < sprdata.size(); i++)
            {
                Sprite scur = sprdata[i];
                if(scur.id == id)
                {
                    if(scur.type == 0)
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
                    else if(scur.type == 1)
                    {
                        DrawImage(scur.x, scur.y, scur.width, scur.height, scur.rawdata, IMAGE_MODE_RGB24);
                    }
                    else if(scur.type == 2)
                    {
                        DrawImage(scur.x, scur.y, scur.width, scur.height, scur.rawdata, IMAGE_MODE_RGBA32);
                    }
                }
            }
        }
        return 1;
    }

    // gfx.setSpriteX(Sprite, X)
    inline duk_ret_t CFUNC_setSpriteX(duk_context *ctx)
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

    // gfx.setSpriteY(Sprite, Y)
    inline duk_ret_t CFUNC_setSpriteY(duk_context *ctx)
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

    // gfx.setSpriteXY(Sprite, X, Y)
    inline duk_ret_t CFUNC_setSpriteXY(duk_context *ctx)
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

    struct Text
    {
        int x;
        int y;
        const char *text;
        int id;
        color_t clr;
        const ffnt_header_t *font;
    };

    static vector<Text> txtdata;

    // gfx.createText(Text, Font, Size, X, Y, R, G, B) -> number
    inline duk_ret_t CFUNC_createText(duk_context *ctx)
    {
        if(duk_get_top(ctx) == 8)
        {
            const ffnt_header_t *hfont;
            const char *txt = duk_to_string(ctx, 0);
            int font = duk_to_number(ctx, 1);
            int size = duk_to_number(ctx, 2);
            int x = duk_to_number(ctx, 3);
            int y = duk_to_number(ctx, 4);
            int cr = duk_to_number(ctx, 5);
            int cg = duk_to_number(ctx, 6);
            int cb = duk_to_number(ctx, 7);
            if(font == 1)
            {
                if(size <= 20) hfont = InterUI_Medium_20;
                else if(size > 20 && size <= 28) hfont = InterUI_Medium_28;
                else if(size > 28 && size <= 30) hfont = InterUI_Medium_30;
                else hfont = InterUI_Medium_42;
            }
            else if(font == 2)
            {
                if(size <= 12) hfont = Tahoma_12;
                else hfont = Tahoma_24;
            }
            else
            {
                if(size <= 14) hfont = InterUI_Regular_14;
                else if(size > 14 && size <= 18) hfont = InterUI_Regular_18;
                else if(size > 18 && size <= 20) hfont = InterUI_Regular_20;
                else hfont = InterUI_Regular_24;
            }
            Text ttxt;
            int cid = rand();
            ttxt.id = cid;
            ttxt.clr = MakeColor(cr, cg, cb, 255);
            ttxt.x = x;
            ttxt.y = y;
            ttxt.font = hfont;
            ttxt.text = txt;
            txtdata.push_back(ttxt);
            duk_push_number(ctx, cid);
        }
        return 1;
    }

    // gfx.showText(Text)
    inline duk_ret_t CFUNC_showText(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int id = duk_to_number(ctx, 0);
            for(int i = 0; i < txtdata.size(); i++)
            {
                Text tcur = txtdata[i];
                if(tcur.id == id)
                {
                    DrawText(tcur.font, tcur.x, tcur.y, tcur.clr, tcur.text);
                }
            }
        }
        return 1;
    }

    // gfx.setTextX(Text, X)
    inline duk_ret_t CFUNC_setTextX(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 2)
        {
            int id = duk_to_number(ctx, 0);
            int px = duk_to_number(ctx, 1);
            for(int i = 0; i < txtdata.size(); i++)
            {
                Text tcur = txtdata[i];
                if(tcur.id == id)
                {
                    txtdata[i].x = px;
                }
            }
        }
        return 1;
    }

    // gfx.setTextY(Text, Y)
    inline duk_ret_t CFUNC_setTextY(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 2)
        {
            int id = duk_to_number(ctx, 0);
            int py = duk_to_number(ctx, 1);
            for(int i = 0; i < txtdata.size(); i++)
            {
                Text tcur = txtdata[i];
                if(tcur.id == id)
                {
                    txtdata[i].y = py;
                }
            }
        }
        return 1;
    }

    // gfx.setTextXY(Text, X)
    inline duk_ret_t CFUNC_setTextXY(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 3)
        {
            int id = duk_to_number(ctx, 0);
            int px = duk_to_number(ctx, 1);
            int py = duk_to_number(ctx, 2);
            for(int i = 0; i < txtdata.size(); i++)
            {
                Text tcur = txtdata[i];
                if(tcur.id == id)
                {
                    txtdata[i].x = px;
                    txtdata[i].y = py;
                }
            }
        }
        return 1;
    }

    // gfx.setTextColor(Text, R, G, B)
    inline duk_ret_t CFUNC_setTextColor(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 4)
        {
            int id = duk_to_number(ctx, 0);
            int cr = duk_to_number(ctx, 1);
            int cg = duk_to_number(ctx, 2);
            int cb = duk_to_number(ctx, 3);
            for(int i = 0; i < txtdata.size(); i++)
            {
                Text tcur = txtdata[i];
                if(tcur.id == id)
                {
                    txtdata[i].clr = MakeColor(cr, cg, cb, 255);
                }
            }
        }
        return 1;
    }

    // gfx.setTextFont(Text, Font, Size)
    inline duk_ret_t CFUNC_setTextFont(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 3)
        {
            int id = duk_to_number(ctx, 0);
            int font = duk_to_number(ctx, 1);
            int size = duk_to_number(ctx, 2);
            for(int i = 0; i < txtdata.size(); i++)
            {
                Text tcur = txtdata[i];
                if(tcur.id == id)
                {
                    const ffnt_header_t *hfont;
                    if(font == 1)
                    {
                        if(size <= 20) hfont = InterUI_Medium_20;
                        else if(size > 20 && size <= 28) hfont = InterUI_Medium_28;
                        else if(size > 28 && size <= 30) hfont = InterUI_Medium_30;
                        else hfont = InterUI_Medium_42;
                    }
                    else if(font == 2)
                    {
                        if(size <= 12) hfont = Tahoma_12;
                        else hfont = Tahoma_24;
                    }
                    else
                    {
                        if(size <= 14) hfont = InterUI_Regular_14;
                        else if(size > 14 && size <= 18) hfont = InterUI_Regular_18;
                        else if(size > 18 && size <= 20) hfont = InterUI_Regular_20;
                        else hfont = InterUI_Regular_24;
                    }
                    txtdata[i].font = hfont;
                }
            }
        }
        return 1;
    }

    // gfx.setTextContent(Text, Content)
    inline duk_ret_t CFUNC_setTextContent(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 2)
        {
            int id = duk_to_number(ctx, 0);
            const char *txt = duk_to_string(ctx, 1);
            for(int i = 0; i < txtdata.size(); i++)
            {
                Text tcur = txtdata[i];
                if(tcur.id == id)
                {
                    txtdata[i].text = txt;
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
    duk_push_c_function(ctx, gfx::CFUNC_spriteFromBMP, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_spriteFromBMP");
    duk_push_c_function(ctx, gfx::CFUNC_spriteFromJPEG, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_spriteFromJPEG");
    duk_push_c_function(ctx, gfx::CFUNC_spriteFromPNG, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_spriteFromPNG");
    duk_push_c_function(ctx, gfx::CFUNC_showSprite, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_showSprite");
    duk_push_c_function(ctx, gfx::CFUNC_setSpriteX, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setSpriteX");
    duk_push_c_function(ctx, gfx::CFUNC_setSpriteY, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setSpriteY");
    duk_push_c_function(ctx, gfx::CFUNC_setSpriteXY, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setSpriteXY");
    duk_push_c_function(ctx, gfx::CFUNC_createText, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_createText");
    duk_push_c_function(ctx, gfx::CFUNC_showText, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_showText");
    duk_push_c_function(ctx, gfx::CFUNC_setTextX, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setTextX");
    duk_push_c_function(ctx, gfx::CFUNC_setTextY, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setTextY");
    duk_push_c_function(ctx, gfx::CFUNC_setTextXY, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setTextXY");
    duk_push_c_function(ctx, gfx::CFUNC_setTextColor, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setTextColor");
    duk_push_c_function(ctx, gfx::CFUNC_setTextFont, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setTextFont");
    duk_push_c_function(ctx, gfx::CFUNC_setTextContent, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setTextContent");
}

string JS_CommonNX()
{
    string js = "";
    js += "gfx.font=[];";
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
    js += "gfx.font.InterUIRegular=0;";
    js += "gfx.font.InterUIMedium=1;";
    js += "gfx.font.Tahoma=2;";
    js += "gfx.spriteFromBMP=function(Path){return __CFUNC__gfx_spriteFromBMP(Path);};";
    js += "gfx.spriteFromJPEG=function(Path){return __CFUNC__gfx_spriteFromJPEG(Path);};";
    js += "gfx.spriteFromPNG=function(Path){return __CFUNC__gfx_spriteFromPNG(Path);};";
    js += "gfx.showSprite=function(Sprite){__CFUNC__gfx_showSprite(Sprite);};";
    js += "gfx.setSpriteX=function(Sprite,X){__CFUNC__gfx_setSpriteX(Sprite,X);};";
    js += "gfx.setSpriteY=function(Sprite,Y){__CFUNC__gfx_setSpriteY(Sprite,Y);};";
    js += "gfx.setSpriteXY=function(Sprite,X,Y){__CFUNC__gfx_setSpriteXY(Sprite,X,Y);};";
    js += "gfx.createText=function(Text,Font,Size,X,Y,R,G,B){return __CFUNC__gfx_createText(Text,Font,Size,X,Y,R,G,B);};";
    js += "gfx.showText=function(Text){__CFUNC__gfx_showText(Text);};";
    js += "gfx.setTextX=function(Text,X){__CFUNC__gfx_setTextX(Text,X);};";
    js += "gfx.setTextY=function(Text,Y){__CFUNC__gfx_setTextY(Text,Y);};";
    js += "gfx.setTextXY=function(Text,X,Y){__CFUNC__gfx_setTextXY(Text,X,Y);};";
    js += "gfx.setTextColor=function(Text,R,G,B){__CFUNC__gfx_setTextColor(Text,R,G,B);};";
    js += "gfx.setTextFont=function(Text,Font,Size){__CFUNC__gfx_setTextFont(Text,Font,Size);};";
    js += "gfx.setTextContent=function(Text,Content){__CFUNC__gfx_setTextContent(Text,Content);};";
    return js;
}
#include "Gfx.hpp"

Brew::API::Module gfx("gfx");
bool ninit = false;

void npix(u8 scr, u32 x, u32 y, u8 r, u8 g, u8 b)
{
    if(x >= 256 || y >= 192) return;
    u32 color = ARGB16(31, r / 8, g / 8, b / 8);
    if(scr == 0) BG_GFX[(256 * y) + x] = color;
    else BG_GFX_SUB[(256 * y) + x] = color;
}

void nrect(u8 scr, u32 x, u32 y, u32 w, u32 h, u8 r, u8 g, u8 b)
{
    for(u32 i = 0; i < w; i++) for(u32 j = 0; j < h; j++) npix(scr, x + i, y + j, r, g, b);
}

Brew::API::Function Brew::BuiltIn::Gfx::init(Brew::API::NativeContext Context)
{
    if(!ninit)
    {
        REG_DISPCNT = MODE_3_2D | DISPLAY_BG3_ACTIVE;
        REG_DISPCNT_SUB = MODE_3_2D | DISPLAY_BG3_ACTIVE;
        REG_BG3CNT = BG_BMP16_256x256;
        REG_BG3CNT_SUB = BG_BMP16_256x256;
        REG_BG3PA = 1 << 8;
        REG_BG3PD = 1 << 8;
        REG_BG3PA_SUB = 1 << 8;
        REG_BG3PD_SUB = 1 << 8;
        nrect(0, 0, 0, 256, 192, 0, 0, 0);
        nrect(1, 0, 0, 256, 192, 0, 0, 0);
        ninit = true;
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Gfx::drawPixel(Brew::API::NativeContext Context)
{
    if(ninit)
    {
        Brew::API::FunctionHandler handler(Context);
        if(handler.checkArgc(6))
        {
            u8 scr = handler.getUInt(0);
            u32 x = handler.getUInt(1);
            u32 y = handler.getUInt(2);
            u8 r = handler.getUInt(3);
            u8 g = handler.getUInt(4);
            u8 b = handler.getUInt(5);
            npix(scr, x, y, r, g, b);
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Gfx::drawRectangle(Brew::API::NativeContext Context)
{
    if(ninit)
    {
        Brew::API::FunctionHandler handler(Context);
        if(handler.checkArgc(8))
        {
            u8 scr = handler.getUInt(0);
            u32 x = handler.getUInt(1);
            u32 y = handler.getUInt(2);
            u32 w = handler.getUInt(3);
            u32 h = handler.getUInt(4);
            u8 r = handler.getUInt(5);
            u8 g = handler.getUInt(6);
            u8 b = handler.getUInt(7);
            nrect(scr, x, y, w, h, r, g, b);
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Gfx::drawScreen(Brew::API::NativeContext Context)
{
    if(ninit)
    {
        Brew::API::FunctionHandler handler(Context);
        if(handler.checkArgc(4))
        {
            u8 scr = handler.getUInt(0);
            u8 r = handler.getUInt(1);
            u8 g = handler.getUInt(2);
            u8 b = handler.getUInt(3);
            nrect(scr, 0, 0, 256, 192, r, g, b);
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Gfx::drawImage(Brew::API::NativeContext Context)
{
    if(ninit)
    {
        Brew::API::FunctionHandler handler(Context);
        if(handler.checkArgc(4))
        {
            u8 scr = handler.getUInt(0);
            string image = handler.getString(1);
            u32 x = handler.getUInt(2);
            u32 y = handler.getUInt(3);
            string ext = image.substr(image.find_last_of(".") + 1);
            transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if(ext == "bmp")
            {
                bitmap_image bmp(image);
                u32 w = bmp.width();
                u32 h = bmp.height();
                for(u32 i = 0; i < w; i++)
                {
                    for(u32 j = 0; j < h; j++)
                    {
                        u8 r;
                        u8 g;
                        u8 b;
                        bmp.get_pixel(i, j, r, g, b);
                        npix(scr, (x + i), (y + j), r, g, b);
                    }
                }
            }
            else if(ext == "png")
            {
                upng_t *png = upng_new_from_file(image.c_str());
                if(png != NULL)
                {
                    upng_decode(png);
                    if(upng_get_error(png) == UPNG_EOK)
                    {
                        u32 w = upng_get_width(png);
                        u32 h = upng_get_height(png);
                        u8 *buf = (u8*)upng_get_buffer(png);
                        for(u32 i = 0; i < w; i++)
                        {
                            for(u32 j = 0; j < h; j++)
                            {
                                u32 pos = ((j * w) + i) * 4;
                                npix(scr, x + i, y + j, buf[pos], buf[pos + 1], buf[pos + 2]);
                            }
                        }
                    }
                    upng_free(png);
                }
            }
            else if(ext == "jpg" || ext == "jpeg")
            {
                FILE *fjpg = fopen(image.c_str(), "rb");
                if(fjpg)
                {
                    fseek(fjpg, 0, SEEK_END);
                    int size = (int)ftell(fjpg);
                    u8 *buf = (u8*)malloc(size);
                    fseek(fjpg, 0, SEEK_SET);
                    size = (int)fread(buf, 1, size, fjpg);
                    fclose(fjpg);
                    njInit();
                    if(njDecode(buf, size) == NJ_OK)
                    {
                        u8 *raw = njGetImage();
                        u32 w = njGetWidth();
                        u32 h = njGetHeight();
                        for(u32 i = 0; i < w; i++)
                        {
                            for(u32 j = 0; j < h; j++)
                            {
                                u32 pos = ((j * w) + i) * 3;
                                npix(scr, x + i, y + j, raw[pos], raw[pos + 1], raw[pos + 2]);
                            }
                        }
                    }
                    njDone();
                }
                else fclose(fjpg);
            }
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Gfx::refresh(Brew::API::NativeContext Context)
{
    if(ninit) swiWaitForVBlank();
    return Brew::API::Return::Void;
}

Brew::API::Module Brew::BuiltIn::Gfx::initModule()
{
    gfx.pushUInt("Top", 0);
    gfx.pushUInt("Bottom", 1);
    gfx.pushFunction("init", init);
    gfx.pushFunction("drawPixel", drawPixel);
    gfx.pushFunction("drawRectangle", drawRectangle);
    gfx.pushFunction("drawScreen", drawScreen);
    gfx.pushFunction("drawImage", drawImage);
    gfx.pushFunction("refresh", refresh);
    return gfx;
}
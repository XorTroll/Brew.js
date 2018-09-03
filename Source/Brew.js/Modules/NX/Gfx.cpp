#include "Gfx.hpp"

Brew::API::Module gfx("gfx");
bool ginit = false;
u8 *framebuf;
u32 fwidth;

u8 BlendColor(u32 src, u32 dst, u8 alpha)
{
    u8 one_minus_alpha = (u8)255 - alpha;
    return (dst*alpha + src*one_minus_alpha)/(u8)255;
}

void pix(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a)
{
    if(x >= 1280 || y >= 720) return;
    u32 off = (y * fwidth + x) * 4;
    framebuf[off] = BlendColor(framebuf[off], r, a); off++;
    framebuf[off] = BlendColor(framebuf[off], g, a); off++;
    framebuf[off] = BlendColor(framebuf[off], b, a); off++;
    framebuf[off] = 0xff;
}

void rect(u32 x, u32 y, u32 w, u32 h, u8 r, u8 g, u8 b, u8 a)
{
    for(u32 j = y; j < y + h; j++) for(u32 i = x; i < x + w; i++) pix(i, j, r, g, b, a);
}

Brew::API::Function Brew::BuiltIn::Gfx::init(Brew::API::NativeContext Context)
{
    if(!ginit)
    {
        gfxInitDefault();
        framebuf = gfxGetFramebuffer(&fwidth, NULL);
        ginit = true;
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Gfx::drawPixel(Brew::API::NativeContext Context)
{
    if(ginit)
    {
        Brew::API::FunctionHandler handler(Context);
        if(handler.checkArgc(6))
        {
            u32 x = handler.getUInt(0);
            u32 y = handler.getUInt(1);
            u8 r = handler.getUInt(2);
            u8 g = handler.getUInt(3);
            u8 b = handler.getUInt(4);
            u8 a = handler.getUInt(5);
            pix(x, y, r, g, b, a);
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Gfx::drawRectangle(Brew::API::NativeContext Context)
{
    if(ginit)
    {
        Brew::API::FunctionHandler handler(Context);
        if(handler.checkArgc(8))
        {
            u32 x = handler.getUInt(0);
            u32 y = handler.getUInt(1);
            u32 w = handler.getUInt(2);
            u32 h = handler.getUInt(3);
            u8 r = handler.getUInt(4);
            u8 g = handler.getUInt(5);
            u8 b = handler.getUInt(6);
            u8 a = handler.getUInt(7);
            rect(x, y, w, h, r, g, b, a);
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Gfx::drawScreen(Brew::API::NativeContext Context)
{
    if(ginit)
    {
        Brew::API::FunctionHandler handler(Context);
        if(handler.checkArgc(4))
        {
            u8 r = handler.getUInt(0);
            u8 g = handler.getUInt(1);
            u8 b = handler.getUInt(2);
            u8 a = handler.getUInt(3);
            rect(0, 0, 1280, 720, r, g, b, a);
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Gfx::drawImage(Brew::API::NativeContext Context)
{
    if(ginit)
    {
        Brew::API::FunctionHandler handler(Context);
        if(handler.checkArgc(3))
        {
            string image = handler.getString(0);
            u32 x = handler.getUInt(1);
            u32 y = handler.getUInt(2);
            string ext = image.substr(image.find_last_of(".") + 1);
            if(ext == "bmp")
            {
                bitmap_image bmp(image);
                u32 h = bmp.height();
                u32 w = bmp.width();
                for(u32 i = 0; i < w; i++)
                {
                    for(u32 j = 0; j < h; j++)
                    {
                        u8 r = 0;
                        u8 g = 0;
                        u8 b = 0;
                        bmp.get_pixel(i, j, r, g, b);
                        pix((x + i), (y + j), r, g, b, 255);
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
                                pix(x + i, y + j, buf[pos], buf[pos + 1], buf[pos + 2], buf[pos + 3]);
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
                                pix(x + i, y + j, raw[pos], raw[pos + 1], raw[pos + 2], 255);
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
    if(ginit)
    {
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Gfx::exit(Brew::API::NativeContext Context)
{
    if(ginit)
    {
        gfxExit();
        free(framebuf);
        fwidth = 0;
        ginit = false;
    }
    return Brew::API::Return::Void;
}

Brew::API::Module Brew::BuiltIn::Gfx::initModule()
{
    gfx.pushFunction("init", init);
    gfx.pushFunction("drawPixel", drawPixel);
    gfx.pushFunction("drawRectangle", drawRectangle);
    gfx.pushFunction("drawScreen", drawScreen);
    gfx.pushFunction("drawImage", drawImage);
    gfx.pushFunction("refresh", refresh);
    gfx.pushFunction("exit", Brew::BuiltIn::Gfx::exit);
    return gfx;
}
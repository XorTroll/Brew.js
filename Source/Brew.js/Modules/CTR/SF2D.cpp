#include "SF2D.hpp"

Brew::API::Module sf2d("sf2d");
bool sfinit = false;

Brew::API::Function Brew::BuiltIn::SF2D::init(Brew::API::NativeContext Context)
{
    if(!sfinit)
    {
        sf2d_init();
        sfinit = true;
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::SF2D::setClearColor(Brew::API::NativeContext Context)
{
    if(sfinit)
    {
        Brew::API::FunctionHandler handler(Context);
        if(handler.checkArgc(4))
        {
            u8 r = handler.getUInt(0);
            u8 g = handler.getUInt(1);
            u8 b = handler.getUInt(2);
            u8 a = handler.getUInt(3);
            sf2d_set_clear_color(RGBA8(r, g, b, a));
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::SF2D::startFrame(Brew::API::NativeContext Context)
{
    if(sfinit)
    {
        Brew::API::FunctionHandler handler(Context);
        if(handler.checkArgc(1))
        {
            u8 scr = handler.getUInt(0);
            if(scr == 0) sf2d_start_frame(GFX_TOP, GFX_LEFT);
            else sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::SF2D::endFrame(Brew::API::NativeContext Context)
{
    if(sfinit) sf2d_end_frame();
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::SF2D::drawFillCircle(Brew::API::NativeContext Context)
{
    if(sfinit)
    {
        Brew::API::FunctionHandler handler(Context);
        if(handler.checkArgc(7))
        {
            u32 x = handler.getUInt(0);
            u32 y = handler.getUInt(1);
            u32 rad = handler.getUInt(2);
            u8 r = handler.getUInt(3);
            u8 g = handler.getUInt(4);
            u8 b = handler.getUInt(5);
            u8 a = handler.getUInt(6);
            sf2d_draw_fill_circle(x, y, rad, RGBA8(r, g, b, a));
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::SF2D::drawRectangle(Brew::API::NativeContext Context)
{
    if(sfinit)
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
            sf2d_draw_rectangle(x, y, w, h, RGBA8(r, g, b, a));
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::SF2D::refresh(Brew::API::NativeContext Context)
{
    if(sfinit) sf2d_swapbuffers();
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::SF2D::exit(Brew::API::NativeContext Context)
{
    if(sfinit) sf2d_fini();
    return Brew::API::Return::Void;
}

Brew::API::Module Brew::BuiltIn::SF2D::initModule()
{
    sf2d.pushUInt("Top", 0);
    sf2d.pushUInt("Bottom", 1);
    sf2d.pushFunction("init", init);
    sf2d.pushFunction("setClearColor", setClearColor);
    sf2d.pushFunction("startFrame", startFrame);
    sf2d.pushFunction("endFrame", endFrame);
    sf2d.pushFunction("drawFillCircle", drawFillCircle);
    sf2d.pushFunction("drawRectangle", drawRectangle);
    sf2d.pushFunction("refresh", refresh);
    sf2d.pushFunction("exit", exit);
    return sf2d;
}
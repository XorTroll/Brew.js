#include <bjs/Modules/CTR/SF2D.hpp>

namespace bjs::modules::SF2D
{
    bool sfinit = false;

    js::Function init(js::NativeContext Context)
    {
        if(!sfinit)
        {
            sf2d_init();
            sfinit = true;
        }
        return js::Return::Void;
    }

    js::Function setClearColor(js::NativeContext Context)
    {
        if(sfinit)
        {
            js::FunctionHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(4))
            {
                u8 r = handler.GetUInt(0);
                u8 g = handler.GetUInt(1);
                u8 b = handler.GetUInt(2);
                u8 a = handler.GetUInt(3);
                sf2d_set_clear_color(RGBA8(r, g, b, a));
            }
        }
        return js::Return::Void;
    }

    js::Function startFrame(js::NativeContext Context)
    {
        if(sfinit)
        {
            js::FunctionHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(1))
            {
                u8 scr = handler.GetUInt(0);
                if(scr == 0) sf2d_start_frame(GFX_TOP, GFX_LEFT);
                else sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            }
        }
        return js::Return::Void;
    }

    js::Function endFrame(js::NativeContext Context)
    {
        if(sfinit) sf2d_end_frame();
        return js::Return::Void;
    }

    js::Function drawFillCircle(js::NativeContext Context)
    {
        if(sfinit)
        {
            js::FunctionHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(7))
            {
                u32 x = handler.GetUInt(0);
                u32 y = handler.GetUInt(1);
                u32 rad = handler.GetUInt(2);
                u8 r = handler.GetUInt(3);
                u8 g = handler.GetUInt(4);
                u8 b = handler.GetUInt(5);
                u8 a = handler.GetUInt(6);
                sf2d_draw_fill_circle(x, y, rad, RGBA8(r, g, b, a));
            }
        }
        return js::Return::Void;
    }

    js::Function drawRectangle(js::NativeContext Context)
    {
        if(sfinit)
        {
            js::FunctionHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(8))
            {
                u32 x = handler.GetUInt(0);
                u32 y = handler.GetUInt(1);
                u32 w = handler.GetUInt(2);
                u32 h = handler.GetUInt(3);
                u8 r = handler.GetUInt(4);
                u8 g = handler.GetUInt(5);
                u8 b = handler.GetUInt(6);
                u8 a = handler.GetUInt(7);
                sf2d_draw_rectangle(x, y, w, h, RGBA8(r, g, b, a));
            }
        }
        return js::Return::Void;
    }

    js::Function refresh(js::NativeContext Context)
    {
        if(sfinit) sf2d_swapbuffers();
        return js::Return::Void;
    }

    js::Function exit(js::NativeContext Context)
    {
        if(sfinit) sf2d_fini();
        return js::Return::Void;
    }

    js::Module Initialize()
    {
        js::Module sf2d("sf2d");
        sf2d.PushUInt("Top", 0);
        sf2d.PushUInt("Bottom", 1);
        sf2d.PushFunction("init", init);
        sf2d.PushFunction("setClearColor", setClearColor);
        sf2d.PushFunction("startFrame", startFrame);
        sf2d.PushFunction("endFrame", endFrame);
        sf2d.PushFunction("drawFillCircle", drawFillCircle);
        sf2d.PushFunction("drawRectangle", drawRectangle);
        sf2d.PushFunction("refresh", refresh);
        sf2d.PushFunction("exit", exit);
        return sf2d;
    }
}
#include <bjs/platform/libctru/sf2d.hpp>

namespace bjs::libctru::sf2d
{
    js::Module CreateModule()
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
#include <bjs/platform/libctru/input/Keys.hpp>

namespace bjs::libctru::input
{
    js::Function getPressed(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        hidScanInput();
        u64 inp = hidKeysDown();
        handler.ReturnUInt(inp);
        return js::Return::Variable;
    }

    js::Function getHeld(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        u64 inp = hidKeysHeld();
        handler.ReturnUInt(inp);
        return js::Return::Variable;
    }

    js::Function getReleased(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        u64 inp = hidKeysUp();
        handler.ReturnUInt(inp);
        return js::Return::Variable;
    }

    js::Function getCPadPosition(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        js::Object obj(Context);
        handler.ReturnObject(obj);
        circlePosition tch;
        hidCircleRead(&tch);
        obj.AddUInt("X", tch.dx);
        obj.AddUInt("Y", tch.dy);
        return js::Return::Variable;
    }
}
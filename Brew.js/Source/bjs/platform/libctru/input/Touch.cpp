#include <bjs/platform/libctru/input/Touch.hpp>

namespace bjs::libctru::input
{
    js::Function getTouch(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        hidScanInput();
        u64 inp = hidKeysHeld();
        js::Object obj(Context);
        handler.ReturnObject(obj);
        if(inp & KEY_TOUCH)
        {
            touchPosition tch;
            hidTouchRead(&tch);
            obj.AddUInt("X", tch.px);
            obj.AddUInt("Y", tch.py);
        }
        else
        {
            obj.AddUInt("X", 0);
            obj.AddUInt("Y", 0);
        }
        return js::Return::Variable;
    }
}
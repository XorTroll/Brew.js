#include <bjs/platform/libnx/input/Touch.hpp>

namespace bjs::libnx::input
{
    js::Function getTouch(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        u32 tchn = 0;
        if(handler.CheckArgcEqualOrBigger(1)) tchn = handler.GetUInt(0);
        hidScanInput();
        u64 inp = hidKeysHeld(CONTROLLER_P1_AUTO);
        js::Object obj(Context);
        handler.ReturnObject(obj);
        if(inp & KEY_TOUCH)
        {
            touchPosition tch;
            hidTouchRead(&tch, tchn);
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

    js::Function getTouchNumber(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        hidScanInput();
        u64 inp = hidKeysHeld(CONTROLLER_P1_AUTO);
        if(inp & KEY_TOUCH)
        {
            u32 tchc = hidTouchCount();
            handler.ReturnUInt(tchc);
        }
        else handler.ReturnUInt(0);
        return js::Return::Variable;
    }
}
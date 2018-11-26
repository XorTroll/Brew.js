#include <bjs/platform/libnx/input/Keys.hpp>

namespace bjs::libnx::input
{
    js::Function getPressed(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        u32 player = 10;
        if(handler.CheckArgcEqualOrBigger(1)) player = handler.GetUInt(0);
        hidScanInput();
        u64 inp = hidKeysDown((HidControllerID)player);
        handler.ReturnUInt(inp);
        return js::Return::Variable;
    }

    js::Function getHeld(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        u32 player = 10;
        if(handler.CheckArgcEqualOrBigger(1)) player = handler.GetUInt(0);
        hidScanInput();
        u64 inp = hidKeysHeld((HidControllerID)player);
        handler.ReturnUInt(inp);
        return js::Return::Variable;
    }

    js::Function getReleased(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        u32 player = 10;
        if(handler.CheckArgcEqualOrBigger(1)) player = handler.GetUInt(0);
        hidScanInput();
        u64 inp = hidKeysUp((HidControllerID)player);
        handler.ReturnUInt(inp);
        return js::Return::Variable;
    }
}
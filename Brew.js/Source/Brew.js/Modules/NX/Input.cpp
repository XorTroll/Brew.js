#include <Brew.js/Modules/NX/Input.hpp>

namespace Brew::BuiltIn::Input
{
    API::Function getPressed(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        u32 player = 10;
        if(handler.CheckArgcEqualOrBigger(1)) player = handler.GetUInt(0);
        hidScanInput();
        u64 inp = hidKeysDown((HidControllerID)player);
        handler.ReturnUInt(inp);
        return API::Return::Variable;
    }

    API::Function getHeld(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        u32 player = 10;
        if(handler.CheckArgcEqualOrBigger(1)) player = handler.GetUInt(0);
        hidScanInput();
        u64 inp = hidKeysHeld((HidControllerID)player);
        handler.ReturnUInt(inp);
        return API::Return::Variable;
    }

    API::Function getReleased(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        u32 player = 10;
        if(handler.CheckArgcEqualOrBigger(1)) player = handler.GetUInt(0);
        hidScanInput();
        u64 inp = hidKeysUp((HidControllerID)player);
        handler.ReturnUInt(inp);
        return API::Return::Variable;
    }

    API::Function getTouch(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        u32 tchn = 0;
        if(handler.CheckArgcEqualOrBigger(1)) tchn = handler.GetUInt(0);
        hidScanInput();
        u64 inp = hidKeysHeld(CONTROLLER_P1_AUTO);
        API::Object obj(Context);
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
        return API::Return::Variable;
    }

    API::Function getTouchNumber(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        hidScanInput();
        u64 inp = hidKeysHeld(CONTROLLER_P1_AUTO);
        if(inp & KEY_TOUCH)
        {
            u32 tchc = hidTouchCount();
            handler.ReturnUInt(tchc);
        }
        else handler.ReturnUInt(0);
        return API::Return::Variable;
    }

    API::Module Initialize()
    {
        API::Module input("input");
        input.PushUInt("A", KEY_A);
        input.PushUInt("B", KEY_B);
        input.PushUInt("X", KEY_X);
        input.PushUInt("Y", KEY_Y);
        input.PushUInt("LStick", KEY_LSTICK);
        input.PushUInt("RStick", KEY_RSTICK);
        input.PushUInt("L", KEY_L);
        input.PushUInt("R", KEY_R);
        input.PushUInt("ZL", KEY_ZL);
        input.PushUInt("ZR", KEY_ZR);
        input.PushUInt("Plus", KEY_PLUS);
        input.PushUInt("Minus", KEY_MINUS);
        input.PushUInt("DPadLeft", KEY_DLEFT);
        input.PushUInt("DPadUp", KEY_DUP);
        input.PushUInt("DPadRight", KEY_DRIGHT);
        input.PushUInt("DPadDown", KEY_DDOWN);
        input.PushUInt("LStickLeft", KEY_LSTICK_LEFT);
        input.PushUInt("LStickUp", KEY_LSTICK_UP);
        input.PushUInt("LStickRight", KEY_LSTICK_RIGHT);
        input.PushUInt("LStickDown", KEY_LSTICK_DOWN);
        input.PushUInt("RStickLeft", KEY_RSTICK_LEFT);
        input.PushUInt("RStickUp", KEY_RSTICK_UP);
        input.PushUInt("RStickRight", KEY_RSTICK_RIGHT);
        input.PushUInt("RStickDown", KEY_RSTICK_DOWN);
        input.PushUInt("SL", KEY_SL);
        input.PushUInt("SR", KEY_SR);
        input.PushUInt("Touch", KEY_TOUCH);
        input.PushUInt("Up", KEY_UP);
        input.PushUInt("Down", KEY_DOWN);
        input.PushUInt("Left", KEY_LEFT);
        input.PushUInt("Right", KEY_RIGHT);
        input.PushFunction("getPressed", getPressed);
        input.PushFunction("getHeld", getHeld);
        input.PushFunction("getReleased", getReleased);
        input.PushFunction("getTouch", getTouch);
        input.PushFunction("getTouchNumber", getTouchNumber);
        return input;
    }
}
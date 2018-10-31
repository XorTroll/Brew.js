#include <bjs/Modules/NTR/Input.hpp>

namespace bjs::modules::Input
{
    js::Function getPressed(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        scanKeys();
        u64 inp = keysDown();
        handler.ReturnUInt(inp);
        return js::Return::Variable;
    }

    js::Function getHeld(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        scanKeys();
        u64 inp = keysCurrent();
        handler.ReturnUInt(inp);
        return js::Return::Variable;
    }

    js::Function getReleased(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        scanKeys();
        u64 inp = keysUp();
        handler.ReturnUInt(inp);
        return js::Return::Variable;
    }

    js::Function getTouch(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        scanKeys();
        u64 inp = keysCurrent();
        js::Object obj(Context);
        handler.ReturnObject(obj);
        if(inp & KEY_TOUCH)
        {
            touchPosition tch;
            touchRead(&tch);
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

    js::Module Initialize()
    {
        js::Module input("input");
        input.PushUInt("A", KEY_A);
        input.PushUInt("B", KEY_B);
        input.PushUInt("Select", KEY_SELECT);
        input.PushUInt("Start", KEY_START);
        input.PushUInt("Right", KEY_RIGHT);
        input.PushUInt("Left", KEY_LEFT);
        input.PushUInt("Up", KEY_UP);
        input.PushUInt("Down", KEY_DOWN);
        input.PushUInt("R", KEY_R);
        input.PushUInt("L", KEY_L);
        input.PushUInt("X", KEY_X);
        input.PushUInt("Y", KEY_X);
        input.PushUInt("Touch", KEY_TOUCH);
        input.PushUInt("Lid", KEY_LID);
        input.PushFunction("getPressed", getPressed);
        input.PushFunction("getHeld", getHeld);
        input.PushFunction("getReleased", getReleased);
        input.PushFunction("getTouch", getTouch);
        return input;
    }
}
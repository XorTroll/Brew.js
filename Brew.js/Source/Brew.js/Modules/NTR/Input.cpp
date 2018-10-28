#include <Brew.js/Modules/NTR/Input.hpp>

namespace Brew::BuiltIn::Input
{
    API::Function getPressed(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        scanKeys();
        u64 inp = keysDown();
        handler.ReturnUInt(inp);
        return API::Return::Variable;
    }

    API::Function getHeld(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        scanKeys();
        u64 inp = keysCurrent();
        handler.ReturnUInt(inp);
        return API::Return::Variable;
    }

    API::Function getReleased(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        scanKeys();
        u64 inp = keysUp();
        handler.ReturnUInt(inp);
        return API::Return::Variable;
    }

    API::Function getTouch(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        scanKeys();
        u64 inp = keysCurrent();
        API::Object obj(Context);
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
        return API::Return::Variable;
    }

    API::Module Initialize()
    {
        API::Module input("input");
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
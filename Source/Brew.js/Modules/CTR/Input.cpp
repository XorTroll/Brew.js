#include "Input.hpp"

Brew::API::Module input("input");

Brew::API::Function Brew::BuiltIn::Input::getPressed(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    hidScanInput();
    u64 inp = hidKeysDown();
    handler.pushUInt(inp);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::Input::getHeld(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    u64 inp = hidKeysHeld();
    handler.pushUInt(inp);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::Input::getReleased(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    u64 inp = hidKeysUp();
    handler.pushUInt(inp);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::Input::getTouch(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    hidScanInput();
    u64 inp = hidKeysHeld();
    Brew::API::Object obj(Context);
    handler.pushObject(obj);
    if(inp & KEY_TOUCH)
    {
        touchPosition tch;
        hidTouchRead(&tch);
        obj.addUInt("X", tch.px);
        obj.addUInt("Y", tch.py);
    }
    else
    {
        obj.addUInt("X", 0);
        obj.addUInt("Y", 0);
    }
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::Input::getCPadPosition(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    Brew::API::Object obj(Context);
    handler.pushObject(obj);
    circlePosition tch;
    hidCircleRead(&tch);
    obj.addUInt("X", tch.dx);
    obj.addUInt("Y", tch.dy);
    return Brew::API::Return::Variable;
}

Brew::API::Module Brew::BuiltIn::Input::initModule()
{
    input.pushUInt("A", KEY_A);
    input.pushUInt("B", KEY_B);
    input.pushUInt("Select", KEY_SELECT);
    input.pushUInt("Start", KEY_START);
    input.pushUInt("DPadRight", KEY_DRIGHT);
    input.pushUInt("DPadLeft", KEY_DLEFT);
    input.pushUInt("DPadUp", KEY_DUP);
    input.pushUInt("DPadDown", KEY_DDOWN);
    input.pushUInt("L", KEY_L);
    input.pushUInt("R", KEY_R);
    input.pushUInt("X", KEY_X);
    input.pushUInt("Y", KEY_Y);
    input.pushUInt("ZL", KEY_ZL);
    input.pushUInt("ZR", KEY_ZR);
    input.pushUInt("Touch", KEY_TOUCH);
    input.pushUInt("CStickRight", KEY_CSTICK_RIGHT);
    input.pushUInt("CStickLeft", KEY_CSTICK_UP);
    input.pushUInt("CStickUp", KEY_CSTICK_UP);
    input.pushUInt("CStickDown", KEY_CSTICK_UP);
    input.pushUInt("CPadRight", KEY_CPAD_RIGHT);
    input.pushUInt("CPadLeft", KEY_CPAD_LEFT);
    input.pushUInt("CPadUp", KEY_CPAD_UP);
    input.pushUInt("CPadDown", KEY_CPAD_DOWN);
    input.pushUInt("Up", KEY_UP);
    input.pushUInt("Down", KEY_DOWN);
    input.pushUInt("Left", KEY_LEFT);
    input.pushUInt("Right", KEY_RIGHT);
    input.pushFunction("getPressed", getPressed);
    input.pushFunction("getHeld", getHeld);
    input.pushFunction("getReleased", getReleased);
    input.pushFunction("getTouch", getTouch);
    input.pushFunction("getCPadPosition", getCPadPosition);
    return input;
}
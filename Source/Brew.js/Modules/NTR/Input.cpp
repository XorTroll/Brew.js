#include "Input.hpp"

Brew::API::Module input("input");

Brew::API::Function Brew::BuiltIn::Input::getPressed(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    scanKeys();
    u64 inp = keysDown();
    handler.pushUInt(inp);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::Input::getHeld(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    scanKeys();
    u64 inp = keysCurrent();
    handler.pushUInt(inp);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::Input::getReleased(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    scanKeys();
    u64 inp = keysUp();
    handler.pushUInt(inp);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::Input::getTouch(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    scanKeys();
    u64 inp = keysCurrent();
    Brew::API::Object obj(Context);
    handler.pushObject(obj);
    if(inp & KEY_TOUCH)
    {
        touchPosition tch;
        touchRead(&tch);
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

Brew::API::Module Brew::BuiltIn::Input::initModule()
{
    input.pushUInt("A", KEY_A);
    input.pushUInt("B", KEY_B);
    input.pushUInt("Select", KEY_SELECT);
    input.pushUInt("Start", KEY_START);
    input.pushUInt("Right", KEY_RIGHT);
    input.pushUInt("Left", KEY_LEFT);
    input.pushUInt("Up", KEY_UP);
    input.pushUInt("Down", KEY_DOWN);
    input.pushUInt("R", KEY_R);
    input.pushUInt("L", KEY_L);
    input.pushUInt("X", KEY_X);
    input.pushUInt("Y", KEY_X);
    input.pushUInt("Touch", KEY_TOUCH);
    input.pushUInt("Lid", KEY_LID);
    input.pushFunction("getPressed", getPressed);
    input.pushFunction("getHeld", getHeld);
    input.pushFunction("getReleased", getReleased);
    input.pushFunction("getTouch", getTouch);
    return input;
}
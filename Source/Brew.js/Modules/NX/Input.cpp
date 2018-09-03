#include "Input.hpp"

Brew::API::Module input("input");

Brew::API::Function Brew::BuiltIn::Input::getPressed(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    u64 inp;

    #if __cur0S == 0

        u32 player = 10;
        if(handler.checkArgc(1)) player = handler.getUInt(0);
        hidScanInput();
        inp = hidKeysDown((HidControllerID)player);

    #elif __cur0S == 1

        hidScanInput();
        inp = hidKeysDown();

    #elif __cur0S == 2

        scanKeys();
        inp = keysDown();

    #endif

    handler.pushUInt(inp);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::Input::getHeld(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    u64 inp;

    #if __cur0S == 0

        u32 player = 10;
        if(handler.checkArgc(1)) player = handler.getUInt(0);
        hidScanInput();
        inp = hidKeysHeld((HidControllerID)player);

    #elif __cur0S == 1

        hidScanInput();
        inp = hidKeysHeld();

    #elif __cur0S == 2

        scanKeys();
        inp = keysCurrent();

    #endif
    
    handler.pushUInt(inp);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::Input::getReleased(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    u32 player = 10;
    if(handler.checkArgc(1)) player = handler.getUInt(0);
    hidScanInput();
    u64 inp = hidKeysUp((HidControllerID)player);
    handler.pushUInt(inp);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::Input::getTouch(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    u32 tchn = 0;
    if(handler.checkArgc(1)) tchn = handler.getUInt(0);
    hidScanInput();
    u64 inp = hidKeysHeld(CONTROLLER_P1_AUTO);
    Brew::API::Object obj(Context);
    handler.pushObject(obj);
    if(inp & KEY_TOUCH)
    {
        touchPosition tch;
        hidTouchRead(&tch, tchn);
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

Brew::API::Function Brew::BuiltIn::Input::getTouchNumber(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    hidScanInput();
    u64 inp = hidKeysHeld(CONTROLLER_P1_AUTO);
    if(inp & KEY_TOUCH)
    {
        u32 tchc = hidTouchCount();
        handler.pushUInt(tchc);
    }
    else handler.pushUInt(0);
    return Brew::API::Return::Variable;
}

Brew::API::Module Brew::BuiltIn::Input::initModule()
{
    input.pushUInt("A", KEY_A);
    input.pushUInt("B", KEY_B);
    input.pushUInt("X", KEY_X);
    input.pushUInt("Y", KEY_Y);
    input.pushUInt("LStick", KEY_LSTICK);
    input.pushUInt("RStick", KEY_RSTICK);
    input.pushUInt("L", KEY_L);
    input.pushUInt("R", KEY_R);
    input.pushUInt("ZL", KEY_ZL);
    input.pushUInt("ZR", KEY_ZR);
    input.pushUInt("Plus", KEY_PLUS);
    input.pushUInt("Minus", KEY_MINUS);
    input.pushUInt("DPadLeft", KEY_DLEFT);
    input.pushUInt("DPadUp", KEY_DUP);
    input.pushUInt("DPadRight", KEY_DRIGHT);
    input.pushUInt("DPadDown", KEY_DDOWN);
    input.pushUInt("LStickLeft", KEY_LSTICK_LEFT);
    input.pushUInt("LStickUp", KEY_LSTICK_UP);
    input.pushUInt("LStickRight", KEY_LSTICK_RIGHT);
    input.pushUInt("LStickDown", KEY_LSTICK_DOWN);
    input.pushUInt("RStickLeft", KEY_RSTICK_LEFT);
    input.pushUInt("RStickUp", KEY_RSTICK_UP);
    input.pushUInt("RStickRight", KEY_RSTICK_RIGHT);
    input.pushUInt("RStickDown", KEY_RSTICK_DOWN);
    input.pushUInt("SL", KEY_SL);
    input.pushUInt("SR", KEY_SR);
    input.pushUInt("Touch", KEY_TOUCH);
    input.pushUInt("Up", KEY_UP);
    input.pushUInt("Down", KEY_DOWN);
    input.pushUInt("Left", KEY_LEFT);
    input.pushUInt("Right", KEY_RIGHT);
    input.pushFunction("getPressed", getPressed);
    input.pushFunction("getHeld", getHeld);
    input.pushFunction("getReleased", getReleased);
    input.pushFunction("getTouch", getTouch);
    input.pushFunction("getTouchNumber", getTouchNumber);
    return input;
}
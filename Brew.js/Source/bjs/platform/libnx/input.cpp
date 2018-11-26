#include <bjs/platform/libnx/input.hpp>

namespace bjs::libnx::input
{
    js::Module CreateModule()
    {
        js::Module input("input");
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
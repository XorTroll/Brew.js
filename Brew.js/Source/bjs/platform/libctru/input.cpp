#include <bjs/platform/libctru/input.hpp>

namespace bjs::libctru::input
{
    js::Module CreateModule()
    {
        js::Module input("input");
        input.PushUInt("A", KEY_A);
        input.PushUInt("B", KEY_B);
        input.PushUInt("Select", KEY_SELECT);
        input.PushUInt("Start", KEY_START);
        input.PushUInt("DPadRight", KEY_DRIGHT);
        input.PushUInt("DPadLeft", KEY_DLEFT);
        input.PushUInt("DPadUp", KEY_DUP);
        input.PushUInt("DPadDown", KEY_DDOWN);
        input.PushUInt("L", KEY_L);
        input.PushUInt("R", KEY_R);
        input.PushUInt("X", KEY_X);
        input.PushUInt("Y", KEY_Y);
        input.PushUInt("ZL", KEY_ZL);
        input.PushUInt("ZR", KEY_ZR);
        input.PushUInt("Touch", KEY_TOUCH);
        input.PushUInt("CStickRight", KEY_CSTICK_RIGHT);
        input.PushUInt("CStickLeft", KEY_CSTICK_UP);
        input.PushUInt("CStickUp", KEY_CSTICK_UP);
        input.PushUInt("CStickDown", KEY_CSTICK_UP);
        input.PushUInt("CPadRight", KEY_CPAD_RIGHT);
        input.PushUInt("CPadLeft", KEY_CPAD_LEFT);
        input.PushUInt("CPadUp", KEY_CPAD_UP);
        input.PushUInt("CPadDown", KEY_CPAD_DOWN);
        input.PushUInt("Up", KEY_UP);
        input.PushUInt("Down", KEY_DOWN);
        input.PushUInt("Left", KEY_LEFT);
        input.PushUInt("Right", KEY_RIGHT);
        input.PushFunction("getPressed", getPressed);
        input.PushFunction("getHeld", getHeld);
        input.PushFunction("getReleased", getReleased);
        input.PushFunction("getTouch", getTouch);
        input.PushFunction("getCPadPosition", getCPadPosition);
        return input;
    }
}
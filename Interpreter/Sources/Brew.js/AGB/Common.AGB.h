#include "Includes.AGB.h"

namespace game
{
    // game.flush()
    inline duk_ret_t CFUNC_flush(duk_context *ctx)
    {
        VBlankIntrWait();
        return 1;
    }
}

namespace input
{
    // input.checkPressed() -> key
    inline duk_ret_t CFUNC_checkPressed(duk_context *ctx)
    {
        scanKeys();
        int key = keysDown();
        duk_push_number(ctx, key);
        return 1;
    }

    // input.checkReleased() -> key
    inline duk_ret_t CFUNC_checkReleased(duk_context *ctx)
    {
        scanKeys();
        int key = keysUp();
        duk_push_number(ctx, key);
        return 1;
    }

    // input.checkHeld() -> key
    inline duk_ret_t CFUNC_checkHeld(duk_context *ctx)
    {
        scanKeys();
        int key = keysHeld();
        duk_push_number(ctx, key);
        return 1;
    }
}

void CTX_CommonAGB(duk_context *ctx)
{
    duk_push_c_function(ctx, game::CFUNC_flush, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__game_flush");
    duk_push_c_function(ctx, input::CFUNC_checkPressed, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__input_checkPressed");
    duk_push_c_function(ctx, input::CFUNC_checkReleased, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__input_checkReleased");
    duk_push_c_function(ctx, input::CFUNC_checkHeld, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__input_checkHeld");
}

string JS_CommonAGB()
{
    string js = "";

    js += "game.flush=function(){__CFUNC__game_flush();};";
    js += "device.Platform='AGB';";
    js += "device.PlatformName='Nintendo GameBoy Advance';";
    js += "input.None=0;";
	js += "input.A=1;";
	js += "input.B=2;";
	js += "input.Select=4;";
	js += "input.Start=8;";
	js += "input.Right=16;";
	js += "input.Left=32;";
	js += "input.Up=64;";
	js += "input.Down=128;";
	js += "input.R=256;";
	js += "input.L=512;";
    js += "input.checkHeld=function(){return __CFUNC__input_checkHeld();};";
    js += "input.checkPressed=function(){return __CFUNC__input_checkPressed();};";
    js += "input.checkReleased=function(){return __CFUNC__input_checkReleased();};";
    return js;
}
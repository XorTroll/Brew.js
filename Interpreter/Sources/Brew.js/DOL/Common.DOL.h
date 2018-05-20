#include "Includes.DOL.h"

namespace game
{
    // game.flush()
    inline duk_ret_t CFUNC_flush(duk_context *ctx)
    {
        VIDEO_WaitVSync();
        return 1;
    }
}

namespace input
{
    // input.checkPressed(Player) -> key
    inline duk_ret_t CFUNC_checkPressed(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            PAD_ScanPads();
            int player = duk_to_number(ctx, 0);
            if(player <= 1) player = 0;
            else player--;
            int key = PAD_ButtonsDown(player);
            duk_push_number(ctx, key);
        }
        return 1;
    }

    // input.checkReleased() -> key
    inline duk_ret_t CFUNC_checkReleased(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            PAD_ScanPads();
            int player = duk_to_number(ctx, 0);
            if(player <= 1) player = 0;
            else player--;
            int key = PAD_ButtonsUp(player);
            duk_push_number(ctx, key);
        }
        return 1;
    }

    inline duk_ret_t CFUNC_checkHeld(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            PAD_ScanPads();
            int player = duk_to_number(ctx, 0);
            if(player <= 1) player = 0;
            else player--;
            int key = PAD_ButtonsHeld(player);
            duk_push_number(ctx, key);
        }
        return 1;
    }
}

void CTX_CommonDOL(duk_context *ctx)
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

string JS_CommonDOL()
{
    string js = "";
    js += "game.flush=function(){__CFUNC__game_flush();};";
    js += "device.Platform='DOL';";
    js += "device.PlatformName='Nintendo GameCube';";
    js += "input.NoPlayer=-1;";
    js += "input.None=0;";
	js += "input.Left=0x0001;";
    js += "input.Right=0x0002;";
    js += "input.Down=0x0004;";
    js += "input.Up=0x0008;";
    js += "input.Z=0x0010;";
    js += "input.R=0x0020;";
    js += "input.L=0x0040;";
    js += "input.A=0x0100;";
    js += "input.B=0x0200;";
    js += "input.X=0x0400;";
    js += "input.Y=0x0800;";
    js += "input.Menu=0x1000;";
    js += "input.Start=0x1000;";
    js += "input.checkHeld=function(){return __CFUNC__input_checkHeld();};";
    js += "input.checkPressed=function(){return __CFUNC__input_checkPressed();};";
    js += "input.checkReleased=function(){return __CFUNC__input_checkReleased();};";
    return js;
}
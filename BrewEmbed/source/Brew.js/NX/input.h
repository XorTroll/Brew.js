
/*
    Brew.js API - C++ core
    Switch API -> Module -> input
*/

#include <switch.h>
#include "../duktape/duktape.h"

namespace input
{
    inline int getHeld(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int pl = duk_get_number(ctx, 0);
            hidScanInput();
            int inpt = hidKeysHeld((HidControllerID)pl);
            duk_push_number(ctx, inpt);
        }
        return 1;
    }

    inline int getPressed(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int pl = duk_get_number(ctx, 0);
            hidScanInput();
            int inpt = hidKeysDown((HidControllerID)pl);
            duk_push_number(ctx, inpt);
        }
        return 1;
    }

    inline int getReleased(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int pl = duk_get_number(ctx, 0);
            hidScanInput();
            int inpt = hidKeysUp((HidControllerID)pl);
            duk_push_number(ctx, inpt);
        }
        return 1;
    }

    inline int getTouchNumber(duk_context *ctx)
    {
        hidScanInput();
        int inpt = hidKeysUp((HidControllerID)10);
        if(inpt & KEY_TOUCH)
        {
            int tchno = hidTouchCount();
            duk_push_number(ctx, tchno);
        }
        return 1;
    }

    inline int getTouch_x(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int tchno = duk_get_number(ctx, 0);
            hidScanInput();
            int inpt = hidKeysHeld((HidControllerID)10);
            if(inpt & KEY_TOUCH)
            {
                touchPosition tch;
                hidTouchRead(&tch, tchno);
                int tchx = tch.px;
                duk_push_number(ctx, tchx);
            }
        }
        return 1;
    }

    inline int getTouch_y(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int tchno = duk_get_number(ctx, 0);
            hidScanInput();
            int inpt = hidKeysHeld((HidControllerID)10);
            if(inpt & KEY_TOUCH)
            {
                touchPosition tch;
                hidTouchRead(&tch, tchno);
                int tchy = tch.py;
                duk_push_number(ctx, tchy);
            }
        }
        return 1;
    }

    void Context(duk_context *ctx)
    {
        duk_push_c_function(ctx, getHeld, DUK_VARARGS);
	    duk_put_global_string(ctx, "__input__getHeld");
        duk_push_c_function(ctx, getPressed, DUK_VARARGS);
	    duk_put_global_string(ctx, "__input__getPressed");
        duk_push_c_function(ctx, getReleased, DUK_VARARGS);
	    duk_put_global_string(ctx, "__input__getReleased");
        duk_push_c_function(ctx, getTouchNumber, DUK_VARARGS);
	    duk_put_global_string(ctx, "__input__getTouchNumber");
        duk_push_c_function(ctx, getTouch_x, DUK_VARARGS);
	    duk_put_global_string(ctx, "__input__getTouch_x");
        duk_push_c_function(ctx, getTouch_y, DUK_VARARGS);
	    duk_put_global_string(ctx, "__input__getTouch_y");
    }
}
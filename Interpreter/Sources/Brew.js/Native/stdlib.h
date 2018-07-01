
/*
    Brew.js API - C++ core
    Native API -> Module -> stdlib
*/

#include <switch.h>
#include "../duktape/duktape.h"
#include <cstdlib>

namespace stdlib
{
    inline int c_rand(duk_context *ctx)
    {
        int rnd = rand();
        duk_push_number(ctx, rnd);
        return 1;
    }

    inline int c_atof(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            const char *str = duk_get_string(ctx, 0);
            double f = atof(str);
            duk_push_number(ctx, f);
        }
        return 1;
    }

    inline int c_atoi(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            const char *str = duk_get_string(ctx, 0);
            int i = atoi(str);
            duk_push_number(ctx, i);
        }
        return 1;
    }

    inline int c_exit(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int code = duk_get_number(ctx, 0);
            exit(code);
        }
        return 1;
    }

    void Context(duk_context *ctx)
    {
        duk_push_c_function(ctx, c_rand, DUK_VARARGS);
	    duk_put_global_string(ctx, "__stdlib__rand");
        duk_push_c_function(ctx, c_atof, DUK_VARARGS);
	    duk_put_global_string(ctx, "__stdlib__atof");
        duk_push_c_function(ctx, c_atoi, DUK_VARARGS);
	    duk_put_global_string(ctx, "__stdlib__atoi");
        duk_push_c_function(ctx, c_exit, DUK_VARARGS);
	    duk_put_global_string(ctx, "__stdlib__exit");
    }
}
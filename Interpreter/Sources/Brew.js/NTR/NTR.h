#include "Includes.NTR.h"

namespace ntr
{
    // ntr.isDSi() -> boolean
    inline duk_ret_t CFUNC_isDSi(duk_context *ctx)
    {
        duk_push_boolean(ctx, isDSiMode());
        return 1;
    }
}

void CTX_NTR(duk_context *ctx)
{
    duk_push_c_function(ctx, ntr::CFUNC_isDSi, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__ntr_isDSi");
}

string JS_NTR()
{
    string js = "";
    js += "device.screen.Top=0;";
    js += "device.screen.Bottom=1;";
    js += "var ntr=[];";
    js += "ntr.isDSi=function(){return __CFUNC__ntr_isDSi();};";
    return js;
}
#include <bjs/platform/libctru/ctr/Model.hpp>

namespace bjs::libctru::ctr
{
    js::Function isNew3DS(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        bool n3d = false;
        APT_CheckNew3DS(&n3d);
        handler.ReturnBoolean(n3d);
        return js::Return::Variable;
    }

    js::Function is2DS(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        u8 val;
        cfguInit();
        CFGU_GetModelNintendo2DS(&val);
        cfguExit();
        bool is2d = (val == 0);
        handler.ReturnBoolean(is2d);
        return js::Return::Variable;
    }
}
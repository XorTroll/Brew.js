#include <bjs/node/process/ProcessFunctions.hpp>

namespace bjs::node::process
{
    js::Function cwd(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        std::string cwd = "sdmc:/";
        handler.ReturnString(cwd);
        return js::Return::Variable;
    }

    js::Function exit(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        int code = 0;
        if(handler.CheckArgcEqualOrBigger(1)) code = handler.GetInt(0);
        ::exit(code);
        return js::Return::Void;
    }
}
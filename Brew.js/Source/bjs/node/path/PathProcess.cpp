#include <bjs/node/path/PathProcess.hpp>

namespace bjs::node::path
{
    js::Function basename(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqual(1))
        {
            std::string bpath = handler.GetString(0);
            std::string bname = bpath.substr(bpath.find_last_of("/\\") + 1);
            handler.ReturnString(bname);
            return js::Return::Variable;
        }
        else if(handler.CheckArgcEqualOrBigger(2))
        {
            std::string bpath = handler.GetString(0);
            std::string ext = handler.GetString(1);
            std::string bname = bpath.substr(bpath.find_last_of("/\\") + 1);
            if(bname.find_last_of(ext) != std::string::npos) bname = bname.substr(0, bname.length() - ext.length());
            handler.ReturnString(bname);
            return js::Return::Variable;
        }
        return js::Return::Void;
    }

    js::Function extname(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            std::string bpath = handler.GetString(0);
            std::string ext = bpath.substr(bpath.find_last_of("."));
            handler.ReturnString(ext);
            return js::Return::Variable;
        }
        return js::Return::Void;
    }
}
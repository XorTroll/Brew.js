#include <bjs/platform/libnx/jipc/JSIPC.hpp>
#include <iostream>

namespace bjs::libnx::jipc
{
    js::Function getService(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            std::string nsrv = handler.GetString(0);
            if(nsrv.length() > 8) nsrv = nsrv.substr(0, 8);
            Service srv = { 0 };
            smGetService(&srv, nsrv.c_str());
            handler.ReturnUInt(srv.handle);
        }
        return js::Return::Variable;
    }
}
#include <bjs/node/path.hpp>

namespace bjs::node::path
{
    js::Module CreateModule()
    {
        js::Module path("path");
        path.PushString("delimiter", ":");
        path.PushString("sep", "/");
        path.PushFunction("basename", basename);
        path.PushFunction("extname", extname);
        return path;
    }
}
#include <bjs/node/path.hpp>

namespace bjs::node::path
{
    js::Module CreateModule()
    {
        js::Module path("path");
        path.PushString("delimiter", ":");
        path.PushString("sep", "/");
        return path;
    }
}
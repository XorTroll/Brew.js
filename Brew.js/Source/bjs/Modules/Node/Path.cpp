#include <bjs/Modules/Node/Path.hpp>

namespace bjs::modules::Path
{
    js::Module Initialize()
    {
        js::Module path("path");
        path.PushString("delimiter", ":");
        path.PushString("sep", "/");
        return path;
    }
}
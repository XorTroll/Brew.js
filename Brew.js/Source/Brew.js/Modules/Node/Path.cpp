#include <Brew.js/Modules/Node/Path.hpp>

namespace Brew::BuiltIn::Path
{
    API::Module Initialize()
    {
        API::Module path("path");
        path.PushString("delimiter", ":");
        path.PushString("sep", "/");
        return path;
    }
}
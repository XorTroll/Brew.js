#include "Path.hpp"

Brew::API::Module path("path");

Brew::API::Module Brew::BuiltIn::Path::initModule()
{
    path.pushString("delimiter", ":");
    path.pushString("sep", "/");
    return path;
}
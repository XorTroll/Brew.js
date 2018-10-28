#include <Brew.js/Modules/NX/SDL2.hpp>

namespace Brew::BuiltIn::SDL2
{
    API::Module Initialize()
    {
        API::Module sdl2("sdl2");
        return sdl2;
    }
}
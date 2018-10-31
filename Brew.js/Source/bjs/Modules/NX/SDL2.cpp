#include <bjs/Modules/NX/SDL2.hpp>

namespace bjs::modules::SDL2
{
    js::Module Initialize()
    {
        js::Module sdl2("sdl2");
        return sdl2;
    }
}
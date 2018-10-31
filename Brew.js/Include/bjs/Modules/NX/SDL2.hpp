
/**

    @file PegaSwitch.hpp
    @brief Brew.js API - Switch modules - SDL2
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/JavaScript.hpp>

namespace bjs::modules::SDL2
{
    /**
        @brief Gets the Brew.js module of sdl2.
        @return The module.
    */
    js::Module Initialize();
}
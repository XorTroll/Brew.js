
/**

    @file Libs.hpp
    @brief Header to include specific external libraries depending on current platform.
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <Brew.js/Libs/duktape.h>
#include <Brew.js/Libs/json.hpp>
using namespace nlohmann;

#if __curOS == 1
    #include <Brew.js/Libs/sf2d/sf2d.h>
#elif __curOS == 0 || __curOS == 2
    #include <Brew.js/Libs/gfx/bitmap_image.hpp>
    extern "C"
    {
        #include <Brew.js/Libs/gfx/nanojpeg.h>
        #include <Brew.js/Libs/gfx/upng.h>
    }
#endif

/**

    @file Libs.hpp
    @brief Header to include specific external libraries depending on current platform.
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include "duktape.h"
#include "json.hpp"
using namespace nlohmann;
#if __curOS == 1
    #include "sf2d/sf2d.h"
#elif __curOS == 0 || __curOS == 2
    #include "gfx/bitmap_image.hpp"
    extern "C"
    {
        #include "gfx/nanojpeg.h"
        #include "gfx/upng.h"
    }
#endif
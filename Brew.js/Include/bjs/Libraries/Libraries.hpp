
/**

    @file Libraries.hpp
    @brief Header to include specific external libraries depending on current platform.
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/Libraries/duktape.h>
#include <bjs/Libraries/json.hpp>

#if __curOS == 1
    #include <bjs/Libraries/sf2d/sf2d.h>
#elif __curOS == 0 || __curOS == 2
    #include <bjs/Libraries/gfx/bitmap_image.hpp>
    extern "C"
    {
        #include <bjs/Libraries/gfx/nanojpeg.h>
        #include <bjs/Libraries/gfx/upng.h>
    }
#endif
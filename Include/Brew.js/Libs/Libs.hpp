
/**

    @file Libs.hpp
    @brief Header to include specific external libraries depending on current platform.
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include "duktape.h"
#include "bitmap_image.hpp"
extern "C"
{
    #include "nanojpeg.h"
    #include "upng.h"
}
#include "json.hpp"
using namespace nlohmann;
#if __curOS == 1

    #include "sf2d/sf2d.h"

#endif
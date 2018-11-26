
/**

    @file lib.hpp
    @brief Header to include specific external libraries depending on the building platform.
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/lib/duktape/duktape.h>
#include <bjs/lib/json/json.hpp>
#ifdef bjsPlatformLibCTRU
    #include <bjs/lib/sf2d/sf2d.h>
#endif
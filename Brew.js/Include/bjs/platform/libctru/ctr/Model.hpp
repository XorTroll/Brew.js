
/**

    @file System.hpp
    @brief Brew.js API - libctru - 'ctr' module
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>

namespace bjs::libctru::ctr
{
    /**
        @brief API JS Function: "ctr.isNew3DS() → Boolean"
        @note Returns whether the current console is a new 3DS. This description is for using the function with JavaScript.
    */
    js::Function isNew3DS(js::NativeContext Context);

    /**
        @brief API JS Function: "ctr.is2DS() → Boolean"
        @note Returns whether the current console is a 2DS. This description is for using the function with JavaScript.
    */
    js::Function is2DS(js::NativeContext Context);
}
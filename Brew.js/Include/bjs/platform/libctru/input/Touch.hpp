
/**

    @file Touch.hpp
    @brief Brew.js API - libctru - 'input' module
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>

namespace bjs::libctru::input
{
    /**
        @brief API JS Function: "input.getTouch() → Object { X, Y }"
        @note Returns the currently touched coords (if the screen is being touched). This description is for using the function with JavaScript.
    */
    js::Function getTouch(js::NativeContext Context);
}
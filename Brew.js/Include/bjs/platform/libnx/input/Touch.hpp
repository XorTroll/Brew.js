
/**

    @file Touch.hpp
    @brief Brew.js API - libnx - 'input' module
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>

namespace bjs::libnx::input
{
    /**
        @brief API JS Function: "input.getTouch() → Object { X, Y }"
        @note Returns the currently touched coords (if the screen is being touched). This description is for using the function with JavaScript.
    */
    js::Function getTouch(js::NativeContext Context);

    /**
        @brief API JS Function: "input.getTouchNumber() → Number"
        @note Returns the current number of touches (if the screen is being touched). This description is for using the function with JavaScript.
    */
    js::Function getTouchNumber(js::NativeContext Context);
}
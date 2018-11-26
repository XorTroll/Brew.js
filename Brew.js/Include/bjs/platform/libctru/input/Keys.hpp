
/**

    @file Keys.hpp
    @brief Brew.js API - libctru - 'input' module
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>

namespace bjs::libctru::input
{
    /**
        @brief API JS Function: "input.getPressed([Player]) → Number"
        @note Returns the currently pressed controller key. This description is for using the function with JavaScript.
    */
    js::Function getPressed(js::NativeContext Context);
    
    /**
        @brief API JS Function: "input.getHeld([Player]) → Number"
        @note Returns the currently held controller key. This description is for using the function with JavaScript.
    */
    js::Function getHeld(js::NativeContext Context);
    
    /**
        @brief API JS Function: "input.getReleased([Player]) → Number"
        @note Returns the currently released controller key. This description is for using the function with JavaScript.
    */
    js::Function getReleased(js::NativeContext Context);

    /**
        @brief API JS Function: "input.getCPadPosition() → Object { X, Y }"
        @note Returns the current position of the C-pad. This description is for using the function with JavaScript.
    */
    js::Function getCPadPosition(js::NativeContext Context);
}
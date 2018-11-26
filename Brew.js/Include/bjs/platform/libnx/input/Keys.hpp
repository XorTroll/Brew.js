
/**

    @file Keys.hpp
    @brief Brew.js API - libnx - 'input' module
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>

namespace bjs::libnx::input
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
}
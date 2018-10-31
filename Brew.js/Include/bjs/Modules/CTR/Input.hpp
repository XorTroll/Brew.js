
/**

    @file Input.hpp
    @brief Brew.js API - N3DS modules - Input (CTR specific)
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/JavaScript.hpp>

namespace bjs::modules::Input
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
        @brief API JS Function: "input.getTouch() → Object { X, Y }"
        @note Returns the currently touched coords (if the screen is being touched). This description is for using the function with JavaScript.
    */
    js::Function getTouch(js::NativeContext Context);
    
    /**
        @brief API JS Function: "input.getCPadPosition() → Object { X, Y }"
        @note Returns the current position of the C-pad. This description is for using the function with JavaScript.
    */
    js::Function getCPadPosition(js::NativeContext Context);

    /**
        @brief Gets the Brew.js module of input.
        @return The module.
    */
    js::Module Initialize();
}

/**

    @file Input.hpp
    @brief Brew.js API - NDS modules - Input (NTR specific)
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/JavaScript.hpp>

namespace bjs::modules::Input
{
    /**
        @brief API JS Function: "input.getPressed() → Number"
        @note Returns the currently pressed controller key. This description is for using the function with JavaScript.
    */
    js::Function getPressed(js::NativeContext Context);
    
    /**
        @brief API JS Function: "input.getHeld() → Number"
        @note Returns the currently held controller key. This description is for using the function with JavaScript.
    */
    js::Function getHeld(js::NativeContext Context);
    
    /**
        @brief API JS Function: "input.getReleased() → Number"
        @note Returns the currently released controller key. This description is for using the function with JavaScript.
    */
    js::Function getReleased(js::NativeContext Context);
    
    /**
        @brief API JS Function: "input.getTouch() → Object { X, Y }"
        @note Returns the currently touched coords (if the screen is being touched). This description is for using the function with JavaScript.
    */
    js::Function getTouch(js::NativeContext Context);

    /**
        @brief Gets the Brew.js module of input.
        @return The module.
    */
    js::Module Initialize();
}
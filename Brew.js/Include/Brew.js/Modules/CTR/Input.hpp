
/**

    @file Input.hpp
    @brief Brew.js API - N3DS modules - Input (CTR specific)
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <Brew.js/API.hpp>

namespace Brew::BuiltIn::Input
{
    /**
        @brief API JS Function: "input.getPressed([Player]) → Number"
        @note Returns the currently pressed controller key. This description is for using the function with JavaScript.
    */
    API::Function getPressed(API::NativeContext Context);
    
    /**
        @brief API JS Function: "input.getHeld([Player]) → Number"
        @note Returns the currently held controller key. This description is for using the function with JavaScript.
    */
    API::Function getHeld(API::NativeContext Context);
    
    /**
        @brief API JS Function: "input.getReleased([Player]) → Number"
        @note Returns the currently released controller key. This description is for using the function with JavaScript.
    */
    API::Function getReleased(API::NativeContext Context);
    
    /**
        @brief API JS Function: "input.getTouch() → Object { X, Y }"
        @note Returns the currently touched coords (if the screen is being touched). This description is for using the function with JavaScript.
    */
    API::Function getTouch(API::NativeContext Context);
    
    /**
        @brief API JS Function: "input.getCPadPosition() → Object { X, Y }"
        @note Returns the current position of the C-pad. This description is for using the function with JavaScript.
    */
    API::Function getCPadPosition(API::NativeContext Context);

    /**
        @brief Gets the Brew.js module of input.
        @return The module.
    */
    API::Module Initialize();
}

/**

    @file Input.hpp
    @brief Brew.js API - Switch modules - Input (NX specific)
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include "../../API.hpp"

namespace Brew
{
    namespace BuiltIn
    {
        namespace Input
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
                @brief API JS Function: "input.getTouchNumber() → Number"
                @note Returns the current number of touches (if the screen is being touched). This description is for using the function with JavaScript.
            */
            API::Function getTouchNumber(API::NativeContext Context);
            
            /**
                @brief Gets the Brew.js module of input.
                @return The module.
            */
            API::Module initModule();
        }
    }
}
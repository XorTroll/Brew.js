
/**

    @file Console.hpp
    @brief Brew.js API - NDS modules - Console (NTR specific)
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include "../../API.hpp"

namespace Brew
{
    namespace BuiltIn
    {
        namespace Console
        {
            /**
                @brief API JS Function: "console.init([Screen])"
                @note Initializes the console output. If a screen is passed then that screen is focused. This description is for using the function with JavaScript.
            */
            API::Function init(API::NativeContext Context);
            
            /**
                @brief API JS Function: "console.log(...), warn(...), error(...), info(...), debug(...)"
                @note Prints all arguments it gets to the console. This description is for using the function with JavaScript.
            */
            API::Function print(API::NativeContext Context);
            
            /**
                @brief API JS Function: "console.trace(...)"
                @note Same as "console.log(...)"" but printing 'Trace: ' before. This description is for using the function with JavaScript.
            */
            API::Function trace(API::NativeContext Context);
            
            /**
                @brief API JS Function: "console.clear()"
                @note Clears the console screen. This description is for using the function with JavaScript.
            */
            API::Function clear(API::NativeContext Context);
            
            /**
                @brief API JS Function: "console.select(Screen)"
                @note Initializes the console output. This description is for using the function with JavaScript.
            */
            API::Function select(API::NativeContext Context);
            
            /**
                @brief API JS Function: "console.refresh()"
                @note Flushes buffers. This should be called in the main loop. This description is for using the function with JavaScript.
            */
            API::Function refresh(API::NativeContext Context);
            
            /**
                @brief Gets the Brew.js module of console.
                @return The module.
            */
            API::Module initModule();
        }
    }
}
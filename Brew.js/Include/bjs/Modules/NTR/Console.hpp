
/**

    @file Console.hpp
    @brief Brew.js API - NDS modules - Console (NTR specific)
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/JavaScript.hpp>

namespace bjs::modules::Console
{
    /**
        @brief API JS Function: "console.init([Screen])"
        @note Initializes the console output. If a screen is passed then that screen is focused. This description is for using the function with JavaScript.
    */
    js::Function init(js::NativeContext Context);
    
    /**
        @brief API JS Function: "console.log(...), warn(...), error(...), info(...), debug(...)"
        @note Prints all arguments it gets to the console. This description is for using the function with JavaScript.
    */
    js::Function print(js::NativeContext Context);
    
    /**
        @brief API JS Function: "console.trace(...)"
        @note Same as "console.log(...)"" but printing 'Trace: ' before. This description is for using the function with JavaScript.
    */
    js::Function trace(js::NativeContext Context);
    
    /**
        @brief API JS Function: "console.clear()"
        @note Clears the console screen. This description is for using the function with JavaScript.
    */
    js::Function clear(js::NativeContext Context);
    
    /**
        @brief API JS Function: "console.select(Screen)"
        @note Initializes the console output. This description is for using the function with JavaScript.
    */
    js::Function select(js::NativeContext Context);
    
    /**
        @brief API JS Function: "console.refresh()"
        @note Flushes buffers. This should be called in the main loop. This description is for using the function with JavaScript.
    */
    js::Function refresh(js::NativeContext Context);
    
    /**
        @brief Gets the Brew.js module of console.
        @return The module.
    */
    js::Module Initialize();
}
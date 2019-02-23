
#pragma once
#include <bjs/js.hpp>

namespace bjs::node::console
{
    /**
        @brief API JS Function: "console.init()"
        @note Initializes the console output. This description is for using the function with JavaScript.
    */
    js::Function init(js::NativeContext Context);
    
    /**
        @brief API JS Function: "console.log(...), warn(...), error(...), info(...), debug(...)"
        @note Prints all arguments it gets to the console. This description is for using the function with JavaScript.
    */
    js::Function print(js::NativeContext Context);
    
    /**
        @brief API JS Function: "console.trace(...)"
        @note Same as "console.log(...)" but printing 'Trace: ' before. This description is for using the function with JavaScript.
    */
    js::Function trace(js::NativeContext Context);

    js::Function printAssert(js::NativeContext Context);
    
    /**
        @brief API JS Function: "console.clear()"
        @note Clears the console screen. This description is for using the function with JavaScript.
    */
    js::Function clear(js::NativeContext Context);
    
    /**
        @brief API JS Function: "console.refresh()"
        @note Flushes buffers. This should be called in the main loop. This description is for using the function with JavaScript.
    */
    js::Function refresh(js::NativeContext Context);
    
    /**
        @brief API JS Function: "console.exit()"
        @note Finishes the console output. This description is for using the function with JavaScript.
    */
    js::Function exit(js::NativeContext Context);
}
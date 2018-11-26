
#pragma once
#include <bjs/js.hpp>

namespace bjs::node::process
{
    /**
        @brief API JS Function: "process.cwd() → String"
        @note Gets the current working directory. Is hardcoded to the SD card (FAT filesystem on NDS) root. This description is for using the function with JavaScript.
    */
    js::Function cwd(js::NativeContext Context);
    
    /**
        @brief API JS Function: "process.exit(Code)"
        @note Wrapper for C/C++ "exit()" function. Use code 0 for normal exit. This description is for using the function with JavaScript.
    */
    js::Function exit(js::NativeContext Context);
}
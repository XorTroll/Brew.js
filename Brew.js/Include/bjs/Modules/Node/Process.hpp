
/**

    @file Process.hpp
    @brief Brew.js API - Node.js modules - Process
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/JavaScript.hpp>

namespace bjs::modules::Process
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

    /**
        @brief Gets the Brew.js module of process.
        @return The module.
    */
    js::Module Initialize();
}
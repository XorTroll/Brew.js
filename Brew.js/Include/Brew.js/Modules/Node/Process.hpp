
/**

    @file Process.hpp
    @brief Brew.js API - Node.js modules - Process
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <Brew.js/API.hpp>

namespace Brew::BuiltIn::Process
{
    /**
        @brief API JS Function: "process.cwd() → String"
        @note Gets the current working directory. Is hardcoded to the SD card (FAT filesystem on NDS) root. This description is for using the function with JavaScript.
    */
    API::Function cwd(API::NativeContext Context);
    
    /**
        @brief API JS Function: "process.exit(Code)"
        @note Wrapper for C/C++ "exit()" function. Use code 0 for normal exit. This description is for using the function with JavaScript.
    */
    API::Function exit(API::NativeContext Context);

    /**
        @brief Gets the Brew.js module of process.
        @return The module.
    */
    API::Module Initialize();
}

/**

    @file OS.hpp
    @brief Brew.js API - Node.js modules - OS
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <Brew.js/API.hpp>

namespace Brew::BuiltIn::OS
{
    /**
        @brief API JS Function: "os.arch()"
        @note Returns the current OS's architecture. This description is for using the function with JavaScript.
    */
    API::Function arch(API::NativeContext Context);

    /**
        @brief API JS Function: "os.platform()"
        @note Returns the current platform's name. This description is for using the function with JavaScript.
    */
    API::Function platform(API::NativeContext Context);
    
    /**
        @brief API JS Function: "os.homedir()"
        @note Returns the home directory of the current OS. This description is for using the function with JavaScript.
    */
    API::Function homedir(API::NativeContext Context);

    /**
        @brief API JS Function: "os.endianess()"
        @note Returns the endianess of the current OS (all OSs have little endian) .This description is for using the function with JavaScript.
    */
    API::Function endianess(API::NativeContext Context);

    /**
        @brief API JS Function: "os.type()"
        @note Returns the type name of the current OS. This description is for using the function with JavaScript.
    */
    API::Function type(API::NativeContext Context);

    /**
        @brief Gets the Brew.js module of os.
        @return The module.
    */
    API::Module Initialize();
}
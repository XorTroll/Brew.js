
/**

    @file Process.hpp
    @brief Brew.js API - Node.js modules - Process
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include "../../API.hpp"

namespace Brew
{
    namespace BuiltIn
    {
        namespace Process
        {
            API::Function cwd(API::NativeContext Context);
            API::Function exit(API::NativeContext Context);

            /**
                @brief Gets the Brew.js module of process.
                @return The module.
            */
            API::Module initModule();
        }
    }
}

/**

    @file NTR.hpp
    @brief Brew.js API - NDS modules - NTR
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include "../../API.hpp"

namespace Brew
{
    namespace BuiltIn
    {
        namespace NTR
        {
            /**
                @brief API JS Function: "ntr.isDSi() → Boolean"
                @note Returns whether the current console is a DSi model. This description is for using the function with JavaScript.
            */
            API::Function isDSi(API::NativeContext Context);

            /**
                @brief API JS Function: "ntr.shutDown()"
                @note Powers off the console. This description is for using the function with JavaScript.
            */
            API::Function shutDown(API::NativeContext Context);

            /**
                @brief API JS Function: "ntr.sleep()"
                @note Sets the console into sleep mode. This description is for using the function with JavaScript.
            */
            API::Function sleep(API::NativeContext Context);

            /**
                @brief API JS Function: "ntr.batteryLevel() → Number"
                @note Gets the battery level of the console. This description is for using the function with JavaScript.
            */
            API::Function batteryLevel(API::NativeContext Context);

            /**
                @brief Gets the Brew.js module of ntr.
                @return The module.
            */
            API::Module initModule();
        }
    }
}
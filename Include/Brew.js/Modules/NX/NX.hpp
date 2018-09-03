
/**

    @file NX.hpp
    @brief Brew.js API - Switch modules - NX
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include "../../API.hpp"

namespace Brew
{
    namespace BuiltIn
    {
        namespace NX
        {
            /**
                @brief API JS Function: "nx.reboot()"
                @note Reboots the console. This description is for using the function with JavaScript.
            */
            API::Function reboot(API::NativeContext Context);
            
            /**
                @brief API JS Function: "nx.reboot()"
                @note powers off the console. This description is for using the function with JavaScript.
            */
            API::Function shutDown(API::NativeContext Context);
            
            /**
                @brief API JS Function: "nx.getTitles() → Array [Number]"
                @note Gets all title IDs from SD card, gamecart and NAND. This description is for using the function with JavaScript.
            */
            API::Function getTitles(API::NativeContext Context);
            
            /**
                @brief API JS Function: "nx.getGameCartTitle() → Number"
                @note Gets the title ID from the gamecart. This description is for using the function with JavaScript.
            */
            API::Function getGameCartTitle(API::NativeContext Context);
            
            /**
                @brief API JS Function: "nx.getNANDTitles() → Array [Number]"
                @note Gets all title IDs from NAND. This description is for using the function with JavaScript.
            */
            API::Function getNANDTitles(API::NativeContext Context);
            
            /**
                @brief API JS Function: "nx.getSDTitles() → Array [Number]"
                @note Gets all title IDs from the SD card. This description is for using the function with JavaScript.
            */
            API::Function getSDTitles(API::NativeContext Context);
            
            /**
                @brief Gets the Brew.js module of nx.
                @return The module.
            */
            API::Module initModule();
        }
    }
}

/**

    @file NX.hpp
    @brief Brew.js API - Switch modules - NX
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/JavaScript.hpp>

namespace bjs::modules::NX
{
    /**
        @brief API JS Function: "nx.reboot()"
        @note Reboots the console. This description is for using the function with JavaScript.
    */
    js::Function reboot(js::NativeContext Context);
    
    /**
        @brief API JS Function: "nx.reboot()"
        @note powers off the console. This description is for using the function with JavaScript.
    */
    js::Function shutDown(js::NativeContext Context);
    
    /**
        @brief API JS Function: "nx.getTitles() → Array [Number]"
        @note Gets all title IDs from SD card, gamecart and NAND. This description is for using the function with JavaScript.
    */
    js::Function getTitles(js::NativeContext Context);
    
    /**
        @brief API JS Function: "nx.getGameCartTitle() → Number"
        @note Gets the title ID from the gamecart. This description is for using the function with JavaScript.
    */
    js::Function getGameCartTitle(js::NativeContext Context);
    
    /**
        @brief API JS Function: "nx.getNANDTitles() → Array [Number]"
        @note Gets all title IDs from NAND. This description is for using the function with JavaScript.
    */
    js::Function getNANDTitles(js::NativeContext Context);
    
    /**
        @brief API JS Function: "nx.getSDTitles() → Array [Number]"
        @note Gets all title IDs from the SD card. This description is for using the function with JavaScript.
    */
    js::Function getSDTitles(js::NativeContext Context);

    /**
        @brief Gets the Brew.js module of nx.
        @return The module.
    */
    js::Module Initialize();
}
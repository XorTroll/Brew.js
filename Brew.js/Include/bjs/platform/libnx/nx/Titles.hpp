
/**

    @file Titles.hpp
    @brief Brew.js API - libnx - 'nx' module
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>

namespace bjs::libnx::nx
{
    /**
        @brief API JS Function: "nx.getAllTitles() → Array [Number]"
        @note Gets all title IDs from SD card, gamecart and NAND. This description is for using the function with JavaScript.
    */
    js::Function getAllTitles(js::NativeContext Context);
    
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
        @brief API JS Function: "nx.getSDCardTitles() → Array [Number]"
        @note Gets all title IDs from the SD card. This description is for using the function with JavaScript.
    */
    js::Function getSDCardTitles(js::NativeContext Context);
}
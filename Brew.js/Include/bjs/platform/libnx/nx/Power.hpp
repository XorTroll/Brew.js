
/**

    @file Power.hpp
    @brief Brew.js API - libnx - 'nx' module
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>

namespace bjs::libnx::nx
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
}
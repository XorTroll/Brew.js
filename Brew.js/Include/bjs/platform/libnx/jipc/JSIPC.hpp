
/**

    @file Power.hpp
    @brief Brew.js API - libnx - 'jipc' module
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>

namespace bjs::libnx::jipc
{
    js::Function getService(js::NativeContext Context);
}
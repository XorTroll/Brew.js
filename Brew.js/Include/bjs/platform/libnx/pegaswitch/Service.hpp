
/**

    @file Titles.hpp
    @brief Brew.js API - libnx - 'pegaswitch' module
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>

namespace bjs::libnx::pegaswitch
{
    namespace Service
    {
        js::Function xDescriptor(js::NativeContext Context);
        js::Function cDescriptor(js::NativeContext Context);
        js::Function datau32(js::NativeContext Context);
        js::Function datau64(js::NativeContext Context);
        js::Function sendPid(js::NativeContext Context);
        js::Function send(js::NativeContext Context);
        js::Function ipcMsg(js::NativeContext Context);
        js::Function getService(js::NativeContext Context);
        js::Class CreateClass();
    }
}

/**

    @file Titles.hpp
    @brief Brew.js API - libnx - 'jipc' module
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>

namespace bjs::libnx::jipc
{
    namespace Object
    {
        js::Function isValid(js::NativeContext Context);
        js::Function isDomain(js::NativeContext Context);
        js::Function convertToDomain(js::NativeContext Context);
        js::Function isSubService(js::NativeContext Context);
        js::Function queryPointerBufferSize(js::NativeContext Context);
        js::Function close(js::NativeContext Context);
        js::Function prepareForRequest(js::NativeContext Context);
        js::Function requestSendProcessId(js::NativeContext Context);
        js::Function requestSetIn(js::NativeContext Context);
        js::Function processRequest(js::NativeContext Context);
        js::Function requestPopOut(js::NativeContext Context);
        js::Function finalizeRequest(js::NativeContext Context);
        js::Class CreateClass();
    }
}
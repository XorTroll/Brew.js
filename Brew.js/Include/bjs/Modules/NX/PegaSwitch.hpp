
/**

    @file PegaSwitch.hpp
    @brief Brew.js API - Switch modules - PegaSwitch
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/JavaScript.hpp>

namespace bjs::modules::PegaSwitch
{
    js::Function Service_xDescriptor(js::NativeContext Context);
    js::Function Service_cDescriptor(js::NativeContext Context);
    js::Function Service_datau32(js::NativeContext Context);
    js::Function Service_datau64(js::NativeContext Context);
    js::Function Service_sendPid(js::NativeContext Context);
    js::Function Service_send(js::NativeContext Context);
    js::Function Service_ipcMsg(js::NativeContext Context);
    js::Function Service_getService(js::NativeContext Context);
    js::Class Service();

    /**
        @brief Gets the Brew.js module of pegaswitch.
        @return The module.
    */
    js::Module Initialize();
}
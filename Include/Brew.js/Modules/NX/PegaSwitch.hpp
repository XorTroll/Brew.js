
/**

    @file PegaSwitch.hpp
    @brief Brew.js API - Switch modules - PegaSwitch
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include "../../API.hpp"

namespace Brew
{
    namespace BuiltIn
    {
        namespace PegaSwitch
        {
            API::Function Service_xDescriptor(API::NativeContext Context);
            API::Function Service_cDescriptor(API::NativeContext Context);
            API::Function Service_datau32(API::NativeContext Context);
            API::Function Service_datau64(API::NativeContext Context);
            API::Function Service_send(API::NativeContext Context);
            API::Function Service_ipcMsg(API::NativeContext Context);
            API::Function Service_getService(API::NativeContext Context);


            API::Class Service();

            /**
                @brief Gets the Brew.js module of pegaswitch.
                @return The module.
            */
            API::Module initModule();
        }
    }
}
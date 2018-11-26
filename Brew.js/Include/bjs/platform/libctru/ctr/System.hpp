
/**

    @file System.hpp
    @brief Brew.js API - libctru - 'ctr' module
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>

namespace bjs::libctru::ctr
{
    /**
        @brief API JS Function: "ctr.launchLibraryApplet(AppletID)"
        @note Attemps to launch a library applet with its ID. This description is for using the function with JavaScript.
    */
    js::Function launchLibraryApplet(js::NativeContext Context);
    
    /**
        @brief API JS Function: "ctr.getBatteryLevel() → Number"
        @note Returns the current battery level (0 to 100). This description is for using the function with JavaScript.
    */
    js::Function getBatteryLevel(js::NativeContext Context);

    /**
        @brief API JS Function: "ctr.reboot()"
        @note Reboots the console. This description is for using the function with JavaScript.
    */
    js::Function reboot(js::NativeContext Context);

    /**
        @brief API JS Function: "ctr.shutDown()"
        @note Powers off the console. This description is for using the function with JavaScript.
    */
    js::Function shutDown(js::NativeContext Context);
    
    /**
        @brief API JS Function: "ctr.isCharging() → Boolean"
        @note Returns whether the console is being charged at the moment. This description is for using the function with JavaScript.
    */
    js::Function isCharging(js::NativeContext Context);
    
    /**
        @brief API JS Function: "ctr.sendNotification(Title, Message)"
        @note Sends a notification to the news applet of the 3DS console. This description is for using the function with JavaScript.
    */
    js::Function sendNotification(js::NativeContext Context);
}
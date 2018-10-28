
/**

    @file SF2D.hpp
    @brief Brew.js API - N3DS modules - SF2D (wrapper of sf2dlib)
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <Brew.js/API.hpp>

namespace Brew::BuiltIn::SF2D
{
    /**
        @brief API JS Function: "sf2d.init()"
        @note Initializes SF2D gfx API. This description is for using the function with JavaScript.
    */
    API::Function init(API::NativeContext Context);

    /**
        @brief API JS Function: "sf2d.setClearColor(R, G, B, A)"
        @note Sets the background color when rendering frames. This description is for using the function with JavaScript.
    */
    API::Function setClearColor(API::NativeContext Context);
    
    /**
        @brief API JS Function: "sf2d.startFrame(Screen)"
        @note Sets current screen as rendering target. This description is for using the function with JavaScript.
    */
    API::Function startFrame(API::NativeContext Context);
    
    /**
        @brief API JS Function: "sf2d.endFrame()"
        @note Ends the rendering process on current target screen. This description is for using the function with JavaScript.
    */
    API::Function endFrame(API::NativeContext Context);

    /**
        @brief API JS Function: "sf2d.drawFillCircle(X, Y, Radius, R, G, B, A)"
        @note Draws a filled circle with given arguments. This description is for using the function with JavaScript.
    */
    API::Function drawFillCircle(API::NativeContext Context);

    /**
        @brief API JS Function: "sf2d.drawRectangle(X, Y, Width, Height, R, G, B, A)"
        @note Draws a rectangle with given arguments. This description is for using the function with JavaScript.
    */
    API::Function drawRectangle(API::NativeContext Context);
    
    /**
        @brief API JS Function: "sf2d.refresh()"
        @note Flushes the gfx. This should be called every main loop if used. This description is for using the function with JavaScript.
    */
    API::Function refresh(API::NativeContext Context);

    /**
        @brief API JS Function: "sf2d.exit()"
        @note Finishes SF2D gfx API. This description is for using the function with JavaScript.
    */
    API::Function exit(API::NativeContext Context);

    /**
        @brief Gets the Brew.js module of sf2d.
        @return The module.
    */
    API::Module Initialize();
}
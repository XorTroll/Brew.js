
/**

    @file Draw.hpp
    @brief Brew.js API - libctru - 'sf2d' module
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>

namespace bjs::libctru::sf2d
{
    /**
        @brief API JS Function: "sf2d.init()"
        @note Initializes SF2D gfx API. This description is for using the function with JavaScript.
    */
    js::Function init(js::NativeContext Context);

    /**
        @brief API JS Function: "sf2d.setClearColor(R, G, B, A)"
        @note Sets the background color when rendering frames. This description is for using the function with JavaScript.
    */
    js::Function setClearColor(js::NativeContext Context);
    
    /**
        @brief API JS Function: "sf2d.startFrame(Screen)"
        @note Sets current screen as rendering target. This description is for using the function with JavaScript.
    */
    js::Function startFrame(js::NativeContext Context);
    
    /**
        @brief API JS Function: "sf2d.endFrame()"
        @note Ends the rendering process on current target screen. This description is for using the function with JavaScript.
    */
    js::Function endFrame(js::NativeContext Context);

    /**
        @brief API JS Function: "sf2d.drawFillCircle(X, Y, Radius, R, G, B, A)"
        @note Draws a filled circle with given arguments. This description is for using the function with JavaScript.
    */
    js::Function drawFillCircle(js::NativeContext Context);

    /**
        @brief API JS Function: "sf2d.drawRectangle(X, Y, Width, Height, R, G, B, A)"
        @note Draws a rectangle with given arguments. This description is for using the function with JavaScript.
    */
    js::Function drawRectangle(js::NativeContext Context);
    
    /**
        @brief API JS Function: "sf2d.refresh()"
        @note Flushes the gfx. This should be called every main loop if used. This description is for using the function with JavaScript.
    */
    js::Function refresh(js::NativeContext Context);

    /**
        @brief API JS Function: "sf2d.exit()"
        @note Finishes SF2D gfx API. This description is for using the function with JavaScript.
    */
    js::Function exit(js::NativeContext Context);
}
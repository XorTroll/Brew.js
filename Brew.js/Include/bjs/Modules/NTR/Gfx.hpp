
/**

    @file Gfx.hpp
    @brief Brew.js API - NDS modules - Gfx (NTR specific)
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/JavaScript.hpp>

namespace bjs::modules::Gfx
{
    /**
        @brief API JS Function: "gfx.init()"
        @note Initializes framebuffer gfx API. This description is for using the function with JavaScript.
    */
    js::Function init(js::NativeContext Context);

    /**
        @brief API JS Function: "gfx.drawPixel(Screen, X, Y, R, G, B)"
        @note Draws a pixel with given arguments. This description is for using the function with JavaScript.
    */
    js::Function drawPixel(js::NativeContext Context);

    /**
        @brief API JS Function: "gfx.drawRectangle(Screen, X, Y, Width, Height, R, G, B)"
        @note Draws a rectangle with given arguments. This description is for using the function with JavaScript.
    */
    js::Function drawRectangle(js::NativeContext Context);
    
    /**
        @brief API JS Function: "gfx.drawScreen(Screen, R, G, B)"
        @note Draws the entire screen with given arguments. This description is for using the function with JavaScript.
    */
    js::Function drawScreen(js::NativeContext Context);

    /**
        @brief API JS Function: "gfx.drawImage(Screen, Path, X, Y)"
        @note Draws an image (BMP, PNG or JPG). This description is for using the function with JavaScript.
    */
    js::Function drawImage(js::NativeContext Context);
    
    /**
        @brief API JS Function: "gfx.refresh()"
        @note Flushes framebuffer gfx. This description is for using the function with JavaScript.
    */
    js::Function refresh(js::NativeContext Context);

    /**
        @brief Gets the Brew.js module of gfx.
        @return The module.
    */
    js::Module Initialize();
}
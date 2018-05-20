/*
*   Copyright 2017-2018 nx-hbmenu Authors
*
*   Permission to use, copy, modify, and/or distribute this software for any purpose 
*   with or without fee is hereby granted, provided that the above copyright notice 
*   and this permission notice appear in all copies.
*
*   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH 
*   REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND 
*   FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, 
*   OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
*   OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, 
*   ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef DRAW_HPP
#define DRAW_HPP

#include <switch.h>
#include "types.hpp"

extern const ffnt_header_t interuimedium20_nxfnt;
extern const ffnt_header_t interuimedium28_nxfnt;
extern const ffnt_header_t interuimedium30_nxfnt;
extern const ffnt_header_t interuimedium42_nxfnt;
extern const ffnt_header_t interuiregular14_nxfnt;
extern const ffnt_header_t interuiregular18_nxfnt;
extern const ffnt_header_t interuiregular20_nxfnt;
extern const ffnt_header_t interuiregular24_nxfnt;
extern const ffnt_header_t tahoma12_nxfnt;
extern const ffnt_header_t tahoma24_nxfnt;

#define InterUI_Medium_20 &interuimedium20_nxfnt
#define InterUI_Medium_28 &interuimedium28_nxfnt
#define InterUI_Medium_30 &interuimedium30_nxfnt
#define InterUI_Medium_42 &interuimedium42_nxfnt
#define InterUI_Regular_14 &interuiregular14_nxfnt
#define InterUI_Regular_18 &interuiregular18_nxfnt
#define InterUI_Regular_20 &interuiregular20_nxfnt
#define InterUI_Regular_24 &interuiregular24_nxfnt
#define Tahoma_12 &tahoma12_nxfnt
#define Tahoma_24 &tahoma24_nxfnt

// the following code is from nx-hbmenu
// https://github.com/switchbrew/nx-hbmenu/blob/master/common/common.h#L63

static inline u8 BlendColor(u32 src, u32 dst, u8 alpha)
{
    u8 one_minus_alpha = (u8)255 - alpha;
    return (dst*alpha + src*one_minus_alpha)/(u8)255;
}

static inline color_t MakeColor(u8 r, u8 g, u8 b, u8 a)
{
    color_t clr;
    clr.r = r;
    clr.g = g;
    clr.b = b;
    clr.a = a;
    return clr;
}

static inline void DrawPixel(u32 x, u32 y, color_t clr)
{
    u8 *framebuf = gfxGetFramebuffer(NULL, NULL);
    if (x >= 1280 || y >= 720)
        return;
    u32 off = (y * 1280 + x) * 4;
    framebuf[off] = BlendColor(framebuf[off], clr.r, clr.a); off++;
    framebuf[off] = BlendColor(framebuf[off], clr.g, clr.a); off++;
    framebuf[off] = BlendColor(framebuf[off], clr.b, clr.a); off++;
    framebuf[off] = 0xff;
}

static inline void Draw4PixelsRaw(u32 x, u32 y, color_t clr)
{
    u8 *framebuf = gfxGetFramebuffer(NULL, NULL);
    if (x >= 1280 || y >= 720 || x > 1280-4)
        return;

    u32 color = clr.r | (clr.g<<8) | (clr.b<<16) | (0xff<<24);
    u128 val = color | ((u128)color<<32) | ((u128)color<<64) | ((u128)color<<96);
    u32 off = (y * 1280 + x) * 4;
    *((u128*)&framebuf[off]) = val;
}

void rectangle(u32 x, u32 y, u32 w, u32 h, color_t color);
void rectangled(u32 x, u32 y, u32 w, u32 h, color_t color);
void DrawPixel(u32 x, u32 y, color_t clr);
void DrawText(const ffnt_header_t* font, u32 x, u32 y, color_t clr, const char* text);
void DrawTextTruncate(const ffnt_header_t* font, u32 x, u32 y, color_t clr, const char* text, u32 max_width);
void GetTextDimensions(const ffnt_header_t* font, const char* text, u32* width_out, u32* height_out);
void DrawImage(int x, int y, int width, int height, const u8 *image, ImageMode mode);

#endif
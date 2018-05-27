#include "Includes.NX.h"
#include "gfx/bmpread.h"
#include "gfx/draw.hpp"
#include "gfx/nanojpeg.c"
#include "gfx/lodepng.h"

namespace gfx
{
    struct Pixel
    {
        color_t color;
        int x;
        int y;
    };

    struct Sprite
    {
        int width;
        int height;
        int type;
        vector<Pixel> pixdata;
        u8 *rawdata;
    };

    struct Object
    {
        int id;
        Sprite spr;
        int depth;
        int x;
        int y;
        bool show;
        int hspeed;
        int vspeed;
    };

    struct Text
    {
        int id;
        color_t color;
        int x;
        int y;
        int depth;
        string content;
        const ffnt_header_t *font;
        bool show;
    };

    static vector<Object> gfxblock;
    static vector<Text> txtblock;
    static Sprite bgspr;
    static bool bgnul = true;
    inline void __gfx_render()
    {
        if(bgnul)
        {
            for(int a = 0; a < 1280; a++)
            {
                for(int b = 0; b < 720; b++)
                {
                    DrawPixel(a, b, MakeColor(255, 255, 255, 255));
                }
            }
        }
        else
        {
            if(bgspr.type == 0)
            {
                if(bgspr.pixdata.size() > 0) for(int a = 0; a < bgspr.pixdata.size(); a++)
                {
                    Pixel cpix = bgspr.pixdata[a];
                    DrawPixel(cpix.x, cpix.y, cpix.color);
                }
            }
            else if(bgspr.type == 1)
            {
                DrawImage(0, 0, bgspr.width, bgspr.height, bgspr.rawdata, IMAGE_MODE_RGB24);
            }
            else if(bgspr.type == 2)
            {
                DrawImage(0, 0, bgspr.width, bgspr.height, bgspr.rawdata, IMAGE_MODE_RGBA32);
            }
        }
        for(int d = 0; d < 16; d++)
        {
            if(gfxblock.size() > 0) for(int i = 0; i < gfxblock.size(); i++)
            {
                Object cur = gfxblock[i];
                if(cur.show && cur.depth == d)
                {
                    gfxblock[i].x += gfxblock[i].hspeed;
                    gfxblock[i].y += gfxblock[i].vspeed;
                    if(cur.spr.type == 0)
                    {
                        for(int a = 0; a < cur.spr.pixdata.size(); a++)
                        {
                            Pixel cpix = cur.spr.pixdata[a];
                            DrawPixel(cpix.x + cur.x, cpix.y + cur.y, cpix.color);
                        }
                    }
                    else if(cur.spr.type == 1)
                    {
                        DrawImage(cur.x, cur.y, cur.spr.width, cur.spr.height, cur.spr.rawdata, IMAGE_MODE_RGB24);
                    }
                    else if(cur.spr.type == 2)
                    {
                        DrawImage(cur.x, cur.y, cur.spr.width, cur.spr.height, cur.spr.rawdata, IMAGE_MODE_RGBA32);
                    }
                }
            }
            for(int j = 0; j < txtblock.size(); j++)
            {
                Text cur = txtblock[j];
                if(cur.show && cur.depth == d)
                {
                    DrawText(cur.font, cur.x, cur.y, cur.color, cur.content.c_str());
                }
            }
        }
    }

    // gfx.setBackground(Color)
    // gfx.setBackground(Path)
    inline duk_ret_t CFUNC_setBackground(duk_context *ctx)
    {
        int argc = duk_get_top(ctx);
        if (argc == 1)
        {
            string path = string(duk_get_string(ctx, 0));
            ifstream ifs(path);
            if (ifs.good())
            {
                if (path.substr(path.find_last_of(".") + 1) == "png")
                {
                    unsigned res;
                    unsigned w;
                    unsigned h;
                    u8 *raw;
                    res = lodepng_decode32_file(&raw, &w, &h, path.c_str());
                    if (!res)
                    {
                        bgspr = {(int)w, (int)h, 2, vector<Pixel>(), raw};
                    }
                }
                else if (path.substr(path.find_last_of(".") + 1) == "bmp")
                {
                    bmpread_t bmp;
                    if (bmpread(path.c_str(), BMPREAD_TOP_DOWN | BMPREAD_ANY_SIZE, &bmp))
                    {
                        bgspr = {bmp.width, bmp.height, 1, vector<Pixel>(), bmp.data};
                    }
                }
                else if (path.substr(path.find_last_of(".") + 1) == "jpg" || path.substr(path.find_last_of(".") + 1) == "jpeg")
                {
                    FILE *fle = fopen(path.c_str(), "rb");
                    if (fle)
                    {
                        u8 *buf;
                        int size;
                        fseek(fle, 0, SEEK_END);
                        size = (int)ftell(fle);
                        buf = (u8 *)malloc(size);
                        fseek(fle, 0, SEEK_SET);
                        size = (int)fread(buf, 1, size, fle);
                        if (njDecode(buf, size) == NJ_OK)
                        {
                            u8 *raw = njGetImage();
                            int w = njGetWidth();
                            int h = njGetHeight();
                            bgspr = {w, h, 1, vector<Pixel>(), raw};
                        }
                    }
                    fclose(fle);
                    njDone();
                }
            }
            ifs.close();
        }
        else if (argc >= 3)
        {
            int r = duk_get_number(ctx, 0);
            int g = duk_get_number(ctx, 1);
            int b = duk_get_number(ctx, 2);
            color_t clr = MakeColor(r, g, b, 255);
            vector<Pixel> pixd;
            for (int i = 0; i < 1280; i++)
            {
                for (int j = 0; j < 720; j++)
                {
                    pixd.push_back({clr, i, j});
                }
            }
            bgspr = { 1280, 720, 0, pixd, NULL };
        }
        bgnul = false;
        return 1;
    }

    namespace text
    {
        inline const ffnt_header_t *getfont(int fid)
        {
            const ffnt_header_t *font = InterUI_Medium_20;
            switch(fid)
            {
                case 0:
                    font = InterUI_Medium_20;
                    break;
                case 1:
                    font = InterUI_Medium_28;
                    break;
                case 2:
                    font = InterUI_Medium_30;
                    break;
                case 3:
                    font = InterUI_Medium_42;
                    break;
                case 4:
                    font = InterUI_Regular_14;
                    break;
                case 5:
                    font = InterUI_Regular_18;
                    break;
                case 6:
                    font = InterUI_Regular_20;
                    break;
                case 7:
                    font = InterUI_Regular_24;
                    break;
                case 8:
                    font = Tahoma_12;
                    break;
                case 9:
                    font = Tahoma_24;
                    break;
            }
            return font;
        }

        inline int getfontint(const ffnt_header_t *fid)
        {
            int id = 0;
            if(fid == InterUI_Medium_20) id = 0;
            else if(fid == InterUI_Medium_28) id = 1;
            else if(fid == InterUI_Medium_30) id = 2;
            else if(fid == InterUI_Medium_42) id = 3;
            else if(fid == InterUI_Regular_14) id = 4;
            else if(fid == InterUI_Regular_18) id = 5;
            else if(fid == InterUI_Regular_20) id = 6;
            else if(fid == InterUI_Regular_24) id = 7;
            else if(fid == Tahoma_12) id = 8;
            else if(fid == Tahoma_24) id = 9;
            return id;
        }

        inline int INTERNAL_textCreate(int x, int y, int depth, string content, color_t color, const ffnt_header_t *font)
        {
            int cid = rand();
            txtblock.push_back({ cid, color, x, y, depth, content, font, false });
            return cid;
        }

        inline void INTERNAL_textSetContent(int id, string content)
        {
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].content = content;
                    break;
                }
            }
        }

        inline string INTERNAL_textGetContent(int id)
        {
            string cnt;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    cnt = txtblock[i].content;
                    break;
                }
            }
            return cnt;
        }

        inline void INTERNAL_textShow(int id)
        {
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].show = true;
                    break;
                }
            }
        }

        inline void INTERNAL_textHide(int id)
        {
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].show = false;
                    break;
                }
            }
        }

        inline bool INTERNAL_textIsShown(int id)
        {
            bool sh = false;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    sh = txtblock[i].show;
                    break;
                }
            }
            return sh;
        }

        inline void INTERNAL_textSetColor(int id, color_t color)
        {
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].color = color;
                    break;
                }
            }
        }

        inline void INTERNAL_textSetFont(int id, const ffnt_header_t *font)
        {
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].font = font;
                    break;
                }
            }
        }

        inline const ffnt_header_t *INTERNAL_textGetFont(int id)
        {
            const ffnt_header_t *font;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    font = txtblock[i].font;
                    break;
                }
            }
            return font;
        }

        inline color_t INTERNAL_textGetColor(int id)
        {
            color_t clr;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    clr = txtblock[i].color;
                    break;
                }
            }
            return clr;
        }

        inline void INTERNAL_textSetXY(int id, int x, int y)
        {
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].x = x;
                    txtblock[i].y = y;
                    break;
                }
            }
        }

        inline int INTERNAL_textGetX(int id)
        {
            int rx = 0;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    rx = txtblock[i].x;
                    break;
                }
            }
            return rx;
        }

        inline int INTERNAL_textGetY(int id)
        {
            int ry = 0;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    ry = txtblock[i].y;
                    break;
                }
            }
            return ry;
        }

        // gfx.text.create(Text, X, Y, Depth, R, G, B, Font);
        inline duk_ret_t PART_create(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 8)
            {
                const char* text = duk_get_string(ctx, 0);
                int x = duk_get_number(ctx, 1);
                int y = duk_get_number(ctx, 2);
                int depth = duk_get_number(ctx, 3);
                int cr = duk_get_number(ctx, 4);
                int cg = duk_get_number(ctx, 5);
                int cb = duk_get_number(ctx, 6);
                int fnt = duk_get_number(ctx, 7);
                const ffnt_header_t *font = getfont(fnt);
                color_t clr = MakeColor(cr, cg, cb, 255);
                int txt = INTERNAL_textCreate(x, y, depth, string(text), clr, font);
                duk_push_number(ctx, txt);
            }
            return 1;
        }

        // <class gfx.text.Text>.show()
        inline duk_ret_t PART_show(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_to_number(ctx, 0);
                INTERNAL_textShow(id);
            }
            return 1;
        }

        // <class gfx.text.Text>.hide()
        inline duk_ret_t PART_hide(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_to_number(ctx, 0);
                INTERNAL_textHide(id);
            }
            return 1;
        }

        // <class gfx.text.Text>.x() -> number
        // <class gfx.text.Text>.x(NewX)
        inline duk_ret_t PART_x(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 2)
            {
                int id = duk_to_number(ctx, 0);
                int x = duk_to_number(ctx, 1);
                INTERNAL_textSetXY(id, x, INTERNAL_textGetY(id));
            }
            else if(duk_get_top(ctx) == 1)
            {
                int id = duk_to_number(ctx, 0);
                int x = INTERNAL_textGetX(id);
                duk_push_number(ctx, x);
            }
            return 1;
        }

        // <class gfx.text.Text>.y() -> number
        // <class gfx.text.Text>.y(NewY)
        inline duk_ret_t PART_y(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 2)
            {
                int id = duk_to_number(ctx, 0);
                int y = duk_to_number(ctx, 1);
                INTERNAL_textSetXY(id, INTERNAL_textGetX(id), y);
            }
            else if(duk_get_top(ctx) == 1)
            {
                int id = duk_to_number(ctx, 0);
                int y = INTERNAL_textGetY(id);
                duk_push_number(ctx, y);
            }
            return 1;
        }

        // <class gfx.text.Text>.color(Color)
        inline duk_ret_t PART_setcolor(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 4)
            {
                int id = duk_to_number(ctx, 0);
                int r = duk_to_number(ctx, 1);
                int g = duk_to_number(ctx, 2);
                int b = duk_to_number(ctx, 3);
                INTERNAL_textSetColor(id, MakeColor(r, g, b, 255));
            }
            return 1;
        }

        // <class gfx.text.Text>.color() -> object[R, G, B]
        inline duk_ret_t PART_color_r(duk_context *ctx)
        {
            if(duk_get_top(ctx) == 1)
            {
                int id = duk_to_number(ctx, 0);
                color_t cclr = INTERNAL_textGetColor(id);
                int clr = cclr.r;
                duk_push_number(ctx, clr);
            }
            return 1;
        }

        // <class gfx.text.Text>.color() -> object[R, G, B]
        inline duk_ret_t PART_color_g(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_to_number(ctx, 0);
                color_t cclr = INTERNAL_textGetColor(id);
                int clr = cclr.g;
                duk_push_number(ctx, clr);
            }
            return 1;
        }

        // <class gfx.text.Text>.color() -> object[R, G, B]
        inline duk_ret_t PART_color_b(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_to_number(ctx, 0);
                color_t cclr = INTERNAL_textGetColor(id);
                int clr = cclr.b;
                duk_push_number(ctx, clr);
            }
            return 1;
        }

        // <class gfx.text.Text>.content() -> string
        // <class gfx.text.Text>.content(Text)
        inline duk_ret_t PART_content(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 2)
            {
                int id = duk_to_number(ctx, 0);
                const char *cnt = duk_to_string(ctx, 1);
                INTERNAL_textSetContent(id, cnt);
            }
            else if(duk_get_top(ctx) == 1)
            {
                int id = duk_to_number(ctx, 0);
                const char *cnt = INTERNAL_textGetContent(id).c_str();
                duk_push_string(ctx, cnt);
            }
            return 1;
        }

        // <class gfx.text.Text>.font() -> number
        // <class gfx.text.Text>.font(Font)
        inline duk_ret_t PART_setfont(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 2)
            {
                int id = duk_to_number(ctx, 0);
                int fnt = duk_to_number(ctx, 1);
                const ffnt_header_t *font = getfont(fnt);
                INTERNAL_textSetFont(id, font);
            }
            return 1;
        }

        // <class gfx.text.Text>.font() -> number
        // <class gfx.text.Text>.font(Font)
        inline duk_ret_t PART_font(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_to_number(ctx, 0);
                int fnt = getfontint(INTERNAL_textGetFont(id));
                duk_push_number(ctx, fnt);
            }
            return 1;
        }
    }

    namespace object
    {
        // gfx.object.create(Color, Width, Height) -> number
        // gfx.object.create(Path) -> number
        inline duk_ret_t CFUNC_create(duk_context *ctx)
        {
            int argc = duk_get_top(ctx);
            if(argc == 1)
            {
                string path = string(duk_get_string(ctx, 0));
                ifstream ifs(path);
                if(ifs.good())
                {
                    if(path.substr(path.find_last_of(".") + 1) == "png")
                    {
                        unsigned res;
                        unsigned w;
                        unsigned h;
                        u8 *raw;
                        Sprite spr;
                        res = lodepng_decode32_file(&raw, &w, &h, path.c_str());
                        if(!res)
                        {
                            spr = { (int)w, (int)h, 2, vector<Pixel>(), raw };
                            int cid = rand();
                            gfxblock.push_back({ cid, spr, 0, 0, 0, false, 0, 0 });
                            duk_push_number(ctx, cid);
                        }
                    }
                    else if(path.substr(path.find_last_of(".") + 1) == "bmp")
                    {
                        bmpread_t bmp;
                        if(bmpread(path.c_str(), BMPREAD_TOP_DOWN | BMPREAD_ANY_SIZE, &bmp))
                        {
                            Sprite spr = { bmp.width, bmp.height, 1, vector<Pixel>(), bmp.data };
                            int cid = rand();
                            gfxblock.push_back({ cid, spr, 0, 0, 0, false, 0, 0 });
                            duk_push_number(ctx, cid);
                        }
                    }
                    else if(path.substr(path.find_last_of(".") + 1) == "jpg" || path.substr(path.find_last_of(".") + 1) == "jpeg")
                    {
                        Sprite spr;
                        FILE *fle = fopen(path.c_str(), "rb");
                        if(fle)
                        {
                            u8 *buf;
                            int size;
                            fseek(fle, 0, SEEK_END);
                            size = (int)ftell(fle);
                            buf = (u8*)malloc(size);
                            fseek(fle, 0, SEEK_SET);
                            size = (int)fread(buf, 1, size, fle);
                            if(njDecode(buf, size) == NJ_OK)
                            {
                                u8 *raw = njGetImage();
                                int w = njGetWidth();
                                int h = njGetHeight();
                                spr = { w, h, 1, vector<Pixel>(), raw };
                            }
                        }
                        fclose(fle);
                        njDone();
                        int cid = rand();
                        gfxblock.push_back({ cid, spr, 0, 0, 0, false, 0, 0 });
                        duk_push_number(ctx, cid);
                    }
                }
                ifs.close();
            }
            else if(argc >= 5)
            {
                int r = duk_get_number(ctx, 0);
                int g = duk_get_number(ctx, 1);
                int b = duk_get_number(ctx, 2);
                int w = duk_get_number(ctx, 3);
                int h = duk_get_number(ctx, 4);
                color_t clr = MakeColor(r, g, b, 255);
                vector<Pixel> pixd;
                for(int i = 0; i < w; i++)
                {
                    for(int j = 0; j < h; j++)
                    {
                        pixd.push_back({ clr, i, j });
                    }
                }
                Sprite spr = { w, h, 0, pixd, NULL };
                int cid = rand();
                gfxblock.push_back({ cid, spr, 0, 0, 0, false, 0, 0 });
                duk_push_number(ctx, cid);
            }
            return 1;
        }

        // <class gfx.object.Object>.show()
        inline duk_ret_t CFUNC_show(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_get_number(ctx, 0);
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        gfxblock[i].show = true;
                        break;
                    }
                }
            }
            return 1;
        }

        // <class gfx.object.Object>.hide()
        inline duk_ret_t CFUNC_hide(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_get_number(ctx, 0);
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        gfxblock[i].show = false;
                        break;
                    }
                }
            }
            return 1;
        }

        // <class gfx.object.Object>.isShown() -> boolean
        inline duk_ret_t CFUNC_isShown(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_get_number(ctx, 0);
                bool sh = false;
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        sh = gfxblock[i].show;
                        break;
                    }
                }
                duk_push_boolean(ctx, sh);
            }
            return 1;
        }

        // <class gfx.object.Object>.depth() -> number
        // <class gfx.object.Object>.depth(Depth)
        inline duk_ret_t CFUNC_depth(duk_context *ctx)
        {
            int argc = duk_get_top(ctx);
            if(argc >= 2)
            {
                int id = duk_get_number(ctx, 0);
                int dp = duk_get_number(ctx, 1);
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        gfxblock[i].depth = dp;
                        break;
                    }
                }
            }
            else if(argc == 1)
            {
                int id = duk_get_number(ctx, 0);
                int dp = 0;
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        dp = gfxblock[i].depth;
                        break;
                    }
                }
                duk_push_number(ctx, dp);
            }
            return 1;
        }

        // <class gfx.object.Object>.x() -> number
        // <class gfx.object.Object>.x(X)
        inline duk_ret_t CFUNC_x(duk_context *ctx)
        {
            int argc = duk_get_top(ctx);
            if(argc >= 2)
            {
                int id = duk_get_number(ctx, 0);
                int nx = duk_get_number(ctx, 1);
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        gfxblock[i].x = nx;
                        break;
                    }
                }
            }
            else if(argc == 1)
            {
                int id = duk_get_number(ctx, 0);
                int ox = 0;
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        ox = gfxblock[i].x;
                        break;
                    }
                }
                duk_push_number(ctx, ox);
            }
            return 1;
        }

        // <class gfx.object.Object>.y() -> number
        // <class gfx.object.Object>.y(Y)
        inline duk_ret_t CFUNC_y(duk_context *ctx)
        {
            int argc = duk_get_top(ctx);
            if(argc >= 2)
            {
                int id = duk_get_number(ctx, 0);
                int ny = duk_get_number(ctx, 1);
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        gfxblock[i].y = ny;
                        break;
                    }
                }
            }
            else if(argc == 1)
            {
                int id = duk_get_number(ctx, 0);
                int oy = 0;
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        oy = gfxblock[i].y;
                        break;
                    }
                }
                duk_push_number(ctx, oy);
            }
            return 1;
        }

        // <class gfx.object.Object>.horizontalSpeed() -> number
        // <class gfx.object.Object>.horizontalSpeed(Speed)
        inline duk_ret_t CFUNC_horizontalSpeed(duk_context *ctx)
        {
            int argc = duk_get_top(ctx);
            if(argc >= 2)
            {
                int id = duk_get_number(ctx, 0);
                int sp = duk_get_number(ctx, 1);
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        gfxblock[i].hspeed = sp;
                        break;
                    }
                }
            }
            else if(argc == 1)
            {
                int id = duk_get_number(ctx, 0);
                int sp = 0;
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        sp = gfxblock[i].hspeed;
                        break;
                    }
                }
                duk_push_number(ctx, sp);
            }
            return 1;
        }

        // <class gfx.object.Object>.verticalSpeed() -> number
        // <class gfx.object.Object>.verticalSpeed(Speed)
        inline duk_ret_t CFUNC_verticalSpeed(duk_context *ctx)
        {
            int argc = duk_get_top(ctx);
            if(argc >= 2)
            {
                int id = duk_get_number(ctx, 0);
                int sp = duk_get_number(ctx, 1);
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        gfxblock[i].vspeed = sp;
                        break;
                    }
                }
            }
            else if(argc == 1)
            {
                int id = duk_get_number(ctx, 0);
                int sp = 0;
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        sp = gfxblock[i].vspeed;
                        break;
                    }
                }
                duk_push_number(ctx, sp);
            }
            return 1;
        }

        // <class gfx.object.Object>.checkTouch() -> boolean
        inline duk_ret_t CFUNC_checkTouch(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_get_number(ctx, 0);
                bool chck = false;
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        int x = gfxblock[i].x;
                        int y = gfxblock[i].y;
                        int w = gfxblock[i].spr.width;
                        int h = gfxblock[i].spr.height;
                        hidScanInput();
                        int key = hidKeysHeld(CONTROLLER_P1_AUTO);
                        if(key & KEY_TOUCH)
                        {
                            touchPosition tch;
                            hidTouchRead(&tch, 0);
                            for(int a = x; a < (x + w); a++)
                            {
                                for(int j = y; j < (y + h); j++)
                                {
                                    if(a == tch.px && j == tch.py)
                                    {
                                        chck = true;
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                    }
                }
                duk_push_boolean(ctx, chck);
            }
            return 1;
        }

        // <class gfx.object.Object>.checkCollision(Object) -> boolean
        inline duk_ret_t CFUNC_checkCollision(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 2)
            {
                int id = duk_get_number(ctx, 0);
                int sid = duk_get_number(ctx, 1);
                bool coll = false;
                int x1, y1, w1, h1;
                int x2, y2, w2, h2;
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        x1 = gfxblock[i].x;
                        y1 = gfxblock[i].y;
                        w1 = gfxblock[i].spr.width;
                        h1 = gfxblock[i].spr.height;
                    }
                    else if(gfxblock[i].id == sid)
                    {
                        x2 = gfxblock[i].x;
                        y2 = gfxblock[i].y;
                        w2 = gfxblock[i].spr.width;
                        h2 = gfxblock[i].spr.height;
                    }
                }
                for(int a = x1; a < (x1 + w1); a++)
                {
                    for(int b = y1; b < (y1 + h1); b++)
                    {
                        if((a > x2 && a < (x2 + w2)) && (b > y2 && b < (y2 + h2)))
                        {
                            coll = true;
                            break;
                        }
                    }
                }
                duk_push_boolean(ctx, coll);
            }
            return 1;
        }

        // <class gfx.object.Object>.checkLeftCollision() -> boolean
        inline duk_ret_t CFUNC_checkLeftCollision(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_get_number(ctx, 0);
                bool coll = false;
                int x, w;
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        x = gfxblock[i].x;
                        w = gfxblock[i].spr.width;
                        break;
                    }
                }
                for(int i = x; i < (x + w); i++)
                {
                    if(i < 0)
                    {
                        coll = true;
                        break;
                    }
                }
                duk_push_boolean(ctx, coll);
            }
            return 1;
        }

        // <class gfx.object.Object>.checkRightCollision() -> boolean
        inline duk_ret_t CFUNC_checkRightCollision(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_get_number(ctx, 0);
                bool coll = false;
                int x, w;
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        x = gfxblock[i].x;
                        w = gfxblock[i].spr.width;
                        break;
                    }
                }
                for(int i = x; i < (x + w); i++)
                {
                    if(i > 1280)
                    {
                        coll = true;
                        break;
                    }
                }
                duk_push_boolean(ctx, coll);
            }
            return 1;
        }

        // <class gfx.object.Object>.checkTopCollision() -> boolean
        inline duk_ret_t CFUNC_checkTopCollision(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_get_number(ctx, 0);
                bool coll = false;
                int y, h;
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        y = gfxblock[i].y;
                        h = gfxblock[i].spr.height;
                        break;
                    }
                }
                for(int i = y; i < (y + h); i++)
                {
                    if(i < 0)
                    {
                        coll = true;
                        break;
                    }
                }
                duk_push_boolean(ctx, coll);
            }
            return 1;
        }

        // <class gfx.object.Object>.checkBottomCollision() -> boolean
        inline duk_ret_t CFUNC_checkBottomCollision(duk_context *ctx)
        {
            if(duk_get_top(ctx) >= 1)
            {
                int id = duk_get_number(ctx, 0);
                bool coll = false;
                int y, h;
                for(int i = 0; i < gfxblock.size(); i++)
                {
                    if(gfxblock[i].id == id)
                    {
                        y = gfxblock[i].y;
                        h = gfxblock[i].spr.height;
                        break;
                    }
                }
                for(int i = y; i < (y + h); i++)
                {
                    if(i > 720)
                    {
                        coll = true;
                        break;
                    }
                }
                duk_push_boolean(ctx, coll);
            }
            return 1;
        }
    }

    namespace dialog
    {
        void __drawKbd(string path)
        {
            unsigned res;
            u8 *raw;
            unsigned w;
            unsigned h;
            res = lodepng_decode32_file(&raw, &w, &h, path.c_str());
            if(!res)
            {
                DrawImage(0, 370, w, h, raw, IMAGE_MODE_RGBA32);
            }
        }

        // gfx.dialog.keyboardInput() -> string
        inline duk_ret_t CFUNC_keyboardInput(duk_context *ctx)
        {
            int diff = 370;
            bool keep = true;
            bool caps = false;
            string path = "romfs:/KeyboardDown.png";
            string pathup = "romfs:/Keyboard.png";
            if(caps) __drawKbd(pathup);
            else __drawKbd(path);
            string txt;
            while(keep)
            {
                hidScanInput();
                int key = hidKeysDown(CONTROLLER_P1_AUTO);
                if(key & KEY_TOUCH)
                {
                    touchPosition tch;
                    hidTouchRead(&tch, 0);

                    int x = 0;
                    int y = 0;
                    int w = 0;
                    int h = 0;

                    // Q
                    x = 20;
                    y = 80 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "Q";
                                else txt += "q";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // W
                    x = 125;
                    y = 80 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "W";
                                else txt += "w";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // E
                    x = 230;
                    y = 80 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "E";
                                else txt += "e";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // R
                    x = 335;
                    y = 80 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "R";
                                else txt += "r";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // T
                    x = 440;
                    y = 80 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "T";
                                else txt += "t";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // Y
                    x = 545;
                    y = 80 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "Y";
                                else txt += "y";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // U
                    x = 650;
                    y = 80 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "U";
                                else txt += "u";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // I
                    x = 755;
                    y = 80 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "I";
                                else txt += "i";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // O
                    x = 860;
                    y = 80 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "O";
                                else txt += "o";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // P
                    x = 965;
                    y = 80 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "P";
                                else txt += "p";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // A
                    x = 72;
                    y = 135 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "A";
                                else txt += "a";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // S
                    x = 177;
                    y = 135 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "S";
                                else txt += "s";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // D
                    x = 282;
                    y = 135 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "D";
                                else txt += "d";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // F
                    x = 387;
                    y = 135 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "F";
                                else txt += "f";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // G
                    x = 492;
                    y = 135 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "G";
                                else txt += "g";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // H
                    x = 597;
                    y = 135 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "H";
                                else txt += "h";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // J
                    x = 702;
                    y = 135 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "J";
                                else txt += "j";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // K
                    x = 807;
                    y = 135 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "K";
                                else txt += "k";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // L
                    x = 912;
                    y = 135 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "L";
                                else txt += "l";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // Z
                    x = 177;
                    y = 190 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "Z";
                                else txt += "z";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // X
                    x = 282;
                    y = 190 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "X";
                                else txt += "x";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // C
                    x = 387;
                    y = 190 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "C";
                                else txt += "c";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // V
                    x = 492;
                    y = 190 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "V";
                                else txt += "v";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // B
                    x = 597;
                    y = 190 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "B";
                                else txt += "b";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // N
                    x = 702;
                    y = 190 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "N";
                                else txt += "n";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // M
                    x = 807;
                    y = 190 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "M";
                                else txt += "m";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // 1
                    x = 1100;
                    y = 80 + diff;
                    w = 50;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                txt += "1";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // 2
                    x = 1155;
                    y = 80 + diff;
                    w = 50;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                txt += "2";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // 3
                    x = 1210;
                    y = 80 + diff;
                    w = 50;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                txt += "3";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // 4
                    x = 1100;
                    y = 135 + diff;
                    w = 50;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                txt += "4";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // 5
                    x = 1155;
                    y = 135 + diff;
                    w = 50;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                txt += "5";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // 6
                    x = 1210;
                    y = 135 + diff;
                    w = 50;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                txt += "6";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // 7
                    x = 1100;
                    y = 190 + diff;
                    w = 50;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                txt += "7";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // 8
                    x = 1155;
                    y = 190 + diff;
                    w = 50;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                txt += "8";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // 9
                    x = 1210;
                    y = 190 + diff;
                    w = 50;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                txt += "9";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // 0
                    x = 1155;
                    y = 245 + diff;
                    w = 50;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                txt += "0";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // <
                    x = 20;
                    y = 245 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "<";
                                else txt += ">";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // '
                    x = 125;
                    y = 245 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "\'";
                                else txt += "\"";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // ,
                    x = 230;
                    y = 245 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += ",";
                                else txt += ";";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // .
                    x = 335;
                    y = 245 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += ".";
                                else txt += ":";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // (
                    x = 440;
                    y = 245 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "(";
                                else txt += ")";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // [
                    x = 545;
                    y = 245 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "[";
                                else txt += "]";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // {
                    x = 650;
                    y = 245 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "{";
                                else txt += "}";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // +
                    x = 755;
                    y = 245 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "+";
                                else txt += "-";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // =
                    x = 860;
                    y = 245 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "=";
                                else txt += "*";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // /
                    x = 965;
                    y = 245 + diff;
                    w = 100;
                    h = 50;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(caps) txt += "/";
                                else txt += "\\";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // NUMEROS...

                    // Caps
                    x = 20;
                    y = 305 + diff;
                    w = 205;
                    h = 35;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                caps = !caps;
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // Space
                    x = 335;
                    y = 305 + diff;
                    w = 415;
                    h = 35;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                txt += " ";
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // Backspace
                    x = 860;
                    y = 305 + diff;
                    w = 205;
                    h = 35;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                if(txt.size() > 0) txt.pop_back();
                                if(caps) __drawKbd(pathup);
                                else __drawKbd(path);
                                DrawText(Tahoma_24, 31, 17 + diff, MakeColor(0, 0, 0, 255), txt.c_str());
                                for(int k = x; k < (x + w); k++)
                                {
                                    for(int l = y; l < (y + h); l++)
                                    {
                                        DrawPixel(k, l, MakeColor(0, 255, 32, 128));
                                    }
                                }
                                continue;
                            }
                        }
                    }

                    // OK
                    x = 1100;
                    y = 305 + diff;
                    w = 160;
                    h = 35;
                    for(int i = x; i < (x + w); i++)
                    {
                        for(int j = y; j < (y + h); j++)
                        {
                            if(tch.px == i && tch.py == j)
                            {
                                keep = false;
                            }
                        }
                    }
                }
                else if(key & KEY_A) keep = false;
                else if(key & KEY_B) if(txt.size() > 0) txt.pop_back();
                gfxFlushBuffers();
                gfxSwapBuffers();
                gfxWaitForVsync();
            }
            duk_push_string(ctx, txt.c_str());
            return 1;
        }
    }
}

inline void CTX_GraphicsNX(duk_context *ctx)
{
    duk_push_c_function(ctx, gfx::CFUNC_setBackground, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_setBackground");
    duk_push_c_function(ctx, gfx::text::PART_create, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__gfx_text_create");
    duk_push_c_function(ctx, gfx::text::PART_show, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__gfx_text_show");
    duk_push_c_function(ctx, gfx::text::PART_hide, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__gfx_text_hide");
    duk_push_c_function(ctx, gfx::text::PART_x, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__gfx_text_x");
    duk_push_c_function(ctx, gfx::text::PART_y, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__gfx_text_y");
    duk_push_c_function(ctx, gfx::text::PART_setcolor, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__gfx_text_setcolor");
    duk_push_c_function(ctx, gfx::text::PART_color_r, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__gfx_text_color_r");
    duk_push_c_function(ctx, gfx::text::PART_color_g, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__gfx_text_color_g");
    duk_push_c_function(ctx, gfx::text::PART_color_b, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__gfx_text_color_b");
    duk_push_c_function(ctx, gfx::text::PART_content, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__gfx_text_content");
    duk_push_c_function(ctx, gfx::text::PART_setfont, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__gfx_text_setfont");
    duk_push_c_function(ctx, gfx::text::PART_font, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__PART__gfx_text_font");
    duk_push_c_function(ctx, gfx::object::CFUNC_create, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_create");
    duk_push_c_function(ctx, gfx::object::CFUNC_show, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_show");
    duk_push_c_function(ctx, gfx::object::CFUNC_hide, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_hide");
    duk_push_c_function(ctx, gfx::object::CFUNC_isShown, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_isShown");
    duk_push_c_function(ctx, gfx::object::CFUNC_depth, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_depth");
    duk_push_c_function(ctx, gfx::object::CFUNC_x, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_x");
    duk_push_c_function(ctx, gfx::object::CFUNC_y, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_y");
    duk_push_c_function(ctx, gfx::object::CFUNC_horizontalSpeed, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_horizontalSpeed");
    duk_push_c_function(ctx, gfx::object::CFUNC_verticalSpeed, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_verticalSpeed");
    duk_push_c_function(ctx, gfx::object::CFUNC_checkTouch, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_checkTouch");
    duk_push_c_function(ctx, gfx::object::CFUNC_checkCollision, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_checkCollision");
    duk_push_c_function(ctx, gfx::object::CFUNC_checkLeftCollision, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_checkLeftCollision");
    duk_push_c_function(ctx, gfx::object::CFUNC_checkRightCollision, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_checkRightCollision");
    duk_push_c_function(ctx, gfx::object::CFUNC_checkTopCollision, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_checkTopCollision");
    duk_push_c_function(ctx, gfx::object::CFUNC_checkBottomCollision, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_object_checkBottomCollision");
    duk_push_c_function(ctx, gfx::dialog::CFUNC_keyboardInput, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__gfx_dialog_keyboardInput");
}

inline string JS_GraphicsNX()
{
    string js = "";
    js += "gfx.text=[];";
    js += "gfx.object=[];";
    js += "gfx.dialog=[];";
    js += "gfx.setBackground=function(){if(arguments.length>=1){if(typeof arguments[0]==='string'){__CFUNC__gfx_setBackground(arguments[0]);}else{__CFUNC__gfx_setBackground(arguments[0].R,arguments[0].G,arguments[0].B);}}};";
    js += "gfx.text.InterUIMedium20=0;";
    js += "gfx.text.InterUIMedium28=1;";
    js += "gfx.text.InterUIMedium30=2;";
    js += "gfx.text.InterUIMedium42=3;";
    js += "gfx.text.InterUIRegular14=4;";
    js += "gfx.text.InterUIRegular18=5;";
    js += "gfx.text.InterUIRegular20=6;";
    js += "gfx.text.InterUIRegular24=7;";
    js += "gfx.text.Tahoma12=8;";
    js += "gfx.text.Tahoma24=9;";
    js += "gfx.text.Text={ID:-1,content:function(){if(arguments.length>=1){__CFUNC__PART__gfx_text_content(this.ID,arguments[0]);}else{var cnt=__CFUNC__PART__gfx_text_content(this.ID);return cnt;}},font:function(){if(arguments.length>=1){__CFUNC__PART__gfx_text_setfont(this.ID,arguments[0]);}else{var fnt=__CFUNC__PART__gfx_text_font(this.ID);return fnt;}},x:function(){if(arguments.length>=1){__CFUNC__PART__gfx_text_x(this.ID,arguments[0]);}else{var x=__CFUNC__PART__gfx_text_x(this.ID);return x;}},y:function(){if(arguments.length>=1){__CFUNC__PART__gfx_text_y(this.ID,arguments[0]);}else{var y=__CFUNC__PART__gfx_text_y(this.ID);return y;}},color:function(){if(arguments.length>=1){__CFUNC__PART__gfx_text_setcolor(this.ID,arguments[0].R,arguments[0].G,arguments[0].B);}else{var clr=[];clr.R=__CFUNC__PART__gfx_text_color_r(this.ID);clr.G=__CFUNC__PART__gfx_text_color_g(this.ID);clr.B=__CFUNC__PART__gfx_text_color_b(this.ID);return clr;}},show:function(){__CFUNC__PART__gfx_text_show(this.ID);},hide:function(){__CFUNC__PART__gfx_text_hide(this.ID);}};";
    js += "gfx.text.create=function(Text,X,Y,Depth,Color,Font){var txt=Object.create(gfx.text.Text);txt.ID=__CFUNC__PART__gfx_text_create(Text,X,Y,Depth,Color.R,Color.G,Color.B,Font);return txt;};";
    js += "gfx.object.Object={ID:-1,show:function(){__CFUNC__gfx_object_show(this.ID);},hide:function(){__CFUNC__gfx_object_hide(this.ID);},isShown:function(){return __CFUNC__gfx_object_isShown(this.ID);},x:function(){if(arguments.length>=1){__CFUNC__gfx_object_x(this.ID,arguments[0]);}else{return __CFUNC__gfx_object_x(this.ID);}},y:function(){if(arguments.length>=1){__CFUNC__gfx_object_y(this.ID,arguments[0]);}else{return __CFUNC__gfx_object_y(this.ID);}},depth:function(){if(arguments.length>=1){__CFUNC__gfx_object_depth(this.ID,arguments[0]);}else{return __CFUNC__gfx_object_depth(this.ID);}},checkTouch:function(){return __CFUNC__gfx_object_checkTouch(this.ID);},horizontalSpeed:function(){if(arguments.length>=1){__CFUNC__gfx_object_horizontalSpeed(this.ID,arguments[0]);}else{return __CFUNC__gfx_object_horizontalSpeed(this.ID);}},verticalSpeed:function(){if(arguments.length>=1){__CFUNC__gfx_object_verticalSpeed(this.ID,arguments[0]);}else{return __CFUNC__gfx_object_verticalSpeed(this.ID);}},checkCollision:function(Object){return __CFUNC__gfx_object_checkCollision(this.ID,Object.ID);},checkLeftCollision:function(){return __CFUNC__gfx_object_checkLeftCollision(this.ID);},checkRightCollision:function(){return __CFUNC__gfx_object_checkRightCollision(this.ID);},checkTopCollision:function(){return __CFUNC__gfx_object_checkTopCollision(this.ID);},checkBottomCollision:function(){return __CFUNC__gfx_object_checkBottomCollision(this.ID);}};";
    js += "gfx.object.create=function(){if(arguments.length>=3){var obj=Object.create(gfx.object.Object);obj.ID=__CFUNC__gfx_object_create(arguments[0].R,arguments[0].G,arguments[0].B,arguments[1],arguments[2]);return obj;}else if(arguments.length==1){var obj=Object.create(gfx.object.Object);obj.ID=__CFUNC__gfx_object_create(arguments[0]);return obj;}};";
    js += "gfx.dialog.keyboardInput=function(){return __CFUNC__gfx_dialog_keyboardInput()};";
    return js;
}
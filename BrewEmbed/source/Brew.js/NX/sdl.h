
/*
    Brew.js API - C++ core
    Switch API -> Module -> sdl
*/

#include <switch.h>
#include "../duktape/duktape.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

namespace sdl
{
    struct Object
    {
        int id;
        SDL_Texture *texture;
        int depth;
        int x;
        int y;
        int w;
        int h;
        bool show;
        int hspeed;
        int vspeed;
        bool phys;
    };

    struct Text
    {
        int id;
        SDL_Color color;
        int x;
        int y;
        int depth;
        const char *text;
        TTF_Font *font;
        bool show;
        int hspeed;
        int vspeed;
    };

    static SDL_Window *sdl_wnd;
    static SDL_Surface *sdl_surf;
    static SDL_Renderer *sdl_render;

    static vector<Object> gfxblock;
    static vector<Text> txtblock;
    static Object bgobj;
    static SDL_Color bgcolor = { 255, 255, 255, 255 };
    static bool plainbg = true;
    static int gravity = 0;
    static int fps = 60;
    static Mix_Music *audio;

    inline void render()
    {
	    SDL_RenderClear(sdl_render);
        if(!plainbg)
        {
            SDL_Rect position;
            position.x = bgobj.x;
            position.y = bgobj.y;
            position.w = bgobj.w;
            position.h = bgobj.h;
            SDL_RenderCopy(sdl_render, bgobj.texture, NULL, &position); 
        }
        for(int d = 0; d < 8; d++)
        {
            if(gfxblock.size() > 0) for(int i = 0; i < gfxblock.size(); i++)
            {
                if(gfxblock[i].phys)
                {
                    gfxblock[i].x += gfxblock[i].hspeed;
                    gfxblock[i].y += gfxblock[i].vspeed;
                    gfxblock[i].y += gravity * gravity;
                }
                Object cur = gfxblock[i];
                if(cur.show && cur.depth == d)
                {
                    SDL_Rect position;
                    position.x = cur.x;
                    position.y = cur.y;
                    position.w = cur.w;
                    position.h = cur.h;
                    SDL_RenderCopy(sdl_render, cur.texture, NULL, &position);
                }
            }
            if(txtblock.size() > 0) for(int j = 0; j < txtblock.size(); j++)
            {
                Text cur = txtblock[j];
                if(cur.show && cur.depth == d)
                {
                    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(cur.font, cur.text, cur.color, 1280);
                    SDL_SetSurfaceAlphaMod(surface, 255);
                    SDL_Rect position = {cur.x, cur.y, surface->w, surface->h};
                    SDL_BlitSurface(surface, NULL, sdl_surf, &position);
                    SDL_FreeSurface(surface);
                }
            }
        }
        SDL_RenderPresent(sdl_render);
        if((1000 / fps) > (SDL_GetTicks())) SDL_Delay((1000 / fps) - SDL_GetTicks());
    }

    inline int setGravity(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int grav = duk_get_number(ctx, 0);
            gravity = grav;
        }
        return 1;
    }

    inline int setFPS(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int cfps = duk_get_number(ctx, 0);
            fps = cfps;
        }
        return 1;
    }

    inline int setBackground(duk_context *ctx)
    {
        int argc = duk_get_top(ctx);
        if (argc == 1)
        {
            string path = string(duk_get_string(ctx, 0));
            plainbg = false;
        }
        else if (argc >= 4)
        {
            int r = duk_get_number(ctx, 0);
            int g = duk_get_number(ctx, 1);
            int b = duk_get_number(ctx, 2);
            int a = duk_get_number(ctx, 3);
            bgcolor = { r, g, b, a };
            SDL_SetRenderDrawColor(sdl_render, bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
        }
        return 1;
    }

    inline int Text_create(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 10)
        {
            int cid = rand();
            const char *txt = duk_get_string(ctx, 0);
            int x = duk_get_number(ctx, 1);
            int y = duk_get_number(ctx, 2);
            int depth = duk_get_number(ctx, 3);
            int cr = duk_get_number(ctx, 4);
            int cg = duk_get_number(ctx, 5);
            int cb = duk_get_number(ctx, 6);
            int ca = duk_get_number(ctx, 7);
            const char *fntp = duk_get_string(ctx, 8);
            int fntsize = duk_get_number(ctx, 9);
            TTF_Font *fnt = TTF_OpenFont(fntp, fntsize);
            txtblock.push_back({ cid, { cr, cg, cb, ca }, x, y, depth, txt, fnt, false, 0, 0 });
            duk_push_number(ctx, cid);
        }
        return 1;
    }

    inline int Text_show(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int id = duk_get_number(ctx, 0);
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].show = true;
                    break;
                }
            }
        }
        return 1;
    }

    inline int Text_hide(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int id = duk_get_number(ctx, 0);
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].show = false;
                    break;
                }
            }
        }
        return 1;
    }

    inline int Text_isShown(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int id = duk_get_number(ctx, 0);
            bool sh = false;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    sh = txtblock[i].show;
                    break;
                }
            }
            duk_push_boolean(ctx, sh);
        }
        return 1;
    }

    inline int Text_x(duk_context *ctx)
    {
        int argc = duk_get_top(ctx);
        if(argc >= 2)
        {
            int id = duk_get_number(ctx, 0);
            int x = duk_get_number(ctx, 1);
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].x = x;
                    break;
                }
            }
        }
        else if(argc == 1)
        {
            int id = duk_get_number(ctx, 0);
            int x = -1;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    x = txtblock[i].x;
                    break;
                }
            }
            duk_push_number(ctx, x);
        }
        return 1;
    }

    inline int Text_y(duk_context *ctx)
    {
        int argc = duk_get_top(ctx);
        if(argc >= 2)
        {
            int id = duk_get_number(ctx, 0);
            int y = duk_get_number(ctx, 1);
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].y = y;
                    break;
                }
            }
        }
        else if(argc == 1)
        {
            int id = duk_get_number(ctx, 0);
            int y = -1;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    y = txtblock[i].y;
                    break;
                }
            }
            duk_push_number(ctx, y);
        }
        return 1;
    }

    inline int Text_depth(duk_context *ctx)
    {
        int argc = duk_get_top(ctx);
        if(argc >= 2)
        {
            int id = duk_get_number(ctx, 0);
            int dp = duk_get_number(ctx, 1);
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].depth = dp;
                    break;
                }
            }
        }
        else if(argc == 1)
        {
            int id = duk_get_number(ctx, 0);
            int dp = -1;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    dp = txtblock[i].depth;
                    break;
                }
            }
            duk_push_number(ctx, dp);
        }
        return 1;
    }

    inline int Text_text(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 2)
        {
            int id = duk_get_number(ctx, 0);
            const char *txt = duk_get_string(ctx, 1);
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].text = txt;
                    break;
                }
            }
        }
        else if(duk_get_top(ctx) == 1)
        {
            int id = duk_get_number(ctx, 0);
            const char *txt;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txt = txtblock[i].text;
                    break;
                }
            }
            duk_push_string(ctx, txt);
        }
        return 1;
    }

    inline int Text_color(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 4)
        {
            int id = duk_get_number(ctx, 0);
            int cr = duk_get_number(ctx, 1);
            int cg = duk_get_number(ctx, 2);
            int cb = duk_get_number(ctx, 3);
            int ca = duk_get_number(ctx, 4);
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    txtblock[i].color = { cr, cg, cb, ca };
                    break;
                }
            }
        }
        return 1;
    }

    inline int Text_color_r(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int id = duk_get_number(ctx, 0);
            int clr = 0;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    clr = txtblock[i].color.r;
                    break;
                }
            }
            duk_push_number(ctx, clr);
        }
        return 1;
    }

    inline int Text_color_g(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int id = duk_get_number(ctx, 0);
            int clr = 0;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    clr = txtblock[i].color.g;
                    break;
                }
            }
            duk_push_number(ctx, clr);
        }
        return 1;
    }

    inline int Text_color_b(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int id = duk_get_number(ctx, 0);
            int clr = 0;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    clr = txtblock[i].color.b;
                    break;
                }
            }
            duk_push_number(ctx, clr);
        }
        return 1;
    }

    inline int Text_color_a(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            int id = duk_get_number(ctx, 0);
            int clr = 0;
            for(int i = 0; i < txtblock.size(); i++)
            {
                if(txtblock[i].id == id)
                {
                    clr = txtblock[i].color.a;
                    break;
                }
            }
            duk_push_number(ctx, clr);
        }
        return 1;
    }

    inline int Object_create(duk_context *ctx)
    {
        int argc = duk_get_top(ctx);
        if(argc == 4)
        {
            const char *path = duk_get_string(ctx, 0);
            int x = duk_get_number(ctx, 1);
            int y = duk_get_number(ctx, 2);
            int depth = duk_get_number(ctx, 3);
            SDL_Surface *srf = IMG_Load(path);
            if(srf)
            {
                int cid = rand();
                int w = srf->w;
                int h = srf->h;
                SDL_SetColorKey(srf, SDL_TRUE, SDL_MapRGBA(srf->format, 0, 0, 0, 0));
                SDL_Texture *txt = SDL_CreateTextureFromSurface(sdl_render, srf);
                gfxblock.push_back({ cid, txt, depth, x, y, w, h, false, 0, 0, false });
                duk_push_number(ctx, cid);
            }
            SDL_FreeSurface(srf);
        }
        else if(argc >= 9)
        {
            int x = duk_get_number(ctx, 0);
            int y = duk_get_number(ctx, 1);
            int w = duk_get_number(ctx, 2);
            int h = duk_get_number(ctx, 3);
            int r = duk_get_number(ctx, 4);
            int g = duk_get_number(ctx, 5);
            int b = duk_get_number(ctx, 6);
            int a = duk_get_number(ctx, 7);
            int depth = duk_get_number(ctx, 8);
            int cid = rand();
            SDL_Rect rct;
            rct.x = x;
            rct.y = y;
            rct.w = w;
            rct.h = h;
            SDL_Surface *srf;
            SDL_FillRect(srf, &rct, SDL_MapRGBA(srf->format, r, g, b, a));
            SDL_Texture *txt = SDL_CreateTextureFromSurface(sdl_render, srf);
            gfxblock.push_back({ cid, txt, depth, x, y, w, h, false, 0, 0, false });
            duk_push_number(ctx, cid);
        }
        return 1;
    }

    inline int Object_show(duk_context *ctx)
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

    inline int Object_hide(duk_context *ctx)
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

    inline int Object_isShown(duk_context *ctx)
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

    inline int Object_x(duk_context *ctx)
    {
        int argc = duk_get_top(ctx);
        if(argc >= 2)
        {
            int id = duk_get_number(ctx, 0);
            int x = duk_get_number(ctx, 1);
            for(int i = 0; i < gfxblock.size(); i++)
            {
                if(gfxblock[i].id == id)
                {
                    gfxblock[i].x = x;
                    break;
                }
            }
        }
        else if(argc == 1)
        {
            int id = duk_get_number(ctx, 0);
            int x = -1;
            for(int i = 0; i < gfxblock.size(); i++)
            {
                if(gfxblock[i].id == id)
                {
                    x = gfxblock[i].x;
                    break;
                }
            }
            duk_push_number(ctx, x);
        }
        return 1;
    }

    inline int Object_y(duk_context *ctx)
    {
        int argc = duk_get_top(ctx);
        if(argc >= 2)
        {
            int id = duk_get_number(ctx, 0);
            int y = duk_get_number(ctx, 1);
            for(int i = 0; i < gfxblock.size(); i++)
            {
                if(gfxblock[i].id == id)
                {
                    gfxblock[i].y = y;
                    break;
                }
            }
        }
        else if(argc == 1)
        {
            int id = duk_get_number(ctx, 0);
            int y = -1;
            for(int i = 0; i < gfxblock.size(); i++)
            {
                if(gfxblock[i].id == id)
                {
                    y = gfxblock[i].y;
                    break;
                }
            }
            duk_push_number(ctx, y);
        }
        return 1;
    }

    inline int Object_depth(duk_context *ctx)
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
            int dp = -1;
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

    inline int Object_hspeed(duk_context *ctx)
    {
        int argc = duk_get_top(ctx);
        if(argc >= 2)
        {
            int id = duk_get_number(ctx, 0);
            int hs = duk_get_number(ctx, 1);
            for(int i = 0; i < gfxblock.size(); i++)
            {
                if(gfxblock[i].id == id)
                {
                    gfxblock[i].hspeed = hs;
                    break;
                }
            }
        }
        else if(argc == 1)
        {
            int id = duk_get_number(ctx, 0);
            int hs = -1;
            for(int i = 0; i < gfxblock.size(); i++)
            {
                if(gfxblock[i].id == id)
                {
                    hs = gfxblock[i].hspeed;
                    break;
                }
            }
            duk_push_number(ctx, hs);
        }
        return 1;
    }

    inline int Object_vspeed(duk_context *ctx)
    {
        int argc = duk_get_top(ctx);
        if(argc >= 2)
        {
            int id = duk_get_number(ctx, 0);
            int vs = duk_get_number(ctx, 1);
            for(int i = 0; i < gfxblock.size(); i++)
            {
                if(gfxblock[i].id == id)
                {
                    gfxblock[i].vspeed = vs;
                    break;
                }
            }
        }
        else if(argc == 1)
        {
            int id = duk_get_number(ctx, 0);
            int vs = -1;
            for(int i = 0; i < gfxblock.size(); i++)
            {
                if(gfxblock[i].id == id)
                {
                    vs = gfxblock[i].vspeed;
                    break;
                }
            }
            duk_push_number(ctx, vs);
        }
        return 1;
    }

    inline int Object_usesPhysics(duk_context *ctx)
    {
        int argc = duk_get_top(ctx);
        if(argc >= 2)
        {
            int id = duk_get_number(ctx, 0);
            bool ph = duk_get_number(ctx, 1);
            for(int i = 0; i < gfxblock.size(); i++)
            {
                if(gfxblock[i].id == id)
                {
                    gfxblock[i].phys = ph;
                    break;
                }
            }
        }
        else if(argc == 1)
        {
            int id = duk_get_number(ctx, 0);
            int ph = false;
            for(int i = 0; i < gfxblock.size(); i++)
            {
                if(gfxblock[i].id == id)
                {
                    ph = gfxblock[i].phys;
                    break;
                }
            }
            duk_push_number(ctx, ph);
        }
        return 1;
    }

    inline int Object_checkCollide(duk_context *ctx)
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
                    w1 = gfxblock[i].w;
                    h1 = gfxblock[i].h;
                }
                else if(gfxblock[i].id == sid)
                {
                    x2 = gfxblock[i].x;
                    y2 = gfxblock[i].y;
                    w2 = gfxblock[i].w;
                    h2 = gfxblock[i].h;
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

    inline int Object_leftCollide(duk_context *ctx)
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
                    w = gfxblock[i].w;
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

    inline int Object_rightCollide(duk_context *ctx)
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
                    w = gfxblock[i].w;
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

    inline int Object_topCollide(duk_context *ctx)
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
                    h = gfxblock[i].h;
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

    inline int Object_bottomCollide(duk_context *ctx)
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
                    h = gfxblock[i].h;
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

    inline int openAudio(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            const char *aud = duk_get_string(ctx, 0);
            audio = Mix_LoadMUS(aud);
        }
        return 1;
    }

    inline int playAudio(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            bool loop = duk_get_boolean(ctx, 0);
            int times = loop? -1 : 1;
            Mix_PlayMusic(audio, times);
        }
        return 1;
    }

    inline int pauseAudio(duk_context *ctx)
    {
        Mix_PauseMusic();
        return 1;
    }

    inline int resumeAudio(duk_context *ctx)
    {
        Mix_ResumeMusic();
        return 1;
    }

    inline int pausedAudio(duk_context *ctx)
    {
        bool paused = (Mix_PausedMusic() == 1);
        duk_push_boolean(ctx, paused);
        return 1;
    }

    inline int playingAudio(duk_context *ctx)
    {
        bool playing = (Mix_PlayingMusic() == 1);
        duk_push_boolean(ctx, playing);
        return 1;
    }

    inline int stopAudio(duk_context *ctx)
    {
        Mix_HaltMusic();
        return 1;
    }

    inline int restartAudio(duk_context *ctx)
    {
        Mix_RewindMusic();
        return 1;
    }

    inline int typeAudio(duk_context *ctx)
    {
        string type;
        switch(Mix_GetMusicType(audio))
        {
            case MUS_CMD:
                type = "CMD";
                break;
            case MUS_WAV:
                type = "WAV";
                break;
            case MUS_MOD:
                type = "MOD";
                break;
            case MUS_MID:
                type = "MID";
                break;
            case MUS_OGG:
                type = "OGG";
                break;
            case MUS_MP3:
                type = "MP3";
                break;
            default:
                type = "Unknown";
                break;
        }
        duk_push_string(ctx, type.c_str());
        return 1;
    }

    void Context(duk_context *ctx)
    {
        duk_push_c_function(ctx, setGravity, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__setGravity");
        duk_push_c_function(ctx, setFPS, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__setFPS");
        duk_push_c_function(ctx, setBackground, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__setBackground");
        duk_push_c_function(ctx, Text_create, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_create");
        duk_push_c_function(ctx, Text_show, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_show");
        duk_push_c_function(ctx, Text_hide, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_hide");
        duk_push_c_function(ctx, Text_isShown, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_isShown");
        duk_push_c_function(ctx, Text_x, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_x");
        duk_push_c_function(ctx, Text_y, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_y");
        duk_push_c_function(ctx, Text_depth, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_depth");
        duk_push_c_function(ctx, Text_text, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_text");
        duk_push_c_function(ctx, Text_color, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_color");
        duk_push_c_function(ctx, Text_color_r, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_color_r");
        duk_push_c_function(ctx, Text_color_g, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_color_g");
        duk_push_c_function(ctx, Text_color_b, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_color_b");
        duk_push_c_function(ctx, Text_color_a, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Text_color_a");
        duk_push_c_function(ctx, Object_create, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_create");
        duk_push_c_function(ctx, Object_show, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_show");
        duk_push_c_function(ctx, Object_hide, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_hide");
        duk_push_c_function(ctx, Object_isShown, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_isShown");
        duk_push_c_function(ctx, Object_x, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_x");
        duk_push_c_function(ctx, Object_y, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_y");
        duk_push_c_function(ctx, Object_depth, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_depth");
        duk_push_c_function(ctx, Object_hspeed, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_hspeed");
        duk_push_c_function(ctx, Object_vspeed, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_vspeed");
        duk_push_c_function(ctx, Object_usesPhysics, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_usesPhysics");
        duk_push_c_function(ctx, Object_checkCollide, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_checkCollide");
        duk_push_c_function(ctx, Object_leftCollide, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_leftCollide");
        duk_push_c_function(ctx, Object_rightCollide, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_rightCollide");
        duk_push_c_function(ctx, Object_topCollide, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_topCollide");
        duk_push_c_function(ctx, Object_bottomCollide, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__Object_bottomCollide");
        duk_push_c_function(ctx, openAudio, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__openAudio");
        duk_push_c_function(ctx, playAudio, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__playAudio");
        duk_push_c_function(ctx, pauseAudio, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__pauseAudio");
        duk_push_c_function(ctx, resumeAudio, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__resumeAudio");
        duk_push_c_function(ctx, pausedAudio, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__pausedAudio");
        duk_push_c_function(ctx, playingAudio, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__playingAudio");
        duk_push_c_function(ctx, stopAudio, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__stopAudio");
        duk_push_c_function(ctx, restartAudio, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__restartAudio");
        duk_push_c_function(ctx, typeAudio, DUK_VARARGS);
	    duk_put_global_string(ctx, "__sdl__typeAudio");
    }
}
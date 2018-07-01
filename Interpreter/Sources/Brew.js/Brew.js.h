
/*
    Brew.js API - C++ core
    Main C++ header for Brew.js core

    Include this if you want to embed Brew.js API to your C/C++ app
*/

#include "Includes.h"
#include "duktape/duktape.h"

#include "Native/stdio.h"
#include "Native/stdlib.h"

#include "Node/fs.h"
#include "Node/os.h"
#include "Node/path.h"
#include "Node/process.h"

#include "NX/game.h"
#include "NX/sdl.h"
#include "NX/input.h"
#include "NX/nx.h"

static string lpath;

inline int DUKT_Eval(duk_context *ctx, void *udata)
{
	(void)udata;
	duk_eval(ctx);
	return 1;
}

inline int DUKT_ToStr(duk_context *ctx, void *udata)
{
	(void)udata;
	duk_to_string(ctx, -1);
	return 1;
}

inline int flush(duk_context *ctx)
{
    sdl::render();
    return 1;
}

inline int libAccessJS(duk_context *ctx)
{
    if(duk_get_top(ctx) >= 1)
    {
        string mod = lpath + "/" + string(duk_get_string(ctx, 0));
        ifstream ifs(mod);
        string out;
        if(ifs.good())
        {
            stringstream strm;
            strm << ifs.rdbuf();
            out = strm.str();
        }
        duk_push_string(ctx, out.c_str());
    }
    return 1;
}

void Context(duk_context *ctx)
{
    duk_push_c_function(ctx, flush, DUK_VARARGS);
	duk_put_global_string(ctx, "__flush");
    duk_push_c_function(ctx, libAccessJS, DUK_VARARGS);
	duk_put_global_string(ctx, "__libAccessJS");
}

namespace Brew
{
    inline string ParseLib()
    {
        string code;
        string lib = lpath + "/Brew.js";
        ifstream ifs(lib);
        if(!ifs.good())
        {
            consoleInit(NULL);
            cout << endl;
            cout << "   ERROR: Brew.js's internal file not found:  " << lib << endl;
            while(true)
            {
                gfxFlushBuffers();
                gfxSwapBuffers();
                gfxWaitForVsync();
            }
        }
        stringstream strm;
        strm << ifs.rdbuf();
        code += strm.str();
        ifs.close();
        return code;
    }

    inline void Init(string LibsPath = "romfs:/Lib")
    {
        srand(time(NULL));
        romfsInit();
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_CreateWindowAndRenderer(1280, 720, 0, &sdl::sdl_wnd, &sdl::sdl_render);
        sdl::sdl_surf = SDL_GetWindowSurface(sdl::sdl_wnd);
        SDL_SetRenderDrawBlendMode(sdl::sdl_render, SDL_BLENDMODE_BLEND);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
        TTF_Init();
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
        Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
        lpath = LibsPath;
    }

    inline void Exit()
    {
        TTF_Quit();
        IMG_Quit();
        Mix_CloseAudio();
        Mix_Quit();
        SDL_DestroyRenderer(sdl::sdl_render);
        SDL_FreeSurface(sdl::sdl_surf);
        SDL_DestroyWindow(sdl::sdl_wnd);
        SDL_Quit();
        romfsExit();
    }

    inline string Evaluate(string Source)
    {
        SDL_SetRenderDrawColor(sdl::sdl_render, 255, 255, 255, 255);
        SDL_FillRect(sdl::sdl_surf, NULL, NULL);

        string jscode = ParseLib() + Source;
        duk_context *ctx = duk_create_heap_default();

        Context(ctx);

        stdio::Context(ctx);
        stdlib::Context(ctx);

        fs::Context(ctx);
        os::Context(ctx);
        path::Context(ctx);
        process::Context(ctx);

        game::Context(ctx);
        sdl::Context(ctx);
        input::Context(ctx);
        nx::Context(ctx);

        duk_push_string(ctx, jscode.c_str());
        duk_safe_call(ctx, DUKT_Eval, NULL, 1, 1);
        duk_safe_call(ctx, DUKT_ToStr, NULL, 1, 1);
        string rst = string(duk_get_string(ctx, -1));
        duk_pop(ctx);

        sdl::gfxblock = vector<sdl::Object>();
        sdl::txtblock = vector<sdl::Text>();
        sdl::plainbg = true;

        return rst;
    }

    inline string EvaluateFile(string Path)
    {
        string dir = Path.substr(0, Path.find_last_of("/\\"));
        string src;
        src += "var __dirname = '" + dir + "';";
		src += "var __filename = '" + Path + "';";
        ifstream ifs(Path);
        if(ifs.good())
        {
            stringstream strm;
            strm << ifs.rdbuf();
            src += strm.str();
        }
        ifs.close();
        string rst = Evaluate(src);
        return rst;
    }
}
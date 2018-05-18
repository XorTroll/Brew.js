#include "Includes.h"

namespace game
{
    // game.exit()
    inline duk_ret_t CFUNC_exit(duk_context *ctx)
    {
        exit(0);
        return 1;
    }
}

namespace fs
{
    // fs.exists(Path) -> boolean
    inline duk_ret_t CFUNC_exists(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            const char* path = duk_to_string(ctx, 0);
            ifstream ifs(path);
            bool ext = ifs.good();
            ifs.close();
            duk_push_boolean(ctx, ext);
        }
        return 1;
    }

    // fs.isFile(Path) -> boolean
    inline duk_ret_t CFUNC_isFile(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            const char *pth = duk_to_string(ctx, 0);
            struct stat pinfo;
            if(stat(pth, &pinfo) == 0)
            {
                duk_push_boolean(ctx, (duk_bool_t)(pinfo.st_mode & S_IFREG));
            }
        }
        return 1;
    }

    // fs.isDirectory(Path) -> boolean
    inline duk_ret_t CFUNC_isDirectory(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            const char *pth = duk_to_string(ctx, 0);
            struct stat pinfo;
            if(stat(pth, &pinfo) == 0)
            {
                duk_push_boolean(ctx, (duk_bool_t)(pinfo.st_mode & S_IFDIR));
            }
        }
        return 1;
    }

    // fs.readText(Path) -> string
    inline duk_ret_t CFUNC_readText(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 1)
        {
            const char *fle = duk_to_string(ctx, 0);
            ifstream ifs;
            ifs.open(fle);
            if(ifs.good())
            {
                stringstream strm;
                strm << ifs.rdbuf();
                duk_push_string(ctx, strm.str().c_str());
            }
        }
        return 1;
    }

    // fs.writeText(Path, Text)
    inline duk_ret_t CFUNC_writeText(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 2)
        {
            const char *fle = duk_to_string(ctx, 0);
            const char *txt = duk_to_string(ctx, 1);
            ofstream ofs;
            ofs.open(fle);
            if(ofs.good())
            {
                ofs << txt;
            }
        }
        return 1;
    }
}

namespace console
{
    // console.log(...)
    inline duk_ret_t CFUNC_log(duk_context *ctx)
    {
        int argc = duk_get_top(ctx);
        if (argc > 0) for (int i = 0; i < argc; i++)
        {
            cout << duk_to_string(ctx, i);
        }
        return 1;
    }

    // console.clear()
    inline duk_ret_t CFUNC_clear(duk_context *ctx)
    {
        cout << "\x1b[2J";
        return 1;
    }

    inline duk_ret_t INTERNAL_prline(duk_context *ctx)
    {
        cout << endl;
        return 1;
    }
}

void CTX_Common(duk_context *ctx)
{
    duk_push_c_function(ctx, game::CFUNC_exit, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__game_exit");
    duk_push_c_function(ctx, fs::CFUNC_exists, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__fs_exists");
    duk_push_c_function(ctx, fs::CFUNC_isFile, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__fs_isFile");
    duk_push_c_function(ctx, fs::CFUNC_isDirectory, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__fs_isDirectory");
    duk_push_c_function(ctx, fs::CFUNC_readText, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__fs_readText");
    duk_push_c_function(ctx, fs::CFUNC_writeText, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__fs_writeText");
    duk_push_c_function(ctx, console::CFUNC_log, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__console_log");
    duk_push_c_function(ctx, console::CFUNC_clear, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__console_clear");
    duk_push_c_function(ctx, console::INTERNAL_prline, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__INTERNAL__console__prline");
}

string JS_Common()
{
    string js = "";
    js += "var game=[];";
    js += "var device=[];";
    js += "device.screen=[];";
    js += "var input=[];";
    js += "var fs=[];";
    js += "var console=[];";
    js += "var gfx=[];";
    js += "game.exit=function(){__CFUNC__game_exit();};";
    js += "fs.exists=function(Path){return __CFUNC__fs_exists(Path);};";
    js += "fs.isFile=function(Path){return __CFUNC__fs_isFile(Path);};";
    js += "fs.isDirectory=function(Path){return __CFUNC__fs_isDirectory(Path);};";
    js += "fs.readText=function(Path){return __CFUNC__fs_readText(Path);};";
    js += "fs.writeText=function(Path,Text){__CFUNC__fs_writeText(Path,Text);};";
    js += "console.log=function(){for(var i=0;i!=arguments.length;i++){__CFUNC__console_log(arguments[i]);}};";
    js += "console.clear=function(){__CFUNC__console_clear();};";
    js += "console.logLine=function(){for(var i=0;i!=arguments.length;i++){__CFUNC__console_log(arguments[i]);}__CFUNC__INTERNAL__console__prline();};";
    return js;
}
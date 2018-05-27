#include "Includes.h"
#include "inih/INIReader.h"

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

    // fs.configReadValue(Path, Section, Name) -> string
    inline duk_ret_t CFUNC_readConfigFile(duk_context *ctx)
    {
        if(duk_get_top(ctx) >= 3)
        {
            string file = string(duk_to_string(ctx, 0));
            string sect = string(duk_to_string(ctx, 1));
            string name = string(duk_to_string(ctx, 2));
            INIReader cfg(file);
            if(cfg.ParseError() == 0)
            {
                string value = cfg.Get(sect, name, "undefined");
                duk_push_string(ctx, value.c_str());
            }
        }
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
    duk_push_c_function(ctx, fs::CFUNC_readConfigFile, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__fs_readConfigFile");
}

string JS_Common()
{
    string js = "";
    js += "var game=[];";
    js += "var device=[];";
    js += "device.screen=[];";
    js += "var input=[];";
    js += "var fs=[];";
    js += "var gfx=[];";
    js += "game.exit=function(){__CFUNC__game_exit();};";
    js += "device.randomNumber=function(){if(arguments.length>=2){var min=arguments[0];var max=arguments[1];return Math.floor(Math.random()*(max-min+1))+min;}else{var max=arguments[0];return Math.floor(Math.random()*(max+1));}};";
    js += "fs.exists=function(Path){return __CFUNC__fs_exists(Path);};";
    js += "fs.isFile=function(Path){return __CFUNC__fs_isFile(Path);};";
    js += "fs.isDirectory=function(Path){return __CFUNC__fs_isDirectory(Path);};";
    js += "fs.readText=function(Path){return __CFUNC__fs_readText(Path);};";
    js += "fs.writeText=function(Path,Text){__CFUNC__fs_writeText(Path,Text);};";
    js += "fs.readConfigFile=function(Path,Section,Name){return __CFUNC__fs_readConfigFile(Path,Section,Name);};";
    return js;
}
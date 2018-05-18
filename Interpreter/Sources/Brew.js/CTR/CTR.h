#include "Includes.CTR.h"

namespace ctr
{

    // ctr.isNew3DS() -> boolean
    inline duk_ret_t CFUNC_isNew3DS(duk_context *ctx)
    {
        bool n3d;
	    APT_CheckNew3DS(&n3d);
	    duk_push_boolean(ctx, n3d);
        return 1;
    }

    // ctr.is2DS() -> boolean
    inline duk_ret_t CFUNC_is2DS(duk_context *ctx)
    {
        u8 val;
        cfguInit();
        CFGU_GetModelNintendo2DS(&val);
        cfguExit();
        bool is2ds = (val == 0);
        duk_push_boolean(ctx, is2ds);
        return 1;
    }

    // ctr.modelName() -> string
    inline duk_ret_t CFUNC_modelName(duk_context *ctx)
    {
        u8 val;
        cfguInit();
        CFGU_GetSystemModel(&val);
        cfguExit();
        string model;
        switch(val)
        {
            case 0:
                model = "3DS";
                break;
            case 1:
                model = "3DS XL";
                break;
            case 2:
                model = "New 3DS";
                break;
            case 3:
                model = "2DS";
                break;
            case 4:
                model = "New 3DS XL";
                break;
            case 5:
                model = "2DS XL";
                break;
            default:
                model = "Unknown";
                break;
        }
        duk_push_string(ctx, model.c_str());
        return 1;
    }

    namespace applet
    {
        // ctr.applet.openApplet(Applet)
        inline duk_ret_t CFUNC_openApplet(duk_context *ctx)
        {
            gfxFlushBuffers();
            gfxSwapBuffers();
            gspWaitForVBlank();
            int id = duk_to_number(ctx, 0);
            u32 buff[0x400 / 4];
            memset(buff, 0, sizeof(buff));
            if(!aptLaunchLibraryApplet((NS_APPID)id, buff, sizeof(buff), 0)) exit(0);
            return 0;
        }
    }
}

void CTX_CTR(duk_context *ctx)
{
    duk_push_c_function(ctx, ctr::CFUNC_isNew3DS, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__ctr_isNew3DS");
    duk_push_c_function(ctx, ctr::CFUNC_is2DS, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__ctr_is2DS");
    duk_push_c_function(ctx, ctr::CFUNC_modelName, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__ctr_modelName");
    duk_push_c_function(ctx, ctr::applet::CFUNC_openApplet, DUK_VARARGS);
	duk_put_global_string(ctx, "__CFUNC__ctr_applet_openApplet");
}

string JS_CTR()
{
    string js = "";
    js += "device.screen.Top=0;";
    js += "device.screen.Bottom=1;";
    js += "var ctr=[];";
    js += "ctr.applet=[];";
    js += "ctr.isNew3DS=function(){return __CFUNC__ctr_isNew3DS();};";
    js += "ctr.is2DS=function(){return __CFUNC__ctr_is2DS();};";
    js += "ctr.modelName=function(){return __CFUNC__ctr_modelName();};";
    js += "ctr.applet.HomeMenu=0x101;";
	js += "ctr.applet.AlternateMenu=0x103;";
	js += "ctr.applet.Camera=0x110;";
	js += "ctr.applet.FriendsList=0x112;";
	js += "ctr.applet.GameNotes=0x113;";
	js += "ctr.applet.WebBrowser=0x114;";
	js += "ctr.applet.InstructionManual=0x115;";
	js += "ctr.applet.Notifications=0x116;";
	js += "ctr.applet.Miiverse=0x117;";
	js += "ctr.applet.MiiversePosting=0x118;";
	js += "ctr.applet.AmiiboSettings=0x119;";
	js += "ctr.applet.Application=0x300;";
	js += "ctr.applet.eShop=0x301;";
	js += "ctr.applet.SoftwareKeyboard=0x401;";
	js += "ctr.applet.MiiSelector=0x402;";
	js += "ctr.applet.PhotoSelector=0x404;";
	js += "ctr.applet.SoundSelector=0x405;";
	js += "ctr.applet.ErrorDisplay=0x406;";
	js += "ctr.applet.eShopMint=0x407;";
	js += "ctr.applet.CirclePadCalibrator=0x408;";
	js += "ctr.applet.Notepad=0x409;";
    js += "ctr.applet.openApplet=function(Applet){__CFUNC__ctr_applet_openApplet(Applet);};";
    return js;
}

#include "duktape/duktape.h"
#include "Includes.h"

#include "Common.h"

void PreEvaluate();
void PostEvaluate();

duk_context *GenerateContext();
string JS_API();

inline duk_ret_t DUKT_Eval(duk_context *ctx, void *udata)
{
	(void) udata;
	duk_eval(ctx);
	return 1;
}

inline duk_ret_t DUKT_ToStr(duk_context *ctx, void *udata)
{
	(void) udata;
	duk_to_string(ctx, -1);
	return 1;
}

#if defined CTR

#include "CTR/Includes.CTR.h"
#include "CTR/Common.CTR.h"
#include "CTR/CTR.h"

void PreEvaluate()
{
    romfsInit();
    gfxInitDefault();
}

void PostEvaluate()
{
    romfsExit();
    gfxExit();
}

duk_context *GenerateContext()
{
    duk_context *ctx = duk_create_heap_default();
    CTX_Common(ctx);
    CTX_CommonCTR(ctx);
    CTX_CTR(ctx);
    return ctx;
}

string JS_API()
{
    string js = JS_Common();
    js += JS_CommonCTR();
    js += JS_CTR();
    js += "'Brew.js (CTR platform)';";
    return js;
}

#elif defined NTR

#include "NTR/Includes.NTR.h"
#include "NTR/Common.NTR.h"
#include "NTR/NTR.h"

void PreEvaluate()
{
    videoSetMode(MODE_0_2D);
    oamInit(&oamMain, SpriteMapping_Bmp_1D_128, false);
    vramSetBankB(VRAM_B_MAIN_SPRITE);
    videoSetModeSub(MODE_0_2D);
    oamInit(&oamSub, SpriteMapping_Bmp_1D_128, false);
    vramSetBankD(VRAM_D_SUB_SPRITE);
}

void PostEvaluate()
{
    // no post-evaluate on NTR
}

duk_context *GenerateContext()
{
    duk_context *ctx = duk_create_heap_default();
    CTX_Common(ctx);
    CTX_CommonNTR(ctx);
    CTX_NTR(ctx);
    return ctx;
}

string JS_API()
{
    string js = JS_Common();
    js += JS_CommonNTR();
    js += JS_NTR();
    js += "'Brew.js (NTR platform)';";
    return js;
}

#elif defined NX

#include "NX/Includes.NX.h"
#include "NX/Graphics.NX.h"
#include "NX/Common.NX.h"
#include "NX/NX.h"

void PreEvaluate()
{
    gfxInitDefault();
    srand(time(NULL));
    for(int a = 0; a < 1280; a++)
    {
        for(int b = 0; b < 720; b++)
        {
            DrawPixel(a, b, MakeColor(255, 255, 255, 255));
        }
    }
}

void PostEvaluate()
{
    gfx::gfxblock = vector<gfx::Object>();
    gfx::txtblock = vector<gfx::Text>();
    gfx::bgnul = true;
}

duk_context *GenerateContext()
{
    duk_context *ctx = duk_create_heap_default();
    CTX_Common(ctx);
    CTX_CommonNX(ctx);
    CTX_GraphicsNX(ctx);
    CTX_NX(ctx);
    return ctx;
}

string JS_API()
{
    string js = JS_Common();
    js += JS_CommonNX();
    js += JS_GraphicsNX();
    js += JS_NX();
    js += "'Brew.js (NX platform)';";
    return js;
}

#elif defined DOL

#include "DOL/Includes.DOL.h"

static void *buff = NULL;
static GXRModeObj *rmode = NULL;

#include "DOL/Common.DOL.h"
#include "DOL/DOL.h"

void PreEvaluate()
{
	VIDEO_Init();
	PAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	buff = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(buff);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();
}

void PostEvaluate()
{
    // no post-evaluate on DOL
}

duk_context *GenerateContext()
{
    duk_context *ctx = duk_create_heap_default();
    CTX_Common(ctx);
    CTX_CommonDOL(ctx);
    CTX_DOL(ctx);
    return ctx;
}

string JS_API()
{
    string js = JS_Common();
    js += JS_CommonDOL();
    js += JS_DOL();
    js += "'Brew.js (DOL platform)';";
    return js;
}

#elif defined RVL

#include "RVL/Includes.RVL.h"

static void *buff = NULL;
static GXRModeObj *rmode = NULL;

#include "RVL/Common.RVL.h"
#include "RVL/RVL.h"

void PreEvaluate()
{
	VIDEO_Init();
	WPAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	buff = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(buff);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();
}

void PostEvaluate()
{
    // no post-evaluate on RVL
}

duk_context *GenerateContext()
{
    duk_context *ctx = duk_create_heap_default();
    CTX_Common(ctx);
    CTX_CommonRVL(ctx);
    CTX_RVL(ctx);
    return ctx;
}

string JS_API()
{
    string js = JS_Common();
    js += JS_CommonRVL();
    js += JS_RVL();
    js += "'Brew.js (RVL platform)';";
    return js;
}

#elif defined AGB

#include "AGB/Includes.AGB.h"
#include "AGB/Common.AGB.h"
#include "AGB/AGB.h"

void PreEvaluate()
{
    irqInit();
	irqEnable(IRQ_VBLANK);
}

void PostEvaluate()
{
    // no post-evaluate on AGB
}

duk_context *GenerateContext()
{
    duk_context *ctx = duk_create_heap_default();
    CTX_Common(ctx);
    CTX_CommonAGB(ctx);
    CTX_AGB(ctx);
    return ctx;
}

string JS_API()
{
    string js = JS_Common();
    js += JS_CommonAGB();
    js += JS_AGB();
    js += "'Brew.js (AGB platform)';";
    return js;
}

#else

#error "[Brew.js] A platform must be defined  --  Possible ones: CTR, NTR, NX, DOL, RVL, AGB  --  #define <platform>"

#endif

string Evaluate(string Code)
{
    string jscode = JS_API() + Code;
    duk_context *ctx = GenerateContext();
    duk_push_string(ctx, jscode.c_str());
    duk_safe_call(ctx, DUKT_Eval, NULL, 1, 1);
	duk_safe_call(ctx, DUKT_ToStr, NULL, 1, 1);
    string rst = string(duk_get_string(ctx, -1));
    duk_pop(ctx);
    return rst;
}
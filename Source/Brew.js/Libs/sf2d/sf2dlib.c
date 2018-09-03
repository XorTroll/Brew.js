#include <string.h>
#include "sf2d.h"
#include "shader_shbin.h"


static int sf2d_initialized = 0;
// Temporary memory pool
static void *pool_addr = NULL;
static u32 pool_index = 0;
static u32 pool_size = 0;
//VBlank wait
static int vblank_wait = 1;
//FPS calculation
static float current_fps = 0.0f;
static unsigned int frames = 0;
static u64 last_time = 0;
//Current screen/side
static gfxScreen_t cur_screen = GFX_TOP;
static gfx3dSide_t cur_side = GFX_LEFT;
//Shader stuff
static DVLB_s *dvlb = NULL;
static shaderProgram_s shader;
static int projection_desc = -1;
static int transform_desc = -1;
static int useTransform_desc = -1;
//Rendertarget things
static sf2d_rendertarget * targetTopLeft;
static sf2d_rendertarget * targetTopRight;
static sf2d_rendertarget * targetBottom;
static int in_render;

int sf2d_init()
{
	return sf2d_init_advanced(
		SF2D_GPUCMD_DEFAULT_SIZE,
		SF2D_TEMPPOOL_DEFAULT_SIZE);
}

int sf2d_init_advanced(int gpucmd_size, int temppool_size)
{
	if (sf2d_initialized) return 0;

	gfxInitDefault();
	gfxSet3D(false);
	C3D_Init(gpucmd_size*8);

	//Setup rendertargets
	targetTopLeft  = sf2d_create_rendertarget(400, 240);
	targetTopRight = sf2d_create_rendertarget(400, 240);
	targetBottom   = sf2d_create_rendertarget(320, 240);
	sf2d_set_clear_color(0);
	C3D_RenderTargetSetOutput(targetTopLeft->target,  GFX_TOP,    GFX_LEFT,  0x1000);
	C3D_RenderTargetSetOutput(targetTopRight->target, GFX_TOP,    GFX_RIGHT, 0x1000);
	C3D_RenderTargetSetOutput(targetBottom->target,   GFX_BOTTOM, GFX_LEFT,  0x1000);

	//Setup temp pool
	pool_addr = linearAlloc(temppool_size);
	pool_size = temppool_size;

	//Setup the shader
	dvlb = DVLB_ParseFile((u32 *)shader_shbin, shader_shbin_size);
	shaderProgramInit(&shader);
	shaderProgramSetVsh(&shader, &dvlb->DVLE[0]);

	//Get shader uniform descriptors
	projection_desc = shaderInstanceGetUniformLocation(shader.vertexShader, "projection");
	transform_desc = shaderInstanceGetUniformLocation(shader.vertexShader, "transform");
	useTransform_desc = shaderInstanceGetUniformLocation(shader.vertexShader, "useTransform");

	C3D_BindProgram(&shader);
	C3D_CullFace(GPU_CULL_NONE);
	C3D_DepthTest(true, GPU_GEQUAL, GPU_WRITE_ALL);
	C3D_BoolUnifSet(GPU_VERTEX_SHADER, useTransform_desc, false);

	vblank_wait = 1;
	current_fps = 0.0f;
	frames = 0;
	last_time = osGetTime();

	sf2d_pool_reset();

	sf2d_initialized = 1;

	return 1;
}

int sf2d_fini()
{
	if (!sf2d_initialized) return 0;

	linearFree(pool_addr);

	shaderProgramFree(&shader);
	DVLB_Free(dvlb);
	C3D_Fini();
	gfxExit();

	sf2d_initialized = 0;

	return 1;
}

void sf2d_set_3D(int enable)
{
	gfxSet3D(enable);
}

void sf2d_set_transform(C3D_Mtx* mtx)
{
	C3D_BoolUnifSet(GPU_VERTEX_SHADER, useTransform_desc, mtx != NULL);
	if (mtx) {
		C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, transform_desc, mtx);
	}
}

void sf2d_start_frame(gfxScreen_t screen, gfx3dSide_t side)
{
	cur_screen = screen;
	cur_side = side;

	if (screen == GFX_TOP) {
		if (side == GFX_LEFT) {
			sf2d_start_frame_target(targetTopLeft);
		} else {
			sf2d_start_frame_target(targetTopRight);
		}
	} else {
		sf2d_start_frame_target(targetBottom);
	}
}

void sf2d_start_frame_target(sf2d_rendertarget *target)
{
	if (!in_render) {
		sf2d_pool_reset();
		C3D_FrameBegin(vblank_wait ? C3D_FRAME_SYNCDRAW : 0);
		in_render = 1;
	}

	C3D_FrameDrawOn(target->target);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, projection_desc, &target->projection);
}

void sf2d_end_frame()
{
	// Nothing
}

void sf2d_swapbuffers()
{
	if (!in_render) return;

	in_render = 0;
	C3D_FrameEnd(0);

	//Calculate FPS
	frames++;
	u64 delta_time = osGetTime() - last_time;
	if (delta_time >= 1000) {
		current_fps = frames/(delta_time/1000.0f);
		frames = 0;
		last_time = osGetTime();
	}
}

void sf2d_set_vblank_wait(int enable)
{
	vblank_wait = enable;
}

float sf2d_get_fps()
{
	return current_fps;
}

void *sf2d_pool_malloc(u32 size)
{
	if ((pool_index + size) < pool_size) {
		void *addr = (void *)((u32)pool_addr + pool_index);
		pool_index += size;
		return addr;
	}
	return NULL;
}

void *sf2d_pool_memalign(u32 size, u32 alignment)
{
	u32 new_index = (pool_index + alignment - 1) & ~(alignment - 1);
	if ((new_index + size) < pool_size) {
		void *addr = (void *)((u32)pool_addr + new_index);
		pool_index = new_index + size;
		return addr;
	}
	return NULL;
}

void *sf2d_pool_calloc(u32 nmemb, u32 size)
{
	return sf2d_pool_memalign(nmemb * size, size);
}

unsigned int sf2d_pool_space_free()
{
	return pool_size - pool_index;
}

void sf2d_pool_reset()
{
	pool_index = 0;
}

void sf2d_set_clear_color(u32 color)
{
	sf2d_clear_target(targetTopLeft,  color);
	sf2d_clear_target(targetTopRight, color);
	sf2d_clear_target(targetBottom,   color);
}

void sf2d_set_scissor_test(GPU_SCISSORMODE mode, u32 x, u32 y, u32 w, u32 h)
{
	if (cur_screen == GFX_TOP) {
		C3D_SetScissor(mode, 240 - (y + h), 400 - (x + w), 240 - y, 400 - x);
	} else {
		C3D_SetScissor(mode, 240 - (y + h), 320 - (x + w), 240 - y, 320 - x);
	}
}

gfxScreen_t sf2d_get_current_screen()
{
	return cur_screen;
}

gfx3dSide_t sf2d_get_current_side()
{
	return cur_side;
}

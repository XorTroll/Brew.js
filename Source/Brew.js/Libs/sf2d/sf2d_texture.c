#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sf2d.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define TEX_MIN_SIZE 64

//Grabbed from: http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
unsigned int next_pow2(unsigned int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v >= TEX_MIN_SIZE ? v : TEX_MIN_SIZE;
}

sf2d_texture *sf2d_create_texture(int width, int height, sf2d_texfmt pixel_format, sf2d_place place)
{
	sf2d_texture *texture = calloc(1, sizeof(*texture));
	if (!texture) return NULL;

	bool success = false;
	texture->width = width;
	texture->height = height;
	if (place == SF2D_PLACE_RAM) {
		success = C3D_TexInit(&texture->tex, next_pow2(width), next_pow2(height), pixel_format);
	} else if (place == SF2D_PLACE_VRAM) {
		success = C3D_TexInitVRAM(&texture->tex, next_pow2(width), next_pow2(height), pixel_format);
	}

	if (!success) {
		free(texture);
		return NULL;
	}

	C3D_TexSetWrap(&texture->tex, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);
	return texture;
}

sf2d_rendertarget *sf2d_create_rendertarget(int width, int height)
{
	sf2d_rendertarget *rt = calloc(1, sizeof(*rt));
	if (!rt) return NULL;

	rt->target = C3D_RenderTargetCreate(height, width, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	if (!rt->target)
	{
		free(rt);
		return NULL;
	}

	Mtx_OrthoTilt(&rt->projection, 0.0f, width, height, 0.0f, 0.0f, 1.0f, true);
	return rt;
}

void sf2d_free_texture(sf2d_texture *texture)
{
	if (texture) {
		C3D_TexDelete(&texture->tex);
		free(texture);
	}
}

void sf2d_free_target(sf2d_rendertarget *target)
{
	C3D_RenderTargetDelete(target->target);
	free(target);
}

void sf2d_clear_target(sf2d_rendertarget *target, u32 color)
{
	color = ((color>>24)&0x000000FF) | ((color>>8)&0x0000FF00) | ((color<<8)&0x00FF0000) | ((color<<24)&0xFF000000); // reverse byte order
	C3D_RenderTargetSetClear(target->target, C3D_CLEAR_ALL, color, 0);
}

void sf2d_texture_tile32_hardware(sf2d_texture *texture, const void *data, int w, int h)
{
	if (texture->tiled) return;

	const u32 flags = (GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) |
		GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) |
		GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));

	GSPGPU_FlushDataCache(data, (w*h)<<2);
	GSPGPU_FlushDataCache(texture->tex.data, texture->tex.size);

	C3D_SafeDisplayTransfer(
		(u32*)data,
		GX_BUFFER_DIM(w, h),
		(u32*)texture->tex.data,
		GX_BUFFER_DIM(texture->tex.width, texture->tex.height),
		flags
	);

	gspWaitForPPF();
	texture->tiled = 1;
}

void sf2d_fill_texture_from_RGBA8(sf2d_texture *dst, const void *rgba8, int source_w, int source_h)
{
	// TODO: add support for non-RGBA8 textures

	u8 *tmp = linearAlloc((dst->tex.width * dst->tex.height)<<2);
	int i, j;
	for (i = 0; i < source_h; i++) {
		for (j = 0; j < source_w; j++) {
			((u32 *)tmp)[i*dst->tex.width + j] = __builtin_bswap32(((u32 *)rgba8)[i*source_w + j]);
		}
	}
	sf2d_texture_tile32_hardware(dst, tmp, dst->tex.width, dst->tex.height);
	linearFree(tmp);

}

sf2d_texture *sf2d_create_texture_mem_RGBA8(const void *src_buffer, int src_w, int src_h, sf2d_texfmt pixel_format, sf2d_place place)
{
	sf2d_texture *tex = sf2d_create_texture(src_w, src_h, pixel_format, place);
	if (tex == NULL) return NULL;
	sf2d_fill_texture_from_RGBA8(tex, src_buffer, src_w, src_h);
	return tex;
}

static inline int sf2d_get_texunit(GPU_TEXUNIT unit)
{
	switch (unit) {
		case GPU_TEXUNIT0: return 0;
		case GPU_TEXUNIT1: return 1;
		case GPU_TEXUNIT2: return 2;
		default:           return -1;
	}
}

void sf2d_bind_texture(const sf2d_texture *texture, GPU_TEXUNIT unit)
{
	C3D_TexBind(sf2d_get_texunit(unit), (C3D_Tex*)&texture->tex);

	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, 0, 0);
	C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
}

void sf2d_bind_texture_color(const sf2d_texture *texture, GPU_TEXUNIT unit, u32 color)
{
	C3D_TexBind(sf2d_get_texunit(unit), (C3D_Tex*)&texture->tex);

	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_CONSTANT, 0);
	C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);
	C3D_TexEnvColor(env, color);
}

void sf2d_texture_set_params(sf2d_texture *texture, u32 params)
{
	texture->tex.param = params;
}

int sf2d_texture_get_params(const sf2d_texture *texture)
{
	return texture->tex.param;
}

static inline void sf2d_draw_texture_generic(const sf2d_texture *texture, int x, int y)
{
	sf2d_vertex_pos_tex *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_tex), 8);
	if (!vertices) return;

	int w = texture->width;
	int h = texture->height;

	vertices[0].position = (sf2d_vector_3f){(float)x,   (float)y,   SF2D_DEFAULT_DEPTH};
	vertices[1].position = (sf2d_vector_3f){(float)x+w, (float)y,   SF2D_DEFAULT_DEPTH};
	vertices[2].position = (sf2d_vector_3f){(float)x,   (float)y+h, SF2D_DEFAULT_DEPTH};
	vertices[3].position = (sf2d_vector_3f){(float)x+w, (float)y+h, SF2D_DEFAULT_DEPTH};

	float u = texture->width/(float)texture->tex.width;
	float v = texture->height/(float)texture->tex.height;

	vertices[0].texcoord = (sf2d_vector_2f){0.0f, 0.0f};
	vertices[1].texcoord = (sf2d_vector_2f){u,    0.0f};
	vertices[2].texcoord = (sf2d_vector_2f){0.0f, v};
	vertices[3].texcoord = (sf2d_vector_2f){u,    v};

	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2);

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vertices, sizeof(sf2d_vertex_pos_tex), 2, 0x10);

	C3D_DrawArrays(GPU_TRIANGLE_STRIP, 0, 4);
}

void sf2d_draw_texture(const sf2d_texture *texture, int x, int y)
{
	sf2d_bind_texture(texture, GPU_TEXUNIT0);
	sf2d_draw_texture_generic(texture, x, y);
}

void sf2d_draw_texture_blend(const sf2d_texture *texture, int x, int y, u32 color)
{
	sf2d_bind_texture_color(texture, GPU_TEXUNIT0, color);
	sf2d_draw_texture_generic(texture, x, y);
}

static inline void sf2d_draw_texture_rotate_hotspot_generic(const sf2d_texture *texture, int x, int y, float rad, float center_x, float center_y)
{
	sf2d_vertex_pos_tex *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_tex), 8);
	if (!vertices) return;

	const float w = texture->width;
	const float h = texture->height;

	vertices[0].position.x = -center_x;
	vertices[0].position.y = -center_y;
	vertices[0].position.z = SF2D_DEFAULT_DEPTH;

	vertices[1].position.x = w - center_x;
	vertices[1].position.y = -center_y;
	vertices[1].position.z = SF2D_DEFAULT_DEPTH;

	vertices[2].position.x = -center_x;
	vertices[2].position.y = h - center_y;
	vertices[2].position.z = SF2D_DEFAULT_DEPTH;

	vertices[3].position.x = w - center_x;
	vertices[3].position.y = h - center_y;
	vertices[3].position.z = SF2D_DEFAULT_DEPTH;

	float u = w/(float)texture->tex.width;
	float v = h/(float)texture->tex.height;

	vertices[0].texcoord = (sf2d_vector_2f){0.0f, 0.0f};
	vertices[1].texcoord = (sf2d_vector_2f){u,    0.0f};
	vertices[2].texcoord = (sf2d_vector_2f){0.0f, v};
	vertices[3].texcoord = (sf2d_vector_2f){u,    v};

	const float c = cosf(rad);
	const float s = sinf(rad);
	int i;
	for (i = 0; i < 4; ++i) { // Rotate and translate
		float _x = vertices[i].position.x;
		float _y = vertices[i].position.y;
		vertices[i].position.x = _x*c - _y*s + x;
		vertices[i].position.y = _x*s + _y*c + y;
	}

	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2);

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vertices, sizeof(sf2d_vertex_pos_tex), 2, 0x10);

	C3D_DrawArrays(GPU_TRIANGLE_STRIP, 0, 4);
}

void sf2d_draw_texture_rotate_hotspot(const sf2d_texture *texture, int x, int y, float rad, float center_x, float center_y)
{
	sf2d_bind_texture(texture, GPU_TEXUNIT0);
	sf2d_draw_texture_rotate_hotspot_generic(texture, x, y, rad, center_x, center_y);
}

void sf2d_draw_texture_rotate_hotspot_blend(const sf2d_texture *texture, int x, int y, float rad, float center_x, float center_y, u32 color)
{
	sf2d_bind_texture_color(texture, GPU_TEXUNIT0, color);
	sf2d_draw_texture_rotate_hotspot_generic(texture, x, y, rad, center_x, center_y);
}

void sf2d_draw_texture_rotate(const sf2d_texture *texture, int x, int y, float rad)
{
	sf2d_draw_texture_rotate_hotspot(texture, x, y, rad,
		texture->width/2.0f,
		texture->height/2.0f);
}

void sf2d_draw_texture_rotate_blend(const sf2d_texture *texture, int x, int y, float rad, u32 color)
{
	sf2d_draw_texture_rotate_hotspot_blend(texture, x, y, rad,
		texture->width/2.0f,
		texture->height/2.0f,
		color);
}

static inline void sf2d_draw_texture_rotate_scale_hotspot_generic(const sf2d_texture *texture, int x, int y, float rad, float scale_x, float scale_y, float center_x, float center_y)
{
	sf2d_vertex_pos_tex *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_tex), 8);
	if (!vertices) return;

	const float w = texture->width;
	const float h = texture->height;
    
    vertices[0].position.x = -center_x * scale_x;
	vertices[0].position.y = -center_y * scale_y;
	vertices[0].position.z = SF2D_DEFAULT_DEPTH;

	vertices[1].position.x = (w - center_x) * scale_x;
	vertices[1].position.y = -center_y * scale_y;
	vertices[1].position.z = SF2D_DEFAULT_DEPTH;

	vertices[2].position.x = -center_x * scale_x;
	vertices[2].position.y = (h - center_y) * scale_y;
	vertices[2].position.z = SF2D_DEFAULT_DEPTH;

	vertices[3].position.x = (w - center_x) * scale_x;
	vertices[3].position.y = h - center_y * scale_y;
	vertices[3].position.z = SF2D_DEFAULT_DEPTH;

	float u = w/(float)texture->tex.width;
	float v = h/(float)texture->tex.height;

	vertices[0].texcoord = (sf2d_vector_2f){0.0f, 0.0f};
	vertices[1].texcoord = (sf2d_vector_2f){u,    0.0f};
	vertices[2].texcoord = (sf2d_vector_2f){0.0f, v};
	vertices[3].texcoord = (sf2d_vector_2f){u,    v};

	const float c = cosf(rad);
	const float s = sinf(rad);
	int i;
	for (i = 0; i < 4; ++i) { // Rotate and translate
		float _x = vertices[i].position.x;
		float _y = vertices[i].position.y;
		vertices[i].position.x = _x*c - _y*s + x;
		vertices[i].position.y = _x*s + _y*c + y;
	}

	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2);

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vertices, sizeof(sf2d_vertex_pos_tex), 2, 0x10);

	C3D_DrawArrays(GPU_TRIANGLE_STRIP, 0, 4);
}

void sf2d_draw_texture_rotate_scale_hotspot(const sf2d_texture *texture, int x, int y, float rad, float scale_x, float scale_y, float center_x, float center_y)
{
    sf2d_bind_texture(texture, GPU_TEXUNIT0);
	sf2d_draw_texture_rotate_scale_hotspot_generic(texture, x, y, rad, scale_x, scale_y, center_x, center_y);
}

void sf2d_draw_texture_rotate_scale_hotspot_blend(const sf2d_texture *texture, int x, int y, float rad, float scale_x, float scale_y, float center_x, float center_y, u32 color)
{
    sf2d_bind_texture_color(texture, GPU_TEXUNIT0, color);
	sf2d_draw_texture_rotate_scale_hotspot_generic(texture, x, y, rad, scale_x, scale_y, center_x, center_y);
}

static inline void sf2d_draw_texture_part_generic(const sf2d_texture *texture, int x, int y, int tex_x, int tex_y, int tex_w, int tex_h)
{
	sf2d_vertex_pos_tex *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_tex), 8);
	if (!vertices) return;

	vertices[0].position = (sf2d_vector_3f){(float)x,       (float)y,       SF2D_DEFAULT_DEPTH};
	vertices[1].position = (sf2d_vector_3f){(float)x+tex_w, (float)y,       SF2D_DEFAULT_DEPTH};
	vertices[2].position = (sf2d_vector_3f){(float)x,       (float)y+tex_h, SF2D_DEFAULT_DEPTH};
	vertices[3].position = (sf2d_vector_3f){(float)x+tex_w, (float)y+tex_h, SF2D_DEFAULT_DEPTH};

	float u0 = tex_x/(float)texture->tex.width;
	float v0 = tex_y/(float)texture->tex.height;
	float u1 = (tex_x+tex_w)/(float)texture->tex.width;
	float v1 = (tex_y+tex_h)/(float)texture->tex.height;

	vertices[0].texcoord = (sf2d_vector_2f){u0, v0};
	vertices[1].texcoord = (sf2d_vector_2f){u1, v0};
	vertices[2].texcoord = (sf2d_vector_2f){u0, v1};
	vertices[3].texcoord = (sf2d_vector_2f){u1, v1};

	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2);

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vertices, sizeof(sf2d_vertex_pos_tex), 2, 0x10);

	C3D_DrawArrays(GPU_TRIANGLE_STRIP, 0, 4);
}

void sf2d_draw_texture_part(const sf2d_texture *texture, int x, int y, int tex_x, int tex_y, int tex_w, int tex_h)
{
	sf2d_bind_texture(texture, GPU_TEXUNIT0);
	sf2d_draw_texture_part_generic(texture, x, y, tex_x, tex_y, tex_w, tex_h);
}

void sf2d_draw_texture_part_blend(const sf2d_texture *texture, int x, int y, int tex_x, int tex_y, int tex_w, int tex_h, u32 color)
{
	sf2d_bind_texture_color(texture, GPU_TEXUNIT0, color);
	sf2d_draw_texture_part_generic(texture, x, y, tex_x, tex_y, tex_w, tex_h);
}

static inline void sf2d_draw_texture_scale_generic(const sf2d_texture *texture, int x, int y, float x_scale, float y_scale)
{
	sf2d_vertex_pos_tex *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_tex), 8);
	if (!vertices) return;

	int ws = texture->width * x_scale;
	int hs = texture->height * y_scale;

	vertices[0].position = (sf2d_vector_3f){(float)x,    (float)y,    SF2D_DEFAULT_DEPTH};
	vertices[1].position = (sf2d_vector_3f){(float)x+ws, (float)y,    SF2D_DEFAULT_DEPTH};
	vertices[2].position = (sf2d_vector_3f){(float)x,    (float)y+hs, SF2D_DEFAULT_DEPTH};
	vertices[3].position = (sf2d_vector_3f){(float)x+ws, (float)y+hs, SF2D_DEFAULT_DEPTH};

	float u = texture->width/(float)texture->tex.width;
	float v = texture->height/(float)texture->tex.height;

	vertices[0].texcoord = (sf2d_vector_2f){0.0f, 0.0f};
	vertices[1].texcoord = (sf2d_vector_2f){u,    0.0f};
	vertices[2].texcoord = (sf2d_vector_2f){0.0f, v};
	vertices[3].texcoord = (sf2d_vector_2f){u,    v};

	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2);

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vertices, sizeof(sf2d_vertex_pos_tex), 2, 0x10);

	C3D_DrawArrays(GPU_TRIANGLE_STRIP, 0, 4);
}

void sf2d_draw_texture_scale(const sf2d_texture *texture, int x, int y, float x_scale, float y_scale)
{
	sf2d_bind_texture(texture, GPU_TEXUNIT0);
	sf2d_draw_texture_scale_generic(texture, x, y, x_scale, y_scale);
}

void sf2d_draw_texture_scale_blend(const sf2d_texture *texture, int x, int y, float x_scale, float y_scale, u32 color)
{
	sf2d_bind_texture_color(texture, GPU_TEXUNIT0, color);
	sf2d_draw_texture_scale_generic(texture, x, y, x_scale, y_scale);
}

static inline void sf2d_draw_texture_part_scale_generic(const sf2d_texture *texture, float x, float y, float tex_x, float tex_y, float tex_w, float tex_h, float x_scale, float y_scale)
{
	sf2d_vertex_pos_tex *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_tex), 8);
	if (!vertices) return;

	float u0 = tex_x/(float)texture->tex.width;
	float v0 = tex_y/(float)texture->tex.height;
	float u1 = (tex_x+tex_w)/(float)texture->tex.width;
	float v1 = (tex_y+tex_h)/(float)texture->tex.height;

	vertices[0].texcoord = (sf2d_vector_2f){u0, v0};
	vertices[1].texcoord = (sf2d_vector_2f){u1, v0};
	vertices[2].texcoord = (sf2d_vector_2f){u0, v1};
	vertices[3].texcoord = (sf2d_vector_2f){u1, v1};

	tex_w *= x_scale;
	tex_h *= y_scale;

	vertices[0].position = (sf2d_vector_3f){(float)x,       (float)y,       SF2D_DEFAULT_DEPTH};
	vertices[1].position = (sf2d_vector_3f){(float)x+tex_w, (float)y,       SF2D_DEFAULT_DEPTH};
	vertices[2].position = (sf2d_vector_3f){(float)x,       (float)y+tex_h, SF2D_DEFAULT_DEPTH};
	vertices[3].position = (sf2d_vector_3f){(float)x+tex_w, (float)y+tex_h, SF2D_DEFAULT_DEPTH};

	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2);

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vertices, sizeof(sf2d_vertex_pos_tex), 2, 0x10);

	C3D_DrawArrays(GPU_TRIANGLE_STRIP, 0, 4);
}

void sf2d_draw_texture_part_scale(const sf2d_texture *texture, float x, float y, float tex_x, float tex_y, float tex_w, float tex_h, float x_scale, float y_scale)
{
	sf2d_bind_texture(texture, GPU_TEXUNIT0);
	sf2d_draw_texture_part_scale_generic(texture, x, y, tex_x, tex_y, tex_w, tex_h, x_scale, y_scale);
}

void sf2d_draw_texture_part_scale_blend(const sf2d_texture *texture, float x, float y, float tex_x, float tex_y, float tex_w, float tex_h, float x_scale, float y_scale, u32 color)
{
	sf2d_bind_texture_color(texture, GPU_TEXUNIT0, color);
	sf2d_draw_texture_part_scale_generic(texture, x, y, tex_x, tex_y, tex_w, tex_h, x_scale, y_scale);
}

static inline void sf2d_draw_texture_part_rotate_scale_generic(const sf2d_texture *texture, int x, int y, float rad, int tex_x, int tex_y, int tex_w, int tex_h, float x_scale, float y_scale)
{
	sf2d_vertex_pos_tex *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_tex), 8);
	if (!vertices) return;

	int w2 = (tex_w * x_scale)/2.0f;
	int h2 = (tex_h * y_scale)/2.0f;

	vertices[0].position = (sf2d_vector_3f){(float)-w2, (float)-h2, SF2D_DEFAULT_DEPTH};
	vertices[1].position = (sf2d_vector_3f){(float) w2, (float)-h2, SF2D_DEFAULT_DEPTH};
	vertices[2].position = (sf2d_vector_3f){(float)-w2, (float) h2, SF2D_DEFAULT_DEPTH};
	vertices[3].position = (sf2d_vector_3f){(float) w2, (float) h2, SF2D_DEFAULT_DEPTH};

	float u0 = tex_x/(float)texture->tex.width;
	float v0 = tex_y/(float)texture->tex.height;
	float u1 = (tex_x+tex_w)/(float)texture->tex.width;
	float v1 = (tex_y+tex_h)/(float)texture->tex.height;

	vertices[0].texcoord = (sf2d_vector_2f){u0, v0};
	vertices[1].texcoord = (sf2d_vector_2f){u1, v0};
	vertices[2].texcoord = (sf2d_vector_2f){u0, v1};
	vertices[3].texcoord = (sf2d_vector_2f){u1, v1};

	const float c = cosf(rad);
	const float s = sinf(rad);
	int i;
	for (i = 0; i < 4; ++i) { // Rotate and translate
		float _x = vertices[i].position.x;
		float _y = vertices[i].position.y;
		vertices[i].position.x = _x*c - _y*s + x;
		vertices[i].position.y = _x*s + _y*c + y;
	}

	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2);

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vertices, sizeof(sf2d_vertex_pos_tex), 2, 0x10);

	C3D_DrawArrays(GPU_TRIANGLE_STRIP, 0, 4);
}

void sf2d_draw_texture_part_rotate_scale(const sf2d_texture *texture, int x, int y, float rad, int tex_x, int tex_y, int tex_w, int tex_h, float x_scale, float y_scale)
{
	sf2d_bind_texture(texture, GPU_TEXUNIT0);
	sf2d_draw_texture_part_rotate_scale_generic(texture, x, y, rad, tex_x, tex_y, tex_w, tex_h, x_scale, y_scale);
}

void sf2d_draw_texture_part_rotate_scale_blend(const sf2d_texture *texture, int x, int y, float rad, int tex_x, int tex_y, int tex_w, int tex_h, float x_scale, float y_scale, u32 color)
{
	sf2d_bind_texture_color(texture, GPU_TEXUNIT0, color);
	sf2d_draw_texture_part_rotate_scale_generic(texture, x, y, rad, tex_x, tex_y, tex_w, tex_h, x_scale, y_scale);
}

static inline void sf2d_draw_texture_depth_generic(const sf2d_texture *texture, int x, int y, signed short z)
{
	sf2d_vertex_pos_tex *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_tex), 8);
	if (!vertices) return;

	int w = texture->width;
	int h = texture->height;
	float depth = z/32768.0f + 0.5f;

	vertices[0].position = (sf2d_vector_3f){(float)x,   (float)y,   depth};
	vertices[1].position = (sf2d_vector_3f){(float)x+w, (float)y,   depth};
	vertices[2].position = (sf2d_vector_3f){(float)x,   (float)y+h, depth};
	vertices[3].position = (sf2d_vector_3f){(float)x+w, (float)y+h, depth};

	float u = texture->width/(float)texture->tex.width;
	float v = texture->height/(float)texture->tex.height;

	vertices[0].texcoord = (sf2d_vector_2f){0.0f, 0.0f};
	vertices[1].texcoord = (sf2d_vector_2f){u,    0.0f};
	vertices[2].texcoord = (sf2d_vector_2f){0.0f, v};
	vertices[3].texcoord = (sf2d_vector_2f){u,    v};

	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2);

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vertices, sizeof(sf2d_vertex_pos_tex), 2, 0x10);

	C3D_DrawArrays(GPU_TRIANGLE_STRIP, 0, 4);
}

void sf2d_draw_texture_depth(const sf2d_texture *texture, int x, int y, signed short z)
{
	sf2d_bind_texture(texture, GPU_TEXUNIT0);
	sf2d_draw_texture_depth_generic(texture, x, y, z);
}

void sf2d_draw_texture_depth_blend(const sf2d_texture *texture, int x, int y, signed short z, u32 color)
{
	sf2d_bind_texture_color(texture, GPU_TEXUNIT0, color);
	sf2d_draw_texture_depth_generic(texture, x, y, z);
}

void sf2d_draw_quad_uv_current(float left, float top, float right, float bottom, float u0, float v0, float u1, float v1)
{
	sf2d_vertex_pos_tex *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_tex), 8);
	if (!vertices) return;

	vertices[0].position = (sf2d_vector_3f){left,  top,    SF2D_DEFAULT_DEPTH};
	vertices[1].position = (sf2d_vector_3f){right, top,    SF2D_DEFAULT_DEPTH};
	vertices[2].position = (sf2d_vector_3f){left,  bottom, SF2D_DEFAULT_DEPTH};
	vertices[3].position = (sf2d_vector_3f){right, bottom, SF2D_DEFAULT_DEPTH};

	vertices[0].texcoord = (sf2d_vector_2f){u0, v0};
	vertices[1].texcoord = (sf2d_vector_2f){u1, v0};
	vertices[2].texcoord = (sf2d_vector_2f){u0, v1};
	vertices[3].texcoord = (sf2d_vector_2f){u1, v1};

	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2);

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vertices, sizeof(sf2d_vertex_pos_tex), 2, 0x10);

	C3D_DrawArrays(GPU_TRIANGLE_STRIP, 0, 4);
}

void sf2d_draw_quad_uv(const sf2d_texture *texture, float left, float top, float right, float bottom, float u0, float v0, float u1, float v1)
{
	sf2d_bind_texture(texture, GPU_TEXUNIT0);
	sf2d_draw_quad_uv_current(left, top, right, bottom, u0, v0, u1, v1);
}

void sf2d_draw_quad_uv_blend(const sf2d_texture *texture, float left, float top, float right, float bottom, float u0, float v0, float u1, float v1, u32 color)
{
	sf2d_bind_texture_color(texture, GPU_TEXUNIT0, color);
	sf2d_draw_quad_uv_current(left, top, right, bottom, u0, v0, u1, v1);
}

// Grabbed from Citra Emulator (citra/src/video_core/utils.h)
static inline u32 morton_interleave(u32 x, u32 y)
{
	u32 i = (x & 7) | ((y & 7) << 8); // ---- -210
	i = (i ^ (i << 2)) & 0x1313;      // ---2 --10
	i = (i ^ (i << 1)) & 0x1515;      // ---2 -1-0
	i = (i | (i >> 7)) & 0x3F;
	return i;
}

//Grabbed from Citra Emulator (citra/src/video_core/utils.h)
static inline u32 get_morton_offset(u32 x, u32 y, u32 bytes_per_pixel)
{
    u32 i = morton_interleave(x, y);
    unsigned int offset = (x & ~7) * 8;
    return (i + offset) * bytes_per_pixel;
}

void sf2d_set_pixel(sf2d_texture *texture, int x, int y, u32 new_color)
{
	y = (texture->tex.height - 1 - y);
	if (texture->tiled) {
		u32 coarse_y = y & ~7;
		u32 offset = get_morton_offset(x, y, 4) + coarse_y * texture->tex.width * 4;
		*(u32 *)(texture->tex.data + offset) = new_color;
	} else {
		((u32 *)texture->tex.data)[x + y * texture->tex.width] = new_color;
	}
}

u32 sf2d_get_pixel(sf2d_texture *texture, int x, int y)
{
	y = (texture->tex.height - 1 - y);
	if (texture->tiled) {
		u32 coarse_y = y & ~7;
		u32 offset = get_morton_offset(x, y, 4) + coarse_y * texture->tex.width * 4;
		return *(u32 *)(texture->tex.data + offset);
	} else {
		return ((u32 *)texture->tex.data)[x + y * texture->tex.width];
	}
}


void sf2d_texture_tile32(sf2d_texture *texture)
{
	if (texture->tiled) return;

	// TODO: add support for non-RGBA8 textures
	u8 *tmp = linearAlloc(texture->tex.width * texture->tex.height * 4);

	int i, j;
	for (j = 0; j < texture->tex.height; j++) {
		for (i = 0; i < texture->tex.width; i++) {

			u32 coarse_y = j & ~7;
			u32 dst_offset = get_morton_offset(i, j, 4) + coarse_y * texture->tex.width * 4;

			u32 v = ((u32 *)texture->tex.data)[i + (texture->tex.height - 1 - j)*texture->tex.width];
			*(u32 *)(tmp + dst_offset) = __builtin_bswap32(v); /* RGBA8 -> ABGR8 */
		}
	}

	memcpy(texture->tex.data, tmp, texture->tex.width*texture->tex.height*4);
	linearFree(tmp);

	texture->tiled = 1;
}

#include "sf2d.h"
#include <math.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

void sf2d_setup_env_internal(const sf2d_vertex_pos_col* vertices) {
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR, 0, 0);
	C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);
	AttrInfo_AddLoader(attrInfo, 1, GPU_UNSIGNED_BYTE, 4);

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vertices, sizeof(sf2d_vertex_pos_col), 2, 0x10);
}

void sf2d_draw_line(float x0, float y0, float x1, float y1, float width, u32 color)
{
	sf2d_vertex_pos_col *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_col), 8);
	if (!vertices) return;

    float dx = x1 - x0;
    float dy = y1 - y0;

    float nx = -dy;
    float ny = dx;

    float len = sqrt(nx * nx + ny * ny);

    if (len > 0 ){
        nx /= len;
        ny /= len;
    }

    nx *= width*0.5f;
    ny *= width*0.5f;

    vertices[0].position = (sf2d_vector_3f){x0+nx, y0+ny, SF2D_DEFAULT_DEPTH};
    vertices[1].position = (sf2d_vector_3f){x0-nx, y0-ny, SF2D_DEFAULT_DEPTH};

    vertices[2].position = (sf2d_vector_3f){x1+nx, y1+ny, SF2D_DEFAULT_DEPTH};
    vertices[3].position = (sf2d_vector_3f){x1-nx, y1-ny, SF2D_DEFAULT_DEPTH};

	vertices[0].color = color;
	vertices[1].color = vertices[0].color;
	vertices[2].color = vertices[0].color;
	vertices[3].color = vertices[0].color;

	sf2d_setup_env_internal(vertices);

	C3D_DrawArrays(GPU_TRIANGLE_STRIP, 0, 4);
}

void sf2d_draw_rectangle_internal(const sf2d_vertex_pos_col *vertices)
{
    sf2d_setup_env_internal(vertices);

	C3D_DrawArrays(GPU_TRIANGLE_STRIP, 0, 4);
}

void sf2d_draw_triangle_internal(const sf2d_vertex_pos_col *vertices)
{
    sf2d_setup_env_internal(vertices);

	C3D_DrawArrays(GPU_TRIANGLES, 0, 3);
}

void sf2d_draw_rectangle(int x, int y, int w, int h, u32 color)
{
	sf2d_vertex_pos_col *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_col), 8);
	if (!vertices) return;

	vertices[0].position = (sf2d_vector_3f){(float)x,   (float)y,   SF2D_DEFAULT_DEPTH};
	vertices[1].position = (sf2d_vector_3f){(float)x+w, (float)y,   SF2D_DEFAULT_DEPTH};
	vertices[2].position = (sf2d_vector_3f){(float)x,   (float)y+h, SF2D_DEFAULT_DEPTH};
	vertices[3].position = (sf2d_vector_3f){(float)x+w, (float)y+h, SF2D_DEFAULT_DEPTH};

	vertices[0].color = color;
	vertices[1].color = vertices[0].color;
	vertices[2].color = vertices[0].color;
	vertices[3].color = vertices[0].color;

	sf2d_draw_rectangle_internal(vertices);
}

void sf2d_draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, u32 color)
{
	sf2d_vertex_pos_col *vertices = sf2d_pool_memalign(3 * sizeof(sf2d_vertex_pos_col), 8);
	if (!vertices) return;

	vertices[0].position = (sf2d_vector_3f){(float)x1,   (float)y1,   SF2D_DEFAULT_DEPTH};
	vertices[1].position = (sf2d_vector_3f){(float)x2, (float)y2,   SF2D_DEFAULT_DEPTH};
	vertices[2].position = (sf2d_vector_3f){(float)x3,   (float)y3, SF2D_DEFAULT_DEPTH};

	vertices[0].color = color;
	vertices[1].color = vertices[0].color;
	vertices[2].color = vertices[0].color;

	sf2d_draw_triangle_internal(vertices);
}

void sf2d_draw_rectangle_rotate(int x, int y, int w, int h, u32 color, float rad)
{
	sf2d_vertex_pos_col *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_col), 8);
	if (!vertices) return;

	int w2 = w/2.0f;
	int h2 = h/2.0f;

	vertices[0].position = (sf2d_vector_3f){(float)-w2, (float)-h2, SF2D_DEFAULT_DEPTH};
	vertices[1].position = (sf2d_vector_3f){(float) w2, (float)-h2, SF2D_DEFAULT_DEPTH};
	vertices[2].position = (sf2d_vector_3f){(float)-w2, (float) h2, SF2D_DEFAULT_DEPTH};
	vertices[3].position = (sf2d_vector_3f){(float) w2, (float) h2, SF2D_DEFAULT_DEPTH};

	vertices[0].color = color;
	vertices[1].color = vertices[0].color;
	vertices[2].color = vertices[0].color;
	vertices[3].color = vertices[0].color;

	C3D_Mtx m;
	Mtx_Identity(&m);
	Mtx_Translate(&m, x+w2, y+h2, 0, true);
	Mtx_RotateZ(&m, rad, true);

	sf2d_set_transform(&m);
	sf2d_draw_rectangle_internal(vertices);
	sf2d_set_transform(NULL);
}

void sf2d_draw_rectangle_gradient(int x, int y, int w, int h, u32 color1, u32 color2, sf2d_gradient_dir direction)
{
	sf2d_vertex_pos_col *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_col), 8);
	if (!vertices) return;

	vertices[0].position = (sf2d_vector_3f){(float)x,   (float)y,   SF2D_DEFAULT_DEPTH};
	vertices[1].position = (sf2d_vector_3f){(float)x+w, (float)y,   SF2D_DEFAULT_DEPTH};
	vertices[2].position = (sf2d_vector_3f){(float)x,   (float)y+h, SF2D_DEFAULT_DEPTH};
	vertices[3].position = (sf2d_vector_3f){(float)x+w, (float)y+h, SF2D_DEFAULT_DEPTH};

	vertices[0].color = color1;
	vertices[1].color = (direction == SF2D_LEFT_TO_RIGHT) ? color2 : color1;
	vertices[2].color = (direction == SF2D_LEFT_TO_RIGHT) ? color1 : color2;
	vertices[3].color = color2;

	sf2d_draw_rectangle_internal(vertices);
}

void sf2d_draw_rectangle_gradient_rotate(int x, int y, int w, int h, u32 color1, u32 color2, sf2d_gradient_dir direction, float rad)
{
	sf2d_vertex_pos_col *vertices = sf2d_pool_memalign(4 * sizeof(sf2d_vertex_pos_col), 8);
	if (!vertices) return;

	int w2 = w/2.0f;
	int h2 = h/2.0f;

	vertices[0].position = (sf2d_vector_3f){(float)-w2, (float)-h2, SF2D_DEFAULT_DEPTH};
	vertices[1].position = (sf2d_vector_3f){(float) w2, (float)-h2, SF2D_DEFAULT_DEPTH};
	vertices[2].position = (sf2d_vector_3f){(float)-w2, (float) h2, SF2D_DEFAULT_DEPTH};
	vertices[3].position = (sf2d_vector_3f){(float) w2, (float) h2, SF2D_DEFAULT_DEPTH};

	vertices[0].color = color1;
	vertices[1].color = (direction == SF2D_LEFT_TO_RIGHT) ? color2 : color1;
	vertices[2].color = (direction == SF2D_LEFT_TO_RIGHT) ? color1 : color2;
	vertices[3].color = color2;

	C3D_Mtx m;
	Mtx_Identity(&m);
	Mtx_Translate(&m, x+w2, y+h2, 0, true);
	Mtx_RotateZ(&m, rad, true);

	sf2d_set_transform(&m);
	sf2d_draw_rectangle_internal(vertices);
	sf2d_set_transform(NULL);
}

void sf2d_draw_fill_circle(int x, int y, int radius, u32 color)
{
	static const int num_segments = 100;
	sf2d_vertex_pos_col *vertices = sf2d_pool_memalign((num_segments + 2) * sizeof(sf2d_vertex_pos_col), 8);
	if (!vertices) return;

	vertices[0].position = (sf2d_vector_3f){(float)x, (float)y, SF2D_DEFAULT_DEPTH};
	vertices[0].color = color;

	float theta = 2 * M_PI / (float)num_segments;
	float c = cosf(theta);
	float s = sinf(theta);
	float t;

	float xx = radius;
	float yy = 0;
	int i;

	for (i = 1; i <= num_segments; i++) {
		vertices[i].position = (sf2d_vector_3f){(float)(x + xx), (float)(y + yy), SF2D_DEFAULT_DEPTH};
		vertices[i].color = vertices[0].color;

		t = xx;
		xx = c * xx - s * yy;
		yy = s * t + c * yy;
	}

	vertices[num_segments + 1].position = vertices[1].position;
	vertices[num_segments + 1].color = vertices[1].color;

	sf2d_setup_env_internal(vertices);

	C3D_DrawArrays(GPU_TRIANGLE_FAN, 0, num_segments + 2);
}

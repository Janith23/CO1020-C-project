#ifndef RENDERER_H
#define RENDERER_H

#include "math3d.h"

typedef struct {
    vec3_t *vertices;
    int vertex_count;
    int (*edges)[2];
    int edge_count;
} wireframe_t;

// Projects 3D vertex to 2D screen space
void project_vertex(vec3_t vertex, mat4_t model, mat4_t view, mat4_t proj,
                    int width, int height, int *x, int *y, float *depth);

// Circular clipping for viewport
int clip_to_circular_viewport(int width, int height, int x, int y);

// Renders wireframe edges using a line draw function and computes brightness via lighting
void render_wireframe(vec3_t *vertices, int v_count, int edges[][2], int e_count,
                      mat4_t model, mat4_t view, mat4_t proj,
                      int width, int height,
                      void (*draw_line)(int, int, int, int, unsigned char));

void render_wireframe_unlit(vec3_t *vertices, int v_count, int edges[][2], int e_count,
                      mat4_t model, mat4_t view, mat4_t proj,
                      int width, int height,
                      void (*draw_line)(int, int, int, int, unsigned char));

#endif 

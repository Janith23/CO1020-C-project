// renderer.c
#include "renderer.h"
#include "lighting.h"
#include <stdlib.h>
#include <math.h>

void project_vertex(vec3_t vertex, mat4_t model, mat4_t view, mat4_t proj,
                    int width, int height, int *x, int *y, float *depth) {
    vec3_t world = mat4_mul_vec3(model, vertex);
    vec3_t camera = mat4_mul_vec3(view, world);
    vec3_t projected = mat4_mul_vec3(proj, camera);

    if (fabsf(projected.z) < 1e-6f)
        projected.z = (projected.z < 0) ? -1e-6f : 1e-6f;

    float f = 1.0f / -projected.z;

    float px = projected.x * f;
    float py = projected.y * f;

    *x = (int)((px + 1.0f) * 0.5f * width);
    *y = (int)((1.0f - py) * 0.5f * height);
    *depth = -projected.z;
}

int clip_to_circular_viewport(int width, int height, int x, int y) {
    int cx = width / 2, cy = height / 2;
    int r = (width < height ? width : height) / 2;
    int dx = x - cx, dy = y - cy;
    return (dx*dx + dy*dy) <= (r * r);
}

void render_wireframe(vec3_t *vertices, int v_count, int edges[][2], int e_count,
                      mat4_t model, mat4_t view, mat4_t proj,
                      int width, int height,
                      void (*draw_line)(int, int, int, int, unsigned char)) {
    int *xs = malloc(v_count * sizeof(int));
    int *ys = malloc(v_count * sizeof(int));
    float *zs = malloc(v_count * sizeof(float));
    vec3_t *world_vertices = malloc(sizeof(vec3_t) * v_count);
    if (!xs || !ys || !zs || !world_vertices) {
        free(xs); free(ys); free(zs); free(world_vertices);
        return;
    }

   
    for (int i = 0; i < v_count; i++) {
        world_vertices[i] = mat4_mul_vec3(model, vertices[i]);
    }


    for (int i = 0; i < v_count; i++) {
        project_vertex(vertices[i], model, view, proj, width, height, &xs[i], &ys[i], &zs[i]);
    }


    for (int i = 0; i < e_count; i++) {
        int a = edges[i][0], b = edges[i][1];
        int x0 = xs[a], y0 = ys[a], x1 = xs[b], y1 = ys[b];

        if (!clip_to_circular_viewport(width, height, x0, y0)) continue;
        if (!clip_to_circular_viewport(width, height, x1, y1)) continue;

        float brightness = compute_edge_brightness(world_vertices[a], world_vertices[b]);
        unsigned char color = (unsigned char)(brightness * 255.0f);

        draw_line(x0, y0, x1, y1, color);
    }

    free(xs);
    free(ys);
    free(zs);
    free(world_vertices);
}

void render_wireframe_unlit(vec3_t *vertices, int v_count, int edges[][2], int e_count,
                      mat4_t model, mat4_t view, mat4_t proj,
                      int width, int height,
                      void (*draw_line)(int, int, int, int, unsigned char)) {
    int *xs = malloc(v_count * sizeof(int));
    int *ys = malloc(v_count * sizeof(int));
    float *zs = malloc(v_count * sizeof(float));

    for (int i = 0; i < v_count; i++) {
        project_vertex(vertices[i], model, view, proj, width, height, &xs[i], &ys[i], &zs[i]);
    }

    for (int i = 0; i < e_count; i++) {
        int a = edges[i][0], b = edges[i][1];
        int x0 = xs[a], y0 = ys[a], x1 = xs[b], y1 = ys[b];

        if (!clip_to_circular_viewport(width, height, x0, y0)) continue;
        if (!clip_to_circular_viewport(width, height, x1, y1)) continue;

        draw_line(x0, y0, x1, y1, 255); 
    }

    free(xs);
    free(ys);
    free(zs);
}
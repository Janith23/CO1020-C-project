#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"
#include "renderer.h"
#include "lighting.h"
#include "animate_soccerball_cube.h"

#define WIDTH   1024
#define HEIGHT  1024
#define NFRAMES 200

canvas_t *global_canvas = NULL;

static void draw_line_canvas(int x0, int y0, int x1, int y1, unsigned char intensity) {
    extern canvas_t *global_canvas;
    float normalized = intensity / 255.0f;
    draw_line_f(global_canvas, (float)x0, (float)y0, (float)x1, (float)y1, 1.5f, normalized);
}

int main() {
    wireframe_t cube = generate_cube();
    wireframe_t ball = generate_soccer_ball();

    mat4_t proj = mat4_identity();
    proj.m[0][0] = proj.m[1][1] = 1.5f;
    proj.m[2][2] = -1.0f;

    mat4_t view = mat4_translate(0.0f, 0.0f, -10.0f);

    vec3_t lights[] = {{0.0f, 0.0f, -1.0f}}; 
    set_directional_lights(lights, 1);

    for (int frame = 0; frame < NFRAMES; ++frame) {
        float angle = (2.0f * M_PI) * frame / (NFRAMES - 1); 

       
        mat4_t cube_model = mat4_rotate_xyz(angle/10, angle, 0.0f);
        global_canvas = create_canvas(WIDTH, HEIGHT);
        render_wireframe(cube.vertices, cube.vertex_count, cube.edges, cube.edge_count,
                         cube_model, view, proj, WIDTH, HEIGHT, draw_line_canvas);

       
        float radius = 3.0f;
        float bounce = fabsf(sinf(angle * 2.0f)) * 2.0f;
        mat4_t orbit = mat4_translate(radius * cosf(angle), bounce, radius * sinf(angle));
        mat4_t spin = mat4_rotate_xyz(angle, angle, angle);
        mat4_t ball_model = mat4_mul(orbit, spin);
        render_wireframe(ball.vertices, ball.vertex_count, ball.edges, ball.edge_count,
                         ball_model, view, proj, WIDTH, HEIGHT, draw_line_canvas);

      
        char fname[64];
        snprintf(fname, sizeof(fname), "frame%03d.pgm", frame);
        save_canvas_to_pgm(global_canvas, fname);
        free_canvas(global_canvas);
    }

    free(cube.vertices); free(cube.edges);
    free(ball.vertices); free(ball.edges);
    return 0;
}

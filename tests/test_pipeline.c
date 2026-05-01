#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"
#include "renderer.h"
#include "animate_soccerball_cube.h"

#define WIDTH   1024
#define HEIGHT  1024
#define NFRAMES 60

canvas_t *global_canvas = NULL;

static void draw_line_canvas(int x0, int y0, int x1, int y1, unsigned char intensity) {
    extern canvas_t *global_canvas;
    float normalized = intensity / 255.0f;
    draw_line_f(global_canvas, (float)x0, (float)y0, (float)x1, (float)y1, 1.5f, normalized);
}

int main() {
    wireframe_t ball = generate_cube();

    mat4_t proj = mat4_identity();
    proj.m[0][0] = proj.m[1][1] = 1.5f;
    proj.m[2][2] = -1.0f;

    mat4_t view = mat4_translate(0.0f, 0.0f, -10.0f);

    for (int frame = 0; frame < NFRAMES; ++frame) {
        global_canvas = create_canvas(WIDTH, HEIGHT);

        float angle = frame * (2.0f * M_PI / NFRAMES);
        mat4_t model = mat4_rotate_xyz(angle, angle * 0.5f, angle * 0.25f);

        render_wireframe_unlit(ball.vertices, ball.vertex_count, ball.edges, ball.edge_count,
                         model, view, proj, WIDTH, HEIGHT, draw_line_canvas);

        char filename[64];
        snprintf(filename, sizeof(filename), "frame%03d.pgm", frame);
        save_canvas_to_pgm(global_canvas, filename);

        free_canvas(global_canvas);
    }

    free(ball.vertices);
    free(ball.edges);
    return 0;
}

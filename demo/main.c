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
#define NFRAMES 60

canvas_t *global_canvas = NULL;


extern void draw_line_f(canvas_t *c, float x0, float y0, float x1, float y1, float thickness, float intensity);

static void draw_clockface_lines(canvas_t *canvas) {
    int cx = WIDTH / 2;
    int cy = HEIGHT / 2;
    float radius = (WIDTH < HEIGHT ? WIDTH : HEIGHT) * 0.4f;
    
    for (int angle_deg = 0; angle_deg < 360; angle_deg += 15) {
        float angle_rad = angle_deg * (M_PI / 180.0f);
        float x_end = cx + radius * cosf(angle_rad);
        float y_end = cy + radius * sinf(angle_rad);
        draw_line_f(canvas, (float)cx, (float)cy, x_end, y_end, 2.0f, 1.0f); 
    }
}



int main() {


    canvas_t *canvas = create_canvas(WIDTH, HEIGHT);

    draw_clockface_lines(canvas);

    save_canvas_to_pgm(canvas, "clockface.pgm");
    free_canvas(canvas);



    return 0;
}

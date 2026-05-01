#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"

canvas_t* create_canvas(int width, int height) {
    canvas_t* c = malloc(sizeof(canvas_t));
    c->width = width;
    c->height = height;
    c->pixels = malloc(height * sizeof(float*));
    for (int y = 0; y < height; ++y) {
        c->pixels[y] = calloc(width, sizeof(float));
    }
    return c;
}

void free_canvas(canvas_t* c) {
    for (int y = 0; y < c->height; ++y) free(c->pixels[y]);
    free(c->pixels);
    free(c);
}

static float clamp(float v, float min, float max) {
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

void set_pixel_f(canvas_t* c, float x, float y, float intensity) {
    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    float dx = x - x0;
    float dy = y - y0;

    for (int i = 0; i <= 1; ++i) {
        for (int j = 0; j <= 1; ++j) {
            int xi = x0 + i;
            int yj = y0 + j;
            if (xi >= 0 && xi < c->width && yj >= 0 && yj < c->height) {
                float w = (1 - fabsf(dx - i)) * (1 - fabsf(dy - j));
                float new_intensity = clamp(intensity * w, 0.0f, 1.0f);
                if (new_intensity > c->pixels[yj][xi]) {
                    c->pixels[yj][xi] = new_intensity;
                }
            }
        }
    }
}

void draw_line_f(canvas_t* c, float x0, float y0, float x1, float y1, float thickness, float intensity) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float length = sqrtf(dx*dx + dy*dy);
    if (length == 0) return;

    float step_x = dx / length;
    float step_y = dy / length;

    for (float i = 0; i <= length; i += 0.5f) {
        float x = x0 + i * step_x;
        float y = y0 + i * step_y;

        int radius = (int)(thickness / 2.0f + 0.5f);
        for (int oy = -radius; oy <= radius; ++oy) {
            for (int ox = -radius; ox <= radius; ++ox) {
                float dist = sqrtf(ox * ox + oy * oy);
                if (dist <= thickness / 2.0f) {
                    set_pixel_f(c, x + ox, y + oy, intensity);
                }
            }
        }
    }
}

void save_canvas_to_pgm(canvas_t *c, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening file for writing PGM");
        return;
    }

    fprintf(fp, "P2\n%d %d\n255\n", c->width, c->height);
    for (int y = 0; y < c->height; ++y) {
        for (int x = 0; x < c->width; ++x) {
            int val = (int)(c->pixels[y][x] * 255.0f);
            if (val > 255) val = 255;
            if (val < 0) val = 0;
            fprintf(fp, "%d ", val);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

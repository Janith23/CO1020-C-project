#include "animation.h"

static float cubic_bezier(float t, float p0, float p1, float p2, float p3) {
    float u = 1.0f - t;
    return (u*u*u)*p0 + 3*u*u*t*p1 + 3*u*t*t*p2 + t*t*t*p3;
}

float bezier_interp(float start, float end, float c1, float c2, float t) {
    float val = cubic_bezier(t, start, c1, c2, end);
    return val;
}

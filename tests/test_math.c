#include <stdio.h>
#include "math3d.h"

int main() {
    vec3_t p = vec3_from_spherical(1.0f, 1.0f, 1.0f);
    printf("Cartesian: (%f, %f, %f)\n", p.x, p.y, p.z);

    vec3_t unit = vec3_normalize_fast(p);
    printf("Normalized: (%f, %f, %f)\n", unit.x, unit.y, unit.z);

    vec3_t b = {1.0f, 0.0f, 0.0f};
    vec3_t s = vec3_slerp(p, b, 0.5f);
    printf("Slerp: (%f, %f, %f)\n", s.x, s.y, s.z);

    return 0;
}

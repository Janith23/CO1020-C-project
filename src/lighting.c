#include "lighting.h"
#include "math3d.h"

// Fixed directional light vector
static vec3_t fixed_light_dir = { -1.0f, 0.0f, 0.0f };

void set_directional_lights(vec3_t *dirs, int count) {
    (void)dirs;
    (void)count;
}

float compute_edge_brightness(vec3_t start, vec3_t end) {
  
    vec3_t midpoint = {
        (start.x + end.x) * 0.5f,
        (start.y + end.y) * 0.5f,
        (start.z + end.z) * 0.5f,
    };


    vec3_t normal = vec3_normalize_fast(midpoint);

    float dot = vec3_dot(normal, fixed_light_dir);

    if (dot > 0.0f) {
        return 1.0f;  
    } else {
        return 0.4f;   
    }
}

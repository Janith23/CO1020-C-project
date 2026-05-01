#ifndef LIGHTING_H
#define LIGHTING_H

#include "math3d.h"


void set_directional_lights(vec3_t *dirs, int count);
float compute_edge_brightness(vec3_t edge_start, vec3_t edge_end);

#endif
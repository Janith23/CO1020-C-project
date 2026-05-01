#include <stdio.h>
#include <stdlib.h>
#include "math3d.h"
#include "renderer.h"

// Constants for soccer ball geometry
#define C0 0.809016994f
#define C1 1.618033988f
#define C2 1.809016994f
#define C3 2.118033988f
#define C4 2.427050983f

// 60 vertices of biscribed truncated icosahedron (soccer ball)
static const vec3_t base_verts[60] = {
    { 0.5f,  0.0f,  C4}, { 0.5f,  0.0f, -C4}, { -0.5f,  0.0f,  C4}, { -0.5f,  0.0f, -C4},
    { C4,   0.5f,  0.0f}, { C4,  -0.5f,  0.0f}, { -C4,  0.5f,  0.0f}, { -C4, -0.5f,  0.0f},
    { 0.0f,  C4,   0.5f}, { 0.0f,  C4,  -0.5f}, { 0.0f, -C4,   0.5f}, { 0.0f, -C4,  -0.5f},
    { 1.0f,  C0,   C3}, { 1.0f,  C0,  -C3}, { 1.0f, -C0,   C3}, { 1.0f, -C0,  -C3},
    {-1.0f,  C0,   C3}, {-1.0f,  C0,  -C3}, {-1.0f, -C0,   C3}, {-1.0f, -C0,  -C3},
    { C3,   1.0f,  C0}, { C3,   1.0f, -C0}, { C3,  -1.0f,  C0}, { C3,  -1.0f, -C0},
    {-C3,   1.0f,  C0}, {-C3,   1.0f, -C0}, {-C3,  -1.0f,  C0}, {-C3,  -1.0f, -C0},
    { C0,   C3,   1.0f}, { C0,   C3,  -1.0f}, { C0,  -C3,   1.0f}, { C0,  -C3,  -1.0f},
    {-C0,   C3,   1.0f}, {-C0,   C3,  -1.0f}, {-C0,  -C3,   1.0f}, {-C0,  -C3,  -1.0f},
    { 0.5f,   C1,   C2}, { 0.5f,   C1,  -C2}, { 0.5f,  -C1,   C2}, { 0.5f,  -C1,  -C2},
    {-0.5f,   C1,   C2}, {-0.5f,   C1,  -C2}, {-0.5f,  -C1,   C2}, {-0.5f,  -C1,  -C2},
    { C2,    0.5f,   C1}, { C2,    0.5f,  -C1}, { C2,   -0.5f,   C1}, { C2,   -0.5f,  -C1},
    {-C2,    0.5f,   C1}, {-C2,    0.5f,  -C1}, {-C2,   -0.5f,   C1}, {-C2,   -0.5f,  -C1},
    { C1,    C2,    0.5f}, { C1,    C2,   -0.5f}, { C1,   -C2,    0.5f}, { C1,   -C2,   -0.5f},
    {-C1,    C2,    0.5f}, {-C1,    C2,   -0.5f}, {-C1,   -C2,    0.5f}, {-C1,   -C2,   -0.5f},
};

// Faces define pentagons and hexagons for edges extraction
static const int faces[32][6] = {
    {0,2,18,42,38,14},{1,3,17,41,37,13},{2,0,12,36,40,16},{3,1,15,39,43,19},
    {4,5,23,47,45,21},{5,4,20,44,46,22},{6,7,26,50,48,24},{7,6,25,49,51,27},
    {8,9,33,57,56,32},{9,8,28,52,53,29},{10,11,31,55,54,30},{11,10,34,58,59,35},
    {12,44,20,52,28,36},{13,37,29,53,21,45},{14,38,30,54,22,46},{15,47,23,55,31,39},
    {16,40,32,56,24,48},{17,49,25,57,33,41},{18,50,26,58,34,42},{19,43,35,59,27,51},
    {0,14,46,44,12,-1},{1,13,45,47,15,-1},{2,16,48,50,18,-1},{3,19,51,49,17,-1},
    {4,21,53,52,20,-1},{5,22,54,55,23,-1},{6,24,56,57,25,-1},{7,27,59,58,26,-1},
    {8,32,40,36,28,-1},{9,29,37,41,33,-1},{10,30,38,42,34,-1},{11,35,43,39,31,-1}
};

// Build unique edges from faces
int build_edges(int edges[90][2]) {
    int count = 0;
    for (int f = 0; f < 32; f++) {
        int prev = faces[f][5] >= 0 ? faces[f][5] : faces[f][4];
        for (int i = 0; i < 6; i++) {
            int curr = faces[f][i];
            if (curr < 0) break;
            int a = prev < curr ? prev : curr;
            int b = prev < curr ? curr : prev;
            int dup = 0;
            for (int e = 0; e < count; e++) {
                if (edges[e][0] == a && edges[e][1] == b) {
                    dup = 1;
                    break;
                }
            }
            if (!dup) {
                edges[count][0] = a;
                edges[count][1] = b;
                count++;
            }
            prev = curr;
        }
    }
    return count;
}

// Generate soccer ball wireframe structure
wireframe_t generate_soccer_ball(void) {
    wireframe_t wf = {0};
    wf.vertex_count = 60;
    wf.edge_count = 90;
    wf.vertices = malloc(sizeof(vec3_t) * wf.vertex_count);
    wf.edges = malloc(sizeof(int[2]) * wf.edge_count);
    if (!wf.vertices || !wf.edges) {
        free(wf.vertices);
        free(wf.edges);
        wf.vertex_count = 0;
        wf.edge_count = 0;
        return wf;
    }
    float size = 0.5f; 
    for (int i = 0; i < wf.vertex_count; i++) {
        wf.vertices[i].x = base_verts[i].x * size;
        wf.vertices[i].y = base_verts[i].y * size;
        wf.vertices[i].z = base_verts[i].z * size;
    }
    wf.edge_count = build_edges((int (*)[2])wf.edges);
    return wf;
}

// Optional: generate cube wireframe for testing

wireframe_t generate_cube(void) {
    wireframe_t wf = {0};
    float CUBE_SCALE = 2.0f;
    static const vec3_t base_verts[8] = {
        {-0.5f,-0.5f,-0.5f}, { 0.5f,-0.5f,-0.5f},
        { 0.5f, 0.5f,-0.5f}, {-0.5f, 0.5f,-0.5f},
        {-0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f, 0.5f},
        { 0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}
    };
    static const int edges[12][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    wf.vertex_count = 8;
    wf.edge_count = 12;
    wf.vertices = malloc(sizeof(vec3_t) * wf.vertex_count);
    wf.edges = malloc(sizeof(int[2]) * wf.edge_count);
    if (!wf.vertices || !wf.edges) {
        free(wf.vertices);
        free(wf.edges);
        wf.vertex_count = 0;
        wf.edge_count = 0;
        return wf;
    }

    for (int i = 0; i < wf.vertex_count; i++) {
        wf.vertices[i].x = base_verts[i].x * CUBE_SCALE;
        wf.vertices[i].y = base_verts[i].y * CUBE_SCALE;
        wf.vertices[i].z = base_verts[i].z * CUBE_SCALE;
    }

    for (int i = 0; i < wf.edge_count; i++) {
        wf.edges[i][0] = edges[i][0];
        wf.edges[i][1] = edges[i][1];
    }

    return wf;
}

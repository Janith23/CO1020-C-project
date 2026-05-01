# libtiny3d - Software 3D Wireframe Renderer in C

## Overview

`libtiny3d` is a compact software 3D renderer written purely in C, implementing a wireframe rendering pipeline with basic lighting and animation. It outputs rendered frames as PGM images for further processing or conversion to GIF animations.

This project covers:

- 3D vector and matrix math operations  
- Wireframe rendering with perspective projection and viewport clipping  
- Directional lighting using Lambert shading for edges  
- Animations including rotation and bouncing of 3D objects  
- Exporting rendered frames as PGM images  

---

## File Structure

libtiny3d/
├── build/ # Compiled object files and executables
│  ├── visual.exe
│  ├──demo.exe
│  ├──test_math.exe
│  ├──test_pipeline.exe
│  ├──libtiny3d.a
│  ├──all .0 files of codes
├── demo/
│ ├── main.c # Clockface demo
├── include/ # Header files
│ ├── animate_soccerball_cube.h
│ ├── animation.h
│ ├── canvas.h
│ ├── lighting.h
│ ├── math3d.h
│ └── renderer.h
├── src/ # Source code files
│ ├── animate_soccerball_cube.c
│ ├── animation.c
│ ├── canvas.c
│ ├── lighting.c
│ ├── math3d.c
│ └── renderer.c
├── tests/ # Unit and integration tests
│ ├── test_math.c
│ ├── test_pipeline.c
│ └── visual_soccerball.c # Final animation demo (cube + soccer ball)
├── Makefile # Build automation script
└── README.md # This file



---

## Build Instructions

Ensure you have GCC installed.

Run:
    make

This builds the static library and executables:

build/test_math.exe — Unit tests for math functions

build/test_pipeline.exe — Rendering pipeline test

build/demo.exe — Clockface demo

build/visual.exe — Animation demo with cube and soccer ball


Running the Programs

1. Clockface Demo (demo.exe)

Renders a clockface with radial lines every 15°. Outputs clockface.pgm

run:
    build\demo.exe


2. Math Tests (test_math.exe)

Runs vector and matrix math tests.

run:
    build\test_math.exe

3. Pipeline Test (test_pipeline.exe)

Renders a rotating soccer ball wireframe with fixed lighting. Outputs 60 frames (frame_000.pgm to frame_059.pgm).

run:
    build\test_pipeline.exe


4. Animation Demo (visual.exe)

Renders a cube rotating around X-axis with a bouncing, rotating soccer ball orbiting it. Outputs 200 frames (frame000.pgm to frame199.pgm).

run:
    build\visual.exe


About the Tests

test_math.c

Validates vector and matrix operations (dot product, cross product, normalization, rotations).

test_pipeline.c

Tests full rendering pipeline: 3D transformations, projection, clipping, lighting, and wireframe drawing.

visual_soccerball.c

Final animation combining cube rotation, soccer ball bouncing/orbiting, and fixed directional lighting.



Generating GIF from Frames

Use ffmpeg to convert PGM frames to GIF:

in wsl 

run:
    ffmpeg -framerate 20 -i frame%03d.pgm animation.gif





Made by K.J.V. Kahagalla, K.A.H. Kumarasinghe
University of Peradeniya, Sri Lanka
CO1020: Computer Systems Programming 
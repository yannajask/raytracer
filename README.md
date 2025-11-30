# Raytracer

Based off of [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

## Installation

### Requirements

Requires OpenMP for some basic CPU parallelization. CMake file has been made for my specific environment at the moment.

### MacOS

```bash
cmake -S . -B build \         
  -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang \
  -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++
```

To compile, run:
```
cmake --build build
```

To render an image:
```
./build/raytracer >> image.ppm
```

## To-do

- [x] Raytracing in One Weekend
- [ ] Raytracing: The Next Week
- [ ] Raytracing: The Rest of Your Life
- [x] Basic parallelization
- [ ] Better parallelization
- [ ] .png image file formatting
- [ ] Model I/O
- [ ] General purpose CMake file

## Gallery

## Infinity Room

Inspired by [Yayoi Kusama's exhibition at the AGO](https://ago.ca/exhibitions/yayoi-kusamas-infinity-mirrored-room-lets-survive-forever).

![infinity mirror room](examples/infinity_room.png "Infinity Mirror Room")

Rendered at 500 samples per pixel.
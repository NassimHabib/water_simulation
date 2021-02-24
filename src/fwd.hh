#pragma once

#include <memory>

extern int window;

// program

class program;
using program_ptr = std::shared_ptr<program>;

extern program_ptr render;
extern program_ptr wireframe;

class Camera;
using camera_ptr = std::shared_ptr<Camera>;

extern camera_ptr camera;

extern int tesselation_level;

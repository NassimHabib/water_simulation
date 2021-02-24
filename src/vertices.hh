#pragma once

#include <vector>

std::vector<GLfloat> VERTICES_PLANE = {

    // 1st quad
        0.5f,  -1.0f,  0.5f,
        0.5f,  -1.0f, -0.5f,
        -0.5f,  -1.0f, -0.5f,
        -0.5f,  -1.0f,  0.5f,
    // 2nd quad
        1.5f,  -1.0f,  0.5f,
        1.5f,  -1.0f, -0.5f,
        0.5f,  -1.0f, -0.5f,
        0.5f,  -1.0f,  0.5f,

};


std::vector<GLfloat> TEXTURES_PLANE = {
  // 1st quad
  0.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f,

  // 2nd quad
  0.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f,
};


std::vector<GLfloat> NORMALS_PLANE = {
  // 1st quad
   0.f,  1.f,  0.f,
   0.f,  1.f,  0.f,
   0.f,  1.f,  0.f,
   0.f,  1.f,  0.f,
  // 2nd quad
   0.f,  1.f,  0.f,
   0.f,  1.f,  0.f,
   0.f,  1.f,  0.f,
   0.f,  1.f,  0.f,
};

#include <utility>

#include "error.hh"
#include "object.hh"
#include "program.hh"

#include "vertices.hh"
#include <vector>

VAObject::VAObject(std::vector<GLfloat>  vertices)
  : id_(0), vertices_(std::move(vertices))
{}

VAObject::VAObject(std::vector<GLfloat>  vertices, std::vector<GLfloat>  textures)
  : id_(0), vertices_(std::move(vertices)), textures_(std::move(textures))
{}

VAObject::VAObject(std::vector<GLfloat> vertices,
                   std::vector<GLfloat> textures,
                   std::vector<GLfloat> normals)
    : id_(0), vertices_(std::move(vertices)), textures_(std::move(textures)), normals_(std::move(normals))
{}

GLuint& VAObject::id_get()
{
    return id_;
}

GLuint VAObject::id_get() const
{
    return id_;
}

void VAObject::draw()
{
    glBindVertexArray(id_);
    glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
    glBindVertexArray(0);
}

void VAObject::draw_patches()
{
    glBindVertexArray(id_);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    TEST_OPENGL_ERROR();
    glDrawArrays(GL_PATCHES, 0, vertices_.size());
    TEST_OPENGL_ERROR();
    glBindVertexArray(0);
}



PlaneObject::PlaneObject()
    : VAObject(VERTICES_PLANE, TEXTURES_PLANE, NORMALS_PLANE)
{}

PlaneObject::plane_ptr PlaneObject::create_plane()
{
  auto plane = std::make_shared<PlaneObject>();

  GLuint position_id;
  GLuint texture_coords_id;
  GLuint normal_id;

  GLint position_location = glGetAttribLocation(render->id_, "v_position");
  TEST_OPENGL_LOCATION(position_location);

  //GLint texture_coords_location = glGetAttribLocation(render->id_, "v_texture_coords");
  //TEST_OPENGL_LOCATION(texture_coords_location);

  //GLint normal_location = glGetAttribLocation(render->id_, "v_normal");
  //TEST_OPENGL_LOCATION(normal_location);

  // Creation and link of VAO
  glGenVertexArrays(1, &plane->id_get());
  glBindVertexArray(plane->id_get());

  // Creation of VBOs
  glGenBuffers(1, &position_id);
  //glGenBuffers(1, &texture_coords_id);
  //glGenBuffers(1, &normal_id);

  // Link of VBOs
  glBindBuffer(GL_ARRAY_BUFFER, position_id);
  glBufferData(GL_ARRAY_BUFFER, plane->vertices_.size() * sizeof(float), plane->vertices_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(position_location);

  //glBindBuffer(GL_ARRAY_BUFFER, texture_coords_id);
  //glBufferData(GL_ARRAY_BUFFER, plane->textures_.size() * sizeof(float), plane->textures_.data(), GL_STATIC_DRAW);
  //glVertexAttribPointer(texture_coords_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
  //glEnableVertexAttribArray(texture_coords_location);

  //glBindBuffer(GL_ARRAY_BUFFER, normal_id);
  //glBufferData(GL_ARRAY_BUFFER, plane->normals_.size() * sizeof(float), plane->normals_.data(), GL_STATIC_DRAW);
  //glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
  //glEnableVertexAttribArray(normal_location);

  glBindVertexArray(0);

  return plane;
}

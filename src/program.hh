#pragma once

#include <iostream>
#include <memory>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "fwd.hh"

class program
{
public:

  program() = default;
  ~program();

  static std::string load_shader(const std::string& filename);

  static program_ptr make_program(const std::string& vertex_filename,
                                  const std::string& frament_filename,
                                  const std::string& geometry_filename="",
                                  const std::string& tesselation_ctrl_filename="",
                                  const std::string& tesselation_eval_filename="");

  static void error_shader_compilation(const std::string& shader_name, GLuint shader);
  static void error_shader_link(GLuint program);

  bool is_ready() const;
  void use() const;

  void setUniform1i(const std::string& uniform, int value);
  void setUniform3fv(const std::string &uniform, glm::vec3 vector);
  void setUniformMatrix4fv(const std::string& uniform, glm::mat4 matrix);

  bool is_ready_ = false;
  std::string logs_;

  GLuint id_;
  GLuint vertex_shader_;
  GLuint fragment_shader_;
  GLuint geometry_shader_ = 0;
  GLuint tesselation_ctrl_shader_ = 0;
  GLuint tesselation_eval_shader_ = 0;

};

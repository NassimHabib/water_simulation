#pragma once

#include <map>
#include <string>

#include <GL/glew.h>

extern std::map<GLenum, std::string> OPENGL_ERROR_CODES;

#define TEST_OPENGL_ERROR() test_opengl_error(__LINE__, __FILE__)
#define TEST_OPENGL_LOCATION(location) test_opengl_location(__LINE__, __FILE__, location)

void test_opengl_error(int line, const char* file);
void test_opengl_location(int line, const char* file, GLint location);

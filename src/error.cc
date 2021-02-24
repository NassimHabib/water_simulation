#include <iostream>
#include <map>

#include "error.hh"


std::map<GLenum, std::string> OPENGL_ERROR_CODES = {
  {GL_NO_ERROR,                         "NO_ERROR"                          },
  {GL_INVALID_ENUM,                     "GL_INVALID_ENUM"                   },
  {GL_INVALID_VALUE,                    "GL_INVALID_VALUE"                  },
  {GL_INVALID_OPERATION,                "GL_INVALID_OPERATION"              },
  {GL_INVALID_FRAMEBUFFER_OPERATION,    "GL_INVALID_FRAMEBUFFER_OPERATION"  },
  {GL_OUT_OF_MEMORY,                    "GL_OUT_OF_MEMORY"                  },
  {GL_STACK_UNDERFLOW,                  "GL_STACK_UNDERFLOW"                },
  {GL_STACK_OVERFLOW,                   "GL_STACK_OVERFLOW"                 }
};

void test_opengl_error(int line, const char* file)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
      std::string error_string;                                               \

      switch (error) {
        case GL_NO_ERROR:
        case GL_INVALID_ENUM:
        case GL_INVALID_VALUE:
        case GL_INVALID_OPERATION:
        case GL_INVALID_FRAMEBUFFER_OPERATION:
        case GL_OUT_OF_MEMORY:
        case GL_STACK_UNDERFLOW:
        case GL_STACK_OVERFLOW:
          error_string = OPENGL_ERROR_CODES[error];
          break;
        default:
          error_string = "UNKNOWN";
          break;
      }

      std::cerr << "[OpenGL][ERROR] " << file << " " << error_string
                << " at line " << line << std::endl;
    }
}

void test_opengl_location(int line, const char* file, GLint location)
{
  if (location == -1)
  {
    std::cerr << "[OpenGL][ERROR] Location returned -1 at line "
              << line << " in file " << file << std::endl;
  }
}

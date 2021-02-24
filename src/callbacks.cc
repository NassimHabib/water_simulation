#include <iostream>
#include <map>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "fwd.hh"

#include "callbacks.hh"
#include "camera.hh"
#include "program.hh"
#include "object.hh"



constexpr bool callback_debug = false;

std::map<int, std::string> special_keys = {
        {GLUT_KEY_F1,           "F1"        },
        {GLUT_KEY_F2,           "F2"        },
        {GLUT_KEY_F3,           "F3"        },
        {GLUT_KEY_F4,           "F4"        },
        {GLUT_KEY_F5,           "F5"        },
        {GLUT_KEY_F6,           "F6"        },
        {GLUT_KEY_F7,           "F7"        },
        {GLUT_KEY_F8,           "F8"        },
        {GLUT_KEY_F9,           "F9"        },
        {GLUT_KEY_F10,          "F10"       },
        {GLUT_KEY_F11,          "F11"       },
        {GLUT_KEY_F12,          "F12"       },
        {GLUT_KEY_LEFT,         "LEFT"      },
        {GLUT_KEY_UP,           "UP"        },
        {GLUT_KEY_RIGHT,        "RIGHT"     },
        {GLUT_KEY_DOWN,         "DOWN"      },
        {GLUT_KEY_PAGE_UP,      "PAGE_UP"   },
        {GLUT_KEY_PAGE_DOWN,    "PAGE_DOWN" },
        {GLUT_KEY_HOME,         "HOME"      },
        {GLUT_KEY_END,          "END"       },
        {GLUT_KEY_INSERT,       "INSERT"    }
};


static int tesselation_precision = 1;
static bool tesselation_inc = false;
static bool tesselation_dec = false;

static bool show_wireframe = false;


static void update_tesselation()
{
    static int previous_tesselation_level = tesselation_level;

    if (!tesselation_inc && !tesselation_dec)
        return;

    if (tesselation_inc)
        tesselation_level += tesselation_precision;

    if (tesselation_dec)
        tesselation_level -= tesselation_precision;

    tesselation_level = std::clamp(tesselation_level, 1, 100);

    if (previous_tesselation_level != tesselation_level)
    {
        std::cout << "tesselation level: " << tesselation_level << std::endl;
        previous_tesselation_level = tesselation_level;
    }
}


void callbackIdle()
{
    camera->update();
    update_tesselation();

    camera->deg = (float)((int)(camera->deg + 1.f) % 360);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = camera->look_at();

    glm::mat4 projection = glm::perspective(glm::radians(camera->fov_get()), 16.0f / 9.0f, 0.1f, 100.0f);

    glm::vec3 pointlight_position = glm::vec3(3, 0, -1);

    render->use();
    //render->setUniform3fv("u_pointlight_position", pointlight_position);
    render->setUniform1i("u_tesselation_level", tesselation_level);
    render->setUniform1i("u_is_wireframe", false);
    render->setUniformMatrix4fv("u_m", model);
    render->setUniformMatrix4fv("u_v", view);
    render->setUniformMatrix4fv("u_p", projection);

    wireframe->use();
    wireframe->setUniform1i("u_tesselation_level", tesselation_level);
    wireframe->setUniform1i("u_is_wireframe", true);
    wireframe->setUniformMatrix4fv("u_m", model);
    wireframe->setUniformMatrix4fv("u_v", view);
    wireframe->setUniformMatrix4fv("u_p", projection);


    glutPostRedisplay();
}

void callbackDisplay()
{
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render->use();
    render->setUniform1i("u_tesselation_level", tesselation_level);
    plane->draw_patches();

    // Wireframe
    if (show_wireframe)
    {
        wireframe->use();
        wireframe->setUniform1i("u_tesselation_level", tesselation_level);
        plane->draw_patches();
    }

    glutSwapBuffers();
}

void callbackReshape(int width, int height)
{
  if (callback_debug)
  {
    std::cout << "[CALLBACK] Reshape ("
              << "width = " << width << ", height = " << height
              << ")" << std::endl;
  }

  glViewport(0, 0, width, height);
}

void callbackKeyboard(unsigned char key, int x, int y)
{
    if (callback_debug)
    {
        std::cout << "[CALLBACK] Keyboard("
                  << "key = " << key << ", x = " << x << ", y = " << y
                  << ")" << std::endl;
    }

    if (key == 'f' || key == 'F')
    {
        glutFullScreenToggle();
    }
    else if (key == 27 /* ESCAPE */)
    {
        glutDestroyWindow(window);
        std::exit(0);
    }
    // Camera control
    else if (key == 'z' || key == 'Z')
    {
        camera->move(CameraMove::FORWARD);
    }
    else if (key == 's' || key == 'S')
    {
        camera->move(CameraMove::BACKWARD);
    }
    else if (key == 'q' || key == 'Q')
    {
        camera->move(CameraMove::LEFT);
    }
    else if (key == 'd' || key == 'D')
    {
        camera->move(CameraMove::RIGHT);
    }
    else if (key == 'a' || key == 'A')
    {
        camera->move(CameraMove::DOWN);
    }
    else if (key == 'e' || key == 'E')
    {
        camera->move(CameraMove::UP);
    }
    else if (key == 'r' || key == 'R')
    {
        camera->reset();
    }
    else if (key == 'x' || key == 'X')
    {
        camera->move(CameraMove::ROTATE_X);
    }
    else if (key == 'c' || key == 'C')
    {
        camera->move(CameraMove::ROTATE_Y);
    }
    else if (key == 'v' || key == 'V')
    {
        camera->move(CameraMove::ROTATE_Z);
    }
    else if (key == 'u' || key == 'U')
    {
        tesselation_inc = true;
    }
    else if (key == 'j' || key == 'J')
    {
        tesselation_dec = true;
    }

    camera->move(CameraMove::ROTATE_INV, glutGetModifiers() & GLUT_ACTIVE_SHIFT);
}

void callbackKeyboardUp(unsigned char key, int x, int y)
{
    if (callback_debug)
    {
        std::cout << "[CALLBACK] KeyboardUp("
                  << "key = " << key << ", x = " << x << ", y = " << y
                  << ")" << std::endl;
    }

    // Camera control
    if (key == 'z' || key == 'Z')
    {
        camera->move(CameraMove::FORWARD, false);
    }
    else if (key == 's' || key == 'S')
    {
        camera->move(CameraMove::BACKWARD, false);
    }
    else if (key == 'q' || key == 'Q')
    {
        camera->move(CameraMove::LEFT, false);
    }
    else if (key == 'd' || key == 'D')
    {
        camera->move(CameraMove::RIGHT, false);
    }
    else if (key == 'a' || key == 'A')
    {
        camera->move(CameraMove::DOWN, false);
    }
    else if (key == 'e' || key == 'E')
    {
        camera->move(CameraMove::UP, false);
    }
    else if (key == 'x' || key == 'X')
    {
        camera->move(CameraMove::ROTATE_X, false);
    }
    else if (key == 'c' || key == 'C')
    {
        camera->move(CameraMove::ROTATE_Y, false);
    }
    else if (key == 'v' || key == 'V')
    {
        camera->move(CameraMove::ROTATE_Z, false);
    }
    else if (key == 'u' || key == 'U')
    {
        tesselation_inc = false;
    }
    else if (key == 'j' || key == 'J')
    {
        tesselation_dec = false;
    }
    else if (key == 'w' || key == 'W')
    {
        show_wireframe = !show_wireframe;
    }

    camera->move(CameraMove::ROTATE_INV, glutGetModifiers() & GLUT_ACTIVE_SHIFT);
}

void callbackSpecial(int key, int x, int y)
{
    if (callback_debug)
    {
        std::cout << "[CALLBACK] Special("
                  << "key = " << special_keys[key] << ", x = " << x << ", y =" << y
                  << ")" << std::endl;
    }

    // Camera control
    if (key == GLUT_KEY_UP)
    {
        camera->move(CameraMove::FORWARD);
    }
    else if (key == GLUT_KEY_DOWN)
    {
        camera->move(CameraMove::BACKWARD);
    }
    else if (key == GLUT_KEY_LEFT)
    {
        camera->move(CameraMove::LEFT);
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        camera->move(CameraMove::RIGHT);
    }
    else if (key == GLUT_KEY_PAGE_UP)
    {
        camera->move(CameraMove::UP);
    }
    else if (key == GLUT_KEY_PAGE_DOWN)
    {
        camera->move(CameraMove::DOWN);
    }

    camera->move(CameraMove::ROTATE_INV, glutGetModifiers() & GLUT_ACTIVE_SHIFT);
}

void callbackSpecialUp(int key, int x, int y)
{
    if (callback_debug)
    {
        std::cout << "[CALLBACK] SpecialUp("
                  << "key = " << special_keys[key] << ", x = " << x << ", y =" << y
                  << ")" << std::endl;
    }

    // Camera control
    if (key == GLUT_KEY_UP)
    {
        camera->move(CameraMove::FORWARD, false);
    }
    else if (key == GLUT_KEY_DOWN)
    {
        camera->move(CameraMove::BACKWARD, false);
    }
    else if (key == GLUT_KEY_LEFT)
    {
        camera->move(CameraMove::LEFT, false);
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        camera->move(CameraMove::RIGHT, false);
    }
    else if (key == GLUT_KEY_PAGE_UP)
    {
        camera->move(CameraMove::UP, false);
    }
    else if (key == GLUT_KEY_PAGE_DOWN)
    {
        camera->move(CameraMove::DOWN, false);
    }

    camera->move(CameraMove::ROTATE_INV, glutGetModifiers() & GLUT_ACTIVE_SHIFT);
}

void callbackMouseWheel(int wheel, int direction, int x, int y)
{
    float save = camera->fov_get();

    if (direction > 0)
        camera->zoom(-1);
    else
        camera->zoom(1);

    if (callback_debug)
    {
        std::cout << "[CALLBACK] MouseWheel("
                  << "wheel = " << wheel << ", direction = " << direction << ", x = " << x << ", y =" << y << ", old fov =" << save << ", new fov = " << camera->fov_get()
                  << ")" << std::endl;
    }
}

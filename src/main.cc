#include <cmath>

#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "fwd.hh"

#include "callbacks.hh"
#include "camera.hh"
#include "error.hh"
#include "generic_image.hh"
#include "object.hh"
#include "program.hh"


/*
 * Global variables
 */

int window = 0;

camera_ptr camera = nullptr;

// Shaders
program_ptr render              = nullptr;
program_ptr wireframe           = nullptr;

// Objects
PlaneObject::plane_ptr plane = nullptr;

int tesselation_level = 100;


/*
 * Initialization
 */

void init_glut(int &argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(0, 0);\

    window = glutCreateWindow("Water Simulation");

    glutSetCursor(GLUT_CURSOR_NONE);
    glutIgnoreKeyRepeat(1);

    // Callback functions
    glutIdleFunc(callbackIdle);
    glutDisplayFunc(callbackDisplay);
    // glutOverlayDisplayFunc
    glutReshapeFunc(callbackReshape);
    glutKeyboardFunc(callbackKeyboard);
    glutKeyboardUpFunc(callbackKeyboardUp);
    glutSpecialFunc(callbackSpecial);
    glutSpecialUpFunc(callbackSpecialUp);
    // glutMouseFunc
    glutMouseWheelFunc(callbackMouseWheel);
}

bool init_glew()
{
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialization failed" << std::endl;
        return false;
    }
    return true;
}

void init_GL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glClearColor(0, 0, 0, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
}

void init_camera()
{
    camera = Camera::create_camera(glm::vec3(0.0f, 0.0f, 3.0f));
}

void init_programs()
{
    render = program::make_program("../shaders/water_vertex.shd",
                                   "../shaders/water_fragment.shd",
                                   "",
                                   "../shaders/plane_tess_ctrl.shd",
                                   "../shaders/plane_tess_eval.shd");
    if (!render)
      std::exit(1);

    wireframe = program::make_program("../shaders/wireframe_vertex.shd",
                                   "../shaders/wireframe_fragment.shd",
                                   "../shaders/wireframe_geometry.shd",
                                   "../shaders/plane_tess_ctrl.shd",
                                   "../shaders/plane_tess_eval.shd");
    if (!wireframe)
      std::exit(1);
}

void init_objects()
{
    plane = PlaneObject::create_plane();
}


int main(int argc, char *argv[])
{
    init_glut(argc, argv);

    if (!init_glew())
        return 1;

    init_GL();
    TEST_OPENGL_ERROR();

    init_camera();
    init_programs();
    init_objects();

    glutMainLoop();
}

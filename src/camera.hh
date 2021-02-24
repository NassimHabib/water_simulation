#pragma once

#include <map>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "fwd.hh"

enum class CameraMove
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ROTATE_X,
    ROTATE_Y,
    ROTATE_Z,
    ROTATE_INV,
};

class Camera
{
public:

    Camera();
    Camera(glm::vec3 position);

    glm::mat4 look_at() const;

    void reset();

    void fov_set(float fov);
    float fov_get() const;

    void move(CameraMove move, bool set = true);
    void zoom(float value);
    void update();

    static camera_ptr create_camera();
    static camera_ptr create_camera(glm::vec3 position);
    float deg = 0.f;


private:
    float fov_;

    glm::vec3 position_;
    glm::vec3 forward_;
    glm::vec3 up_;

    float rotation_deg = 1.f;

    std::map<CameraMove, bool> moving_;
};

glm::mat3 get_x_rotation_matrix(float deg);
glm::mat3 get_y_rotation_matrix(float deg);
glm::mat3 get_z_rotation_matrix(float deg);

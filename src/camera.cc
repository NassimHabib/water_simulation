#include "camera.hh"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

Camera::Camera()
    : fov_(45.0f), position_(glm::vec3(0.0f)), forward_(glm::vec3(0.0f, 0.0f, -1.0f)), up_(glm::vec3(0.0f, 1.0f, 0.0f))
{}

Camera::Camera(glm::vec3 position)
    : fov_(45.0f), position_(position), forward_(glm::vec3(0.0f, 0.0f, -1.0f)), up_(glm::vec3(0.0f, 1.0f, 0.0f))
{}

glm::mat4 Camera::look_at() const
{
    return glm::lookAt(position_, position_ + forward_, up_);
}

void Camera::reset()
{
    fov_ = 45.0f;
    forward_ = glm::vec3(0.0f, 0.0f, -1.0f);
    up_ = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::fov_set(float fov)
{
    fov_ = fov;
}

float Camera::fov_get() const
{
    return fov_;
}

camera_ptr Camera::create_camera()
{
    return std::make_shared<Camera>();
}

camera_ptr Camera::create_camera(glm::vec3 position)
{
    return std::make_shared<Camera>(position);
}

void Camera::move(CameraMove move, bool set)
{
    moving_[move] = set;
}

void Camera::update()
{
    float camera_speed = 0.04f;

    auto dforward = camera_speed * forward_;
    auto dup = camera_speed * up_;
    auto dside = camera_speed * glm::normalize(glm::cross(forward_, up_));

    if (moving_[CameraMove::FORWARD])
        position_ += dforward;

    if (moving_[CameraMove::BACKWARD])
        position_ -= dforward;

    if (moving_[CameraMove::LEFT])
        position_ -= dside;

    if (moving_[CameraMove::RIGHT])
        position_ += dside;

    if (moving_[CameraMove::UP])
        position_ += dup;

    if (moving_[CameraMove::DOWN])
        position_ -= dup;

    int sign = moving_[CameraMove::ROTATE_INV] ? -1 : 1;
    if (moving_[CameraMove::ROTATE_X])
    {
        glm::mat3 rot_x = get_x_rotation_matrix(sign * camera->rotation_deg);
        forward_ = rot_x * forward_;
        up_ = rot_x * up_;
    }
    if (moving_[CameraMove::ROTATE_Y])
    {
        glm::mat3 rot_y = get_y_rotation_matrix(sign * camera->rotation_deg);
        forward_ = rot_y * forward_;
        up_ = rot_y * up_;
    }
    if (moving_[CameraMove::ROTATE_Z])
    {
        glm::mat3 rot_z = get_z_rotation_matrix(sign * camera->rotation_deg);
        forward_ = rot_z * forward_;
        up_ = rot_z * up_;
    }
}

void Camera::zoom(float value)
{
    fov_ = glm::clamp(fov_ + value, 1.0f, 110.0f);
}



glm::mat3 get_x_rotation_matrix(float deg)
{
    auto rad = glm::radians(deg);
    return glm::mat3(1.0f, 0.f,       0.f,
                     0.f,  cos(rad), -sin(rad),
                     0.f,  sin(rad), cos(rad));
}

glm::mat3 get_y_rotation_matrix(float deg)
{
    auto rad = glm::radians(deg);
    return glm::mat3(cos(rad),  0.f, sin(rad),
                     0.f,       1.f, 0.f,
                     -sin(rad), 0.f, cos(rad));
}
glm::mat3 get_z_rotation_matrix(float deg)
{
    auto rad = glm::radians(deg);
    return glm::mat3(cos(rad), -sin(rad), 0.f,
                     sin(rad),  cos(rad), 0.f,
                     0.f,       0.f,      1.f);
}

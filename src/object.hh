#pragma once

#include <memory>
#include <vector>

#include <GL/glew.h>

class VAObject
{
public:
    VAObject() = default;
    VAObject(std::vector<GLfloat>  vertices);
    VAObject(std::vector<GLfloat>  vertices, std::vector<GLfloat>  textures);
    VAObject(std::vector<GLfloat> vertices,
             std::vector<GLfloat> textures,
             std::vector<GLfloat> normals);

    void draw();
    void draw_patches();

    GLuint& id_get();
    GLuint id_get() const;

    std::vector<GLfloat> vertices_;
    std::vector<GLfloat> textures_;
    std::vector<GLfloat> normals_;

private:
    GLuint id_;
};

class PlaneObject: public VAObject
{
public:
    using plane_ptr = std::shared_ptr<PlaneObject>;

    PlaneObject();

    static plane_ptr create_plane();
};

extern PlaneObject::plane_ptr plane;

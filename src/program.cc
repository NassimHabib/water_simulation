#include <fstream>
#include <iostream>
#include <sstream>

#include "error.hh"
#include "program.hh"

constexpr bool shader_debug = false;

std::string program::load_shader(const std::string &filename)
{
    std::stringstream buffer;
    std::ifstream stream(filename);

    if (!stream.is_open())
    {
        std::cerr << "Shader file " << filename << " failed to open" << std::endl;
    }

    buffer << stream.rdbuf();
    std::string shader(buffer.str());

    if (shader_debug)
    {
        std::cout << "#Shader " << filename << std::endl << shader << std::endl;
    }

    return shader;
}

void program::error_shader_compilation(const std::string &shader_name, GLuint shader)
{
    std::cerr << shader_name << " shader compilation failed" << std::endl;

    GLint log_size;
    char *shader_log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
    shader_log = new char[log_size + 1];
    glGetShaderInfoLog(shader, log_size, &log_size, shader_log);

    std::cerr << shader_log << std::endl;

    delete[] shader_log;
}

void program::error_shader_link(GLuint program)
{
    int length, maxLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    char* log = new char[maxLength];
    glGetProgramInfoLog(program, maxLength, &length, log);

    std::cerr << "Link failed" << std::endl;
    std::cerr << log << std::endl;
    delete[] log;
}

program_ptr program::make_program(const std::string &vertex_filename,
                                  const std::string &fragment_filename,
                                  const std::string &geometry_filename,
                                  const std::string &tesselation_ctrl_filename,
                                  const std::string &tesselation_eval_filename)
{
    auto vertex_shader = load_shader(vertex_filename);
    auto fragment_shader = load_shader(fragment_filename);

    auto prog = std::make_shared<program>();

    // Status
    GLint compilation = GL_FALSE;
    GLint link = GL_FALSE;

    // Vertex Shader
    prog->vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
    auto vertex_shader_c = vertex_shader.c_str();
    glShaderSource(prog->vertex_shader_, 1, (const GLchar **) &vertex_shader_c, 0);
    glCompileShader(prog->vertex_shader_);
    glGetShaderiv(prog->vertex_shader_, GL_COMPILE_STATUS, &compilation);
    if (compilation != GL_TRUE)
    {
        error_shader_compilation("Vertex", prog->vertex_shader_);
        return nullptr;
    }

    // Fragment Shader
    compilation = GL_FALSE;
    prog->fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragment_shader_c = fragment_shader.c_str();
    glShaderSource(prog->fragment_shader_, 1, (const GLchar **) &fragment_shader_c, 0);
    glCompileShader(prog->fragment_shader_);
    glGetShaderiv(prog->fragment_shader_, GL_COMPILE_STATUS, &compilation);
    if (compilation != GL_TRUE)
    {
        error_shader_compilation("Fragment", prog->fragment_shader_);
        return nullptr;
    }

    // Geometry Shader
    if (geometry_filename != "")
    {
        compilation = GL_FALSE;
        auto geometry_shader = load_shader(geometry_filename);

        prog->geometry_shader_ = glCreateShader(GL_GEOMETRY_SHADER);
        auto geometry_shader_c = geometry_shader.c_str();
        glShaderSource(prog->geometry_shader_, 1, (const GLchar **) &geometry_shader_c, 0);
        glCompileShader(prog->geometry_shader_);
        glGetShaderiv(prog->geometry_shader_, GL_COMPILE_STATUS, &compilation);
        if (compilation != GL_TRUE)
        {
            error_shader_compilation("Geometry", prog->geometry_shader_);
            return nullptr;
        }
    }

    // Tesselation Shader
    if (tesselation_ctrl_filename != "" && tesselation_eval_filename != "")
    {
        compilation = GL_FALSE;
        auto tesselation_ctrl_shader = load_shader(tesselation_ctrl_filename);
        auto tesselation_eval_shader = load_shader(tesselation_eval_filename);

        prog->tesselation_ctrl_shader_ = glCreateShader(GL_TESS_CONTROL_SHADER);
        prog->tesselation_eval_shader_ = glCreateShader(GL_TESS_EVALUATION_SHADER);
        auto tesselation_ctrl_shader_c = tesselation_ctrl_shader.c_str();
        auto tesselation_eval_shader_c = tesselation_eval_shader.c_str();
        glShaderSource(prog->tesselation_ctrl_shader_, 1, (const GLchar **) &tesselation_ctrl_shader_c, 0);
        glShaderSource(prog->tesselation_eval_shader_, 1, (const GLchar **) &tesselation_eval_shader_c, 0);
        glCompileShader(prog->tesselation_ctrl_shader_);
        glCompileShader(prog->tesselation_eval_shader_);
        glGetShaderiv(prog->tesselation_ctrl_shader_, GL_COMPILE_STATUS, &compilation);
        if (compilation != GL_TRUE)
        {
            error_shader_compilation("Tesselation ctrl", prog->tesselation_ctrl_shader_);
            return nullptr;
        }
        glGetShaderiv(prog->tesselation_eval_shader_, GL_COMPILE_STATUS, &compilation);
        if (compilation != GL_TRUE)
        {
            error_shader_compilation("Tesselation eval", prog->tesselation_eval_shader_);
            return nullptr;
        }
    }

    // Program
    if (prog->id_ = glCreateProgram(); prog->id_ == 0)
        return nullptr;

    glAttachShader(prog->id_, prog->vertex_shader_);
    glAttachShader(prog->id_, prog->fragment_shader_);
    if (prog->geometry_shader_ != 0)
        glAttachShader(prog->id_, prog->geometry_shader_);
    if (prog->tesselation_ctrl_shader_ != 0)
    {
        glAttachShader(prog->id_, prog->tesselation_ctrl_shader_);
        glAttachShader(prog->id_, prog->tesselation_eval_shader_);
    }
    glLinkProgram(prog->id_);
    glGetProgramiv(prog->id_, GL_LINK_STATUS, &link);
    if (link != GL_TRUE)
    {
        error_shader_link(prog->id_);
        return nullptr;
    }

    prog->is_ready_ = compilation == GL_TRUE && link == GL_TRUE;

    if (prog->tesselation_ctrl_shader_ != 0)
    {
        prog->use();
        prog->setUniform1i("u_tesselation_level", tesselation_level);
        TEST_OPENGL_ERROR();
    }

    return prog;
}

program::~program()
{
    glDeleteShader(vertex_shader_);
    glDeleteShader(fragment_shader_);
    glDeleteShader(geometry_shader_);
    glDeleteShader(tesselation_ctrl_shader_);
    glDeleteShader(tesselation_eval_shader_);
}

bool program::is_ready() const
{
    return is_ready_;
}

void program::use() const
{
    glUseProgram(id_);
}

void program::setUniform1i(const std::string& uniform, int value)
{
    GLint l = glGetUniformLocation(id_, uniform.c_str());
    TEST_OPENGL_LOCATION(l);
    glUniform1i(l, value);
}

void program::setUniform3fv(const std::string& uniform, glm::vec3 vector)
{
    GLint l = glGetUniformLocation(id_, uniform.c_str());
    TEST_OPENGL_LOCATION(l);
    glUniform3fv(l, 1, glm::value_ptr(vector));
}

void program::setUniformMatrix4fv(const std::string& uniform, glm::mat4 matrix)
{
    GLint l = glGetUniformLocation(id_, uniform.c_str());
    TEST_OPENGL_LOCATION(l);
    glUniformMatrix4fv(l, 1, GL_FALSE, glm::value_ptr(matrix));
}

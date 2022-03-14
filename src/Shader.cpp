#include "Shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <vector>

void Shader::loadFromFile(const std::string &file, Shader::ShaderType type)
{
    std::ifstream fstream { file, std::ios::in };
    if (!fstream.is_open())
    {
        std::cerr << "Failed to open file \"" << file << "\"." << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(fstream, line)) {
        lines.push_back(line + '\n');
    }

    std::vector<const char*> lines_raw { lines.size() };
    for (int i = 0; i < lines.size(); ++i) {
        lines_raw[i] = lines[i].c_str();
    }

    int glType;
    int *shaderObj;
    switch (type)
    {
        case ShaderType::VERTEX:
            glType = GL_VERTEX_SHADER;
            shaderObj = &vertex;
            break;
        case ShaderType::GEOMETRY:
            glType = GL_GEOMETRY_SHADER;
            shaderObj = &geometry;
            break;
        case ShaderType::FRAGMENT:
            glType = GL_FRAGMENT_SHADER;
            shaderObj = &fragment;
            break;
    }

    *shaderObj = glCreateShader(glType);
    glShaderSource(*shaderObj, lines_raw.size(), lines_raw.data(), nullptr);
    glCompileShader(*shaderObj);

    int success;
    glGetShaderiv(*shaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        char error[4096];
        glGetShaderInfoLog(*shaderObj, sizeof error, nullptr, error);
        std::cerr << "Error compiling shader " << file << std::endl;
        std::cerr << error << std::endl;
    }
}

void Shader::finalize()
{
    handle = glCreateProgram();
    for (auto &id : { vertex, geometry, fragment }) {
        if (id > 0)
            glAttachShader(handle, id);
    }
    glLinkProgram(handle);
    int success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if (!success) {
        char error[4096];
        glGetProgramInfoLog(handle, sizeof error, nullptr, error);
        std::cerr << "Error linking shader:" << std::endl;
        std::cerr << error << std::endl;
    }

    for (auto id : { &vertex, &geometry, &fragment }) {
        if (*id > 0)
        {
            glDetachShader(handle, *id);
            glDeleteShader(*id);
            *id = -1;
        }
    }

    // load uniforms
    int numUniforms;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &numUniforms);
    for (int i = 0; i < numUniforms; ++i) {
        char name[128];
        GLenum type;
        int size;
        glGetActiveUniform(handle, i, sizeof name, nullptr, &size, &type, name);
        uniforms[name] = { glGetUniformLocation(handle, name), static_cast<int>(type) };
    }
}

template<>
Shader& Shader::uniform<glm::mat4>(const std::string &name, glm::mat4 &value)
{
    auto loc = getUniform(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const float*)&value);
    return *this;
}

int Shader::getUniform(const std::string &name)
{
    auto it = uniforms.find(name);
    if (it == uniforms.end())
    {
        std::cerr << "The specified uniform \"" << name << "\" does not exist." << std::endl;
        return -1;
    }

    return it->second.loc;
}

Shader& Shader::bind()
{
    glUseProgram(handle);
    return *this;
}

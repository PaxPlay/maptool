#ifndef MAPTOOL_SRC_SHADERMANAGER_H
#define MAPTOOL_SRC_SHADERMANAGER_H

#include <map>
#include <string>
#include <memory>

#include "Shader.h"

class ShaderManager {
public:
    ShaderManager() : shaders()
    {
    }
    ShaderManager(const ShaderManager& obj) = delete;
    ShaderManager(const ShaderManager&& obj) = delete;

    void loadShader(const std::string &name, const std::string &vertex_file, const std::string &geometry_file, const std::string &fragment_file)
    {
        shaders[name] = std::make_unique<Shader>();
        if (!vertex_file.empty())
            shaders[name]->loadFromFile(vertex_file, Shader::ShaderType::VERTEX);
        if (!geometry_file.empty())
            shaders[name]->loadFromFile(geometry_file, Shader::ShaderType::GEOMETRY);
        if (!fragment_file.empty())
            shaders[name]->loadFromFile(fragment_file, Shader::ShaderType::FRAGMENT);
        shaders[name]->finalize();
    }

    Shader& operator[](const std::string &name)
    {
        return *shaders[name];
    }

    std::unique_ptr<Shader>& get(const std::string &name)
    {
        return shaders[name];
    }

private:
    std::map<std::string, std::unique_ptr<Shader>> shaders;
};

#endif //MAPTOOL_SRC_SHADERMANAGER_H

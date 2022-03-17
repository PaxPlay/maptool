#ifndef MAPTOOL_SRC_SHADER_H
#define MAPTOOL_SRC_SHADER_H

#include <string>
#include <map>

class Shader {
public:
    Shader() = default;
    Shader(const Shader &obj) = delete;

    enum class ShaderType {
        VERTEX,
        GEOMETRY,
        FRAGMENT
    };

    void loadFromFile(const std::string &file, ShaderType type);
    void finalize();

    template<typename T>
    Shader& uniform(const std::string &name, const T &value);
    Shader& bind();
private:
    int getUniform(const std::string &name);

    struct Uniform {
        int loc;
        int type;
    };

    int handle = -1;
    int vertex = -1;
    int geometry = -1;
    int fragment = -1;
    std::map<std::string, Uniform> uniforms;
};


#endif //MAPTOOL_SRC_SHADER_H

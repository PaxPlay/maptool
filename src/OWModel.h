#ifndef MAPTOOL_SRC_OWMODEL_H
#define MAPTOOL_SRC_OWMODEL_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "VertexArray.h"
#include "IModel.h"
#include "Camera.h"

class OWModel : public IModel {
public:
    explicit OWModel(const std::string &file);
    void draw(const glm::mat4 &VP, const glm::mat4 &M, ShaderManager *shaderManager, const Camera &camera) override;

private:
    struct Bone {
        Bone(std::ifstream &ifstream);
        std::string name;
        int16_t parent;
        glm::vec3 pos;
        glm::vec3 scale;
        glm::quat rot;
    };

    struct Vertex {
        Vertex() = default;
        std::vector<glm::vec2> uvs;
        uint8_t boneWeightCount;
        std::vector<uint16_t> boneIndices;
        std::vector<float> boneWeights;
        glm::vec4 color1;
        glm::vec4 color2;
    };

    /*
    struct Index {
        Index() = default;
        int8_t pointCount;
        std::vector<uint32_t> points;
    };
    */

    struct Mesh {
        Mesh(std::ifstream &ifstream);
        std::string name;
        uint64_t materialKey;
        uint8_t uvCount;
        uint32_t vertexCount;
        uint32_t indexCount;
        std::unique_ptr<std::vector<glm::vec3>> vertexPositions;
        std::unique_ptr<std::vector<glm::vec3>> vertexNormals;
        std::vector<Vertex> vertices;
        std::unique_ptr<std::vector<uint32_t>> indices;
        std::unique_ptr<VertexArray> vertexArray;
    };

    struct Empty {
        Empty(std::ifstream &ifstream);
        std::string name;
        glm::vec3 pos;
        glm::quat rot;
    };

private:
    std::string mtl;
    std::string name;
    uint16_t boneCount = 0;
    uint32_t meshCount = 0;
    uint32_t emptyCount = 0;
    std::vector<Bone> bones;
    std::vector<Mesh> meshes;
    std::vector<Empty> empties;
private:
    static std::string readString(std::ifstream &ifstream);
};

#endif //MAPTOOL_SRC_OWMODEL_H

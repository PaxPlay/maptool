#include "OWModel.h"
#include "Camera.h"

#include <iostream>
#include <fstream>
#include <exception>

OWModel::OWModel(const std::string &file)
{
    std::ifstream fstream { file, std::ios_base::in | std::ios_base::binary };

    if (!fstream.is_open())
    {
        std::cout << "Couldn't open the specified file." << std::endl;
        throw std::runtime_error("Couldn't open the specified file.");
    }

    struct {
        uint16_t major;
        uint16_t minor;
    } version {};
    fstream.read((char*)&version, 4);

    mtl = readString(fstream);
    name = readString(fstream);
    fstream.read((char*)&boneCount, 2);
    fstream.read((char*)&meshCount, 4);
    fstream.read((char*)&emptyCount, 4);

    bones.reserve(boneCount);
    meshes.reserve(meshCount);
    empties.reserve(emptyCount);

    for (size_t i = 0; i < boneCount; ++i)
    {
        bones.emplace_back(Bone(fstream));
    }
    for (size_t i = 0; i < meshCount; ++i)
    {
        meshes.emplace_back(Mesh(fstream));
    }
    for (size_t i = 0; i < emptyCount; ++i)
    {
        empties.emplace_back(Empty(fstream));
    }

    fstream.close();

    std::cout << "======== Model Summary ========" << std::endl
              << "Major: " << version.major << "; Minor: " << version.minor << std::endl
              << "Material: " << mtl << std::endl
              << "Name: " << name << std::endl
              << "Bones: " << boneCount << ", Meshes: " << meshCount << ", Empties: " << emptyCount << std::endl;
}

void OWModel::draw(const glm::mat4 &VP, const glm::mat4 &M, ShaderManager *shaderManager, const Camera &camera)
{
    glm::mat4 MVP = VP * M;
    glm::mat3 NM = glm::mat3(glm::transpose(glm::inverse(M)));
    (*shaderManager)["owmodel"]
            .bind()
            .uniform("MVP", MVP)
            .uniform("M", M)
            .uniform("NM", NM)
            .uniform("camera", camera.getPosition());
    for (auto &mesh : meshes)
    {
        mesh.vertexArray->draw();
    }
}

OWModel::Bone::Bone(std::ifstream &ifstream)
{
    name = readString(ifstream);
    ifstream.read((char*)&parent, 2);
    ifstream.read((char*)&pos, 12);
    ifstream.read((char*)&scale, 12);
    ifstream.read((char*)&rot, 16);
}

OWModel::Mesh::Mesh(std::ifstream &ifstream)
    : vertexPositions(std::make_unique<std::vector<glm::vec3>>()),
    vertexNormals(std::make_unique<std::vector<glm::vec3>>()),
    indices(std::make_unique<std::vector<uint32_t>>())
{
    name = readString(ifstream);
    ifstream.read((char*)&materialKey, 8);
    ifstream.read((char*)&uvCount, 1);
    ifstream.read((char*)&vertexCount, 4);
    ifstream.read((char*)&indexCount, 4);

    vertices.resize(vertexCount);
    vertexPositions->resize(vertexCount);
    vertexNormals->resize(vertexCount);
    for (size_t i = 0; i < vertexCount; ++i)
    {
        Vertex &v = vertices[i];
        ifstream.read((char*)&(*vertexPositions)[i], 12);
        ifstream.read((char*)&(*vertexNormals)[i], 12);
        v.uvs.resize(uvCount);
        for (size_t j = 0; j < uvCount; ++j)
        {
            ifstream.read((char*)&v.uvs[j], 8);
        }

        ifstream.read((char*)&v.boneWeightCount, 1);
        v.boneIndices.resize(v.boneWeightCount);
        for (size_t j = 0; j < v.boneWeightCount; ++j)
        {
            ifstream.read((char*)&v.boneIndices[j], 2);
        }
        v.boneWeights.resize(v.boneWeightCount);
        for (size_t j = 0; j < v.boneWeightCount; ++j)
        {
            ifstream.read((char*)&v.boneWeights[j], 4);
        }

        // not documented, see https://github.com/overtools/OWLib/blob/a7c0e7877b8f519aad231b84514962a90a9a6368/TankLib/ExportFormats/OverwatchModel.cs#L124
        ifstream.read((char*)&v.color1, 16);
        ifstream.read((char*)&v.color2, 16);
    }

    indices->resize(indexCount * 3);
    for (size_t i = 0; i < indexCount; ++i)
    {
        /*
        Index &idx = indices[i];
        ifstream.read((char*)&idx.pointCount, 1);
        idx.points.resize(idx.pointCount);
        for (size_t j = 0; j < idx.pointCount; ++j)
        {
            ifstream.read((char*)&idx.points[j], 4);
        }
        */
        uint8_t size = 0;
        ifstream.read((char*)&size, 1);
        if (size != 3)
            throw std::runtime_error("Only Triangles Indices are supported.");
        ifstream.read((char*)&(*indices)[3 * i], 12);
    }

    vertexArray = std::make_unique<VertexArray>(vertexPositions.get(), indices.get(), VertexArray::Type::TRIANGLES);
    vertexArray->addBuffer<glm::vec3>("normals", vertexNormals.get(), 1);
}

OWModel::Empty::Empty(std::ifstream &ifstream)
{
    name = readString(ifstream);
    ifstream.read((char*)&pos, 12);
    ifstream.read((char*)&rot, 16);
}

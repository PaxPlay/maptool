#include "OWMap.h"

#include <fstream>
#include <iostream>
#include <exception>
#include <memory>
#include <chrono>

#include <GL/glew.h>

OWMap::OWMap(const std::string &path, const std::string &file): path(path)
{
    std::ifstream fstream { path + file, std::ios_base::in | std::ios_base::binary };

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

    name = readString(fstream);

    fstream.read((char*)&objectCount, 4);
    fstream.read((char*)&detailCount, 4);
    fstream.read((char*)&lightCount, 4);

    objects.reserve(objectCount);
    for (size_t i = 0; i < objectCount; ++i)
    {
        objects.emplace_back(Object(fstream));
    }

    fstream.close();

    std::cout << "======== Map Summary ========" << std::endl
              << "Major: " << version.major << "; Minor: " << version.minor << std::endl
              << "Name: " << name << std::endl
              << "Objects: " << objectCount << ", Details: " << detailCount << ", Lights: " << lightCount << std::endl;
}

void OWMap::loadAssets()
{
    auto start = std::chrono::high_resolution_clock::now();
    models.reserve(objectCount);
    for (auto &obj : objects)
    {
        models.emplace_back(OWModel(path + obj.modelFile));
    }
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
    std::cout << "Done loading " << objectCount << " models, took " << duration.count() << "s.";
}

void OWMap::init()
{
    camera.setNear(0.01f);
    camera.setPosition({ 0.0f, 10.0f, 0.0f });

    shaders.loadShader("owmodel", "../shader/owmodel.vert", "", "../shader/owmodel.frag");

    loadAssets();
}

void OWMap::draw()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    auto VP = camera.getVP();

    for (size_t i = 0; i < objectCount; ++i)
    {
        for (auto &ent : objects[i].entities)
        {
            for (auto &rec : ent.records)
            {
                models[i].draw(VP, rec.M, &shaders, camera);
            }
        }
    }
}

Camera *OWMap::getCamera()
{
    return &camera;
}

OWMap::Object::Object(std::ifstream &ifstream)
{
    modelFile = readString(ifstream);
    ifstream.read((char*)&entityCount, 4);
    entities.resize(entityCount);
    for (size_t i = 0; i < entityCount; ++i)
    {
        Entity &ent = entities[i];
        ent.materialFile = readString(ifstream);
        ifstream.read((char*)&ent.recordCount, 4);

        ent.records.resize(ent.recordCount);
        for (size_t j = 0; j < ent.recordCount; ++j)
        {
            Record &r = ent.records[j];
            ifstream.read((char*)&r.pos, 12);
            ifstream.read((char*)&r.scale, 12);
            ifstream.read((char*)&r.rot, 16);

            auto rotation = glm::mat4(r.rot);
            r.M = rotation * glm::scale(glm::mat4(1.0f), r.scale);
            r.M[3] = glm::vec4(r.pos, 1.0f);
        }
    }
}

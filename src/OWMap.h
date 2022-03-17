#ifndef MAPTOOL_SRC_OWMAP_H
#define MAPTOOL_SRC_OWMAP_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "OWAsset.h"
#include "OWModel.h"
#include "RenderScene.h"

class OWMap : public OWAsset, public RenderScene {
public:
    explicit OWMap(const std::string &path, const std::string &file);

    void init() override;
    void draw() override;
    Camera* getCamera() override;

private:
    void loadAssets();

private:
    struct Record {
        glm::vec3 pos;
        glm::vec3 scale;
        glm::quat rot;
        glm::mat4 M;
    };

    struct Entity {
        std::string materialFile;
        uint32_t recordCount;
        std::vector<Record> records;
    };

    struct Object {
        Object(std::ifstream &ifstream);
        std::string modelFile;
        uint32_t entityCount;
        std::vector<Entity> entities;
    };
private:
    Camera camera;
    ShaderManager shaders;
    std::string path;
    std::string name;

    uint32_t objectCount;
    uint32_t detailCount;
    uint32_t lightCount;
    std::vector<Object> objects;
    std::vector<OWModel> models;
};


#endif //MAPTOOL_SRC_OWMAP_H

#ifndef MAPTOOL_SRC_IMODEL_H
#define MAPTOOL_SRC_IMODEL_H

#include <glm/glm.hpp>
#include "ShaderManager.h"
#include "Camera.h"

class IModel {
public:
    virtual void draw(const glm::mat4 &VP, const glm::mat4 &M, ShaderManager *shaderManager, const Camera &camera) = 0;
};


#endif //MAPTOOL_SRC_IMODEL_H

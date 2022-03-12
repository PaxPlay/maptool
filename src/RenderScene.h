#ifndef MAPTOOL_SRC_RENDERSCENE_H
#define MAPTOOL_SRC_RENDERSCENE_H

#include "Camera.h"

class RenderScene {
public:
    virtual void init() = 0;
    virtual void draw() = 0;
    virtual Camera* getCamera() = 0;
};


#endif //MAPTOOL_SRC_RENDERSCENE_H

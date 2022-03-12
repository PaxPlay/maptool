#ifndef MAPTOOL_SRC_TESTRENDERSCENE_H
#define MAPTOOL_SRC_TESTRENDERSCENE_H

#include "RenderScene.h"
#include "Camera.h"
#include "Shader.h"

class TestRenderScene : public RenderScene {
public:
    TestRenderScene() = default;

    void init() override;
    void draw() override;
    Camera* getCamera() override;

private:
    Camera camera;
    Shader basic;
    Shader cube;

    unsigned int vertexBufferObject = 0;
    unsigned int vertexArrayObject = 0;
    unsigned int colorArrayObject = 0;
    unsigned int indexBufferObject = 0;
};


#endif //MAPTOOL_SRC_TESTRENDERSCENE_H

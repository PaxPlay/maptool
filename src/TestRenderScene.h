#ifndef MAPTOOL_SRC_TESTRENDERSCENE_H
#define MAPTOOL_SRC_TESTRENDERSCENE_H

#include <memory>

#include "RenderScene.h"
#include "Camera.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "VertexArray.h"

class TestRenderScene : public RenderScene {
public:
    TestRenderScene() = default;

    void init() override;
    void draw() override;
    Camera* getCamera() override;

private:
    Camera camera;
    ShaderManager shaders;
    std::unique_ptr<VertexArray> cube = nullptr;
};


#endif //MAPTOOL_SRC_TESTRENDERSCENE_H

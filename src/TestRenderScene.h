#ifndef MAPTOOL_SRC_TESTRENDERSCENE_H
#define MAPTOOL_SRC_TESTRENDERSCENE_H

#include <memory>

#include "RenderScene.h"
#include "Camera.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "VertexArray.h"
#include "OWModel.h"

class TestRenderScene : public RenderScene {
public:
    TestRenderScene() = default;

    void init() override;
    void draw() override;
    Camera* getCamera() override;

private:
    Camera camera;
    ShaderManager shaders;
    OWModel model { "../assets/Rialto/79F/Models/00000000598C.00C/00000000598C.owmdl" };

    std::unique_ptr<VertexArray> cube = nullptr;
};


#endif //MAPTOOL_SRC_TESTRENDERSCENE_H

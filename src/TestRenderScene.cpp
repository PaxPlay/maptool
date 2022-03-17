#include "TestRenderScene.h"

#include <GL/glew.h>

#include "ShaderManager.h"
#include "VertexArray.h"

static std::vector<glm::vec3> vertices = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 0.0f)
};

/*
static GLfloat colors[] = {
        1.0, 0.0f, 0.0f,
        0.0, 1.0f, 0.0f,
        0.0, 0.0f, 1.0f
};
*/

static std::vector<unsigned int> indices = {
    3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0
};

void TestRenderScene::init()
{
    camera.setPosition({ 0, 2, 0 });
    camera.setNear(0.01f);

    shaders.loadShader("cube", "../shader/cube.vert", "", "../shader/cube.frag");
    shaders.loadShader("owmodel", "../shader/owmodel.vert", "", "../shader/owmodel.frag");
    cube = std::make_unique<VertexArray>(&vertices, &indices);
}

void TestRenderScene::draw()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glm::mat4 VP = camera.getVP();
    glm::mat4 M = glm::mat4(1.0f);

    model.draw(VP, M, &shaders, camera);
//    shaders["cube"].bind()
//            .uniform("MVP", VP);
//    cube->draw();
}

Camera* TestRenderScene::getCamera()
{
    return &camera;
}

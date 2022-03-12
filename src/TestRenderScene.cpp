#include "TestRenderScene.h"

#include <GL/glew.h>
#include <iostream>
#include <string>

static GLfloat vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f
};

/*
static GLfloat colors[] = {
        1.0, 0.0f, 0.0f,
        0.0, 1.0f, 0.0f,
        0.0, 0.0f, 1.0f
};
*/

static GLuint indices[] = {
    3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0
};

void TestRenderScene::init()
{
    camera.setPosition({ 0, 2, 0 });
    camera.setNear(0.01f);

    basic.loadFromFile("../shader/basic.vert", Shader::ShaderType::VERTEX);
    basic.loadFromFile("../shader/basic.frag", Shader::ShaderType::FRAGMENT);
    basic.finalize();

    cube.loadFromFile("../shader/cube.vert", Shader::ShaderType::VERTEX);
    cube.loadFromFile("../shader/cube.frag", Shader::ShaderType::FRAGMENT);
    cube.finalize();

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    /*
    glGenBuffers(1, &colorArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, colorArrayObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof colors, colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    */

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void printVector(const std::string &name, const glm::vec3 &vec)
{
    std::cout << name << " x: " << vec.x << ", y: " << vec.y << ", z: " << vec.z << std::endl;
}

void TestRenderScene::draw()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    cube.bind();
    auto MVP = camera.getVP();
    cube.uniform("MVP", MVP);

    auto dir = camera.getDirection();
    auto up = camera.getUp();
    if (fabs(glm::dot(dir, up)) > 0.01)
    {
        printVector("angle", camera.getViewAngle());
        printVector("dir  ", dir);
        printVector("up   ", up);
    }

    glBindVertexArray(vertexBufferObject);
    glDrawElements(GL_TRIANGLE_STRIP, sizeof indices / sizeof(GLuint), GL_UNSIGNED_INT, nullptr);
}

Camera* TestRenderScene::getCamera()
{
    return &camera;
}

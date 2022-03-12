#include "RenderContext.h"

#include <GL/glew.h>
#include <iostream>

void RenderContext::setScene(RenderScene *scene)
{
    currentScene = scene;
    currentScene->init();
    currentScene->getCamera()->setAspectRatio(aspectRatio);
}

void RenderContext::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    currentScene->draw();
}

void
RenderContext::setAspectRatio(float value)
{
    aspectRatio = value;
    if (currentScene != nullptr)
        currentScene->getCamera()->setAspectRatio(aspectRatio);
}

void RenderContext::moveCamera(const glm::vec3 &offset)
{
    currentScene->getCamera()->setPosition(currentScene->getCamera()->getPosition() + offset);
}

void RenderContext::rotateCamera(const glm::vec3 &angleOffset)
{
    currentScene->getCamera()->setViewAngle(currentScene->getCamera()->getViewAngle() + angleOffset);
}

void RenderContext::moveCameraKeyboard(const glm::vec3 &move)
{
    auto camera = currentScene->getCamera();
    auto dir = camera->getDirection();
    auto up = camera->getUp();
    auto left = glm::cross(up, dir);

    glm::mat3 moveTransform { dir, left, glm::vec3(0.0f, 1.0f, 0.0f) };
    moveCamera(moveTransform * move);
}

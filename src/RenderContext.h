#ifndef MAPTOOL_SRC_RENDERCONTEXT_H
#define MAPTOOL_SRC_RENDERCONTEXT_H

#include "RenderScene.h"
#include "Camera.h"

class RenderContext {
public:
    RenderContext() = default;
    void setScene(RenderScene *scene);
    void setAspectRatio(float value);
    void render();
    void moveCamera(const glm::vec3 &offset);
    void rotateCamera(const glm::vec3 &angleOffset);
    void moveCameraKeyboard(const glm::vec3 &move);
private:
    RenderScene *currentScene = nullptr;
    float aspectRatio = 1.0f;
};


#endif //MAPTOOL_SRC_RENDERCONTEXT_H

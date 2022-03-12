#ifndef MAPTOOL_SRC_MAINWINDOW_H
#define MAPTOOL_SRC_MAINWINDOW_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "RenderContext.h"

class MainWindow {
public:
    MainWindow();

    int runEventLoop();
    glm::vec2 getSize()
    {
        int x, y;
        glfwGetWindowSize(window, &x, &y);
        return { x, y };
    }
    float getAspectRatio()
    {
        auto size = getSize();
        return size.x / size.y;
    }
    RenderContext& getRenderContext()
    {
        return renderContext;
    }
    bool isFocused()
    {
        return glfwGetWindowAttrib(window, GLFW_FOCUSED);
    }
private:
    bool isKeyPressed(int key);
public:
    void resized(int width, int height);
    void keyCallback(int key, int scancode, int action, int mode);
    void mouseButtonCallback(int button, int action, int mods);
    void cursorPositionCallback(double xpos, double ypos);
private:
    GLFWwindow *window;
    RenderContext renderContext;
    bool isControllingCamera = false;
    glm::vec2 lastMousePosition;
};


#endif //MAPTOOL_SRC_MAINWINDOW_H

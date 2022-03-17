#ifndef MAPTOOL_SRC_CONFIGURATIONMENU_H
#define MAPTOOL_SRC_CONFIGURATIONMENU_H

#include <GLFW/glfw3.h>

class ConfigurationMenu {
public:
    ConfigurationMenu() = default;
    ConfigurationMenu(const ConfigurationMenu &obj) = delete;
    void init(GLFWwindow *window);
    void shutdown();
    void draw();
    bool wantsMouseInput() const;
private:
    bool initialized = false;
public:
    float sensitivity = 1.5f;
    float moveSpeed = 100.0f;
};

extern ConfigurationMenu *config;

#endif //MAPTOOL_SRC_CONFIGURATIONMENU_H

#ifndef MAPTOOL_SRC_MAINWINDOW_H
#define MAPTOOL_SRC_MAINWINDOW_H

#include <SFML/Window.hpp>
#include <glm/glm.hpp>

#include "RenderContext.h"

class MainWindow {
public:
    MainWindow();

    int runEventLoop();
    glm::vec2 getSize()
    {
        auto size = window.getSize();
        return { size.x, size.y };
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
private:
    void resized();
    void processMouseMoveEvent(const sf::Event::MouseMoveEvent &event);
    void processKeyPressEvent(const sf::Event::KeyEvent &event);
    void processMouseButtonClickedEvent(const sf::Event::MouseButtonEvent &event);
private:
    sf::Window window;
    RenderContext renderContext;
};


#endif //MAPTOOL_SRC_MAINWINDOW_H

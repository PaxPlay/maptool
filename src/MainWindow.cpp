#include "MainWindow.h"

#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>
#include <chrono>
#include <thread>

void APIENTRY debugCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const  char *message, const void *userParam);

MainWindow::MainWindow()
{
    window.create(sf::VideoMode(800, 600),
            "Main Window",
            sf::Style::Default,
            sf::ContextSettings(32, 8, 0, 4, 0, sf::ContextSettings::Attribute::Debug));

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Failed to init GLEW: " << glewGetErrorString(err) << std::endl;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    resized();
}

int MainWindow::runEventLoop()
{
    auto lastFrame = std::chrono::high_resolution_clock::now();
    while (window.isOpen()) {
        renderContext.render();
        window.display();
        std::chrono::duration<double> frameTime = std::chrono::high_resolution_clock::now() - lastFrame;
        if (frameTime.count() < 0.01)
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(0.01) - frameTime);
        }

        glm::vec3 move {0.0f};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            move.x += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            move.x -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            move.y += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            move.y -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            move.z += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            move.z -= 1.0f;
        }
        if (glm::length(move) > 0.01f)
            renderContext.moveCameraKeyboard(move * 0.005f);

        sf::Event event {};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                resized(); // stuff happens
            if (event.type == sf::Event::MouseMoved)
                processMouseMoveEvent(event.mouseMove);
            if (event.type == sf::Event::KeyPressed)
                processKeyPressEvent(event.key);
            if (event.type == sf::Event::MouseButtonPressed)
                processMouseButtonClickedEvent(event.mouseButton);
        }
    }
    return 0;
}

void MainWindow::resized()
{
    glViewport(0, 0, (int)window.getSize().x, (int)window.getSize().y);
    renderContext.setAspectRatio(getAspectRatio());
}

void MainWindow::processMouseMoveEvent(const sf::Event::MouseMoveEvent &event)
{
    if (window.hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        auto screenCenter = getSize() / 2.0f;
        int centerX = static_cast<int>(screenCenter.x);
        int centerY = static_cast<int>(screenCenter.y);
        auto dx = static_cast<float>(event.x - centerX);
        auto dy = static_cast<float>(event.y - centerY);
        sf::Mouse::setPosition({ centerX, centerY }, window);
        renderContext.rotateCamera( {dy * 0.0025, dx * 0.0025, 0.0f} );
    }
}

void MainWindow::processKeyPressEvent(const sf::Event::KeyEvent &event)
{

}

void MainWindow::processMouseButtonClickedEvent(const sf::Event::MouseButtonEvent &event)
{
    if (window.hasFocus())
    {
        auto screenCenter = getSize() / 2.0f;
        int centerX = static_cast<int>(screenCenter.x);
        int centerY = static_cast<int>(screenCenter.y);
        sf::Mouse::setPosition({ centerX, centerY }, window);
    }
}

void APIENTRY debugCallback(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    }
    std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    }
    std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

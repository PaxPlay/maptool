#include "MainWindow.h"

#include <GL/glew.h>

#include <iostream>
#include <chrono>
#include <thread>

#include "ConfigurationMenu.h"
#include "OWModel.h"

static MainWindow *windowInstance = nullptr;

void sizeCallback(GLFWwindow *window, int width, int height)
{
    windowInstance->resized(width, height);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    windowInstance->keyCallback(key, scancode, action, mode);
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    windowInstance->mouseButtonCallback(button, action, mods);
}

void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    windowInstance->cursorPositionCallback(xpos, ypos);
}

void APIENTRY debugCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const  char *message, const void *userParam);
static void glfwErrorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

MainWindow::MainWindow() : lastMousePosition(0.0f)
{
    if (windowInstance)
        throw std::logic_error("There can only be one MainWindow instance during runtime.");
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        throw std::logic_error("Failed to initialize GLFW");
    }
    if (!glfwRawMouseMotionSupported())
    {
        std::cerr << "Raw mouse input is not supported." << std::endl;
        throw std::logic_error("Raw mouse input is not supported.");
    }

    window = glfwCreateWindow(800, 600, "maptool", nullptr, nullptr);
    windowInstance = this;
    glfwMakeContextCurrent(window);
    // settings: sf::ContextSettings(32, 8, 0, 4, 0, sf::ContextSettings::Attribute::Debug));

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Failed to init GLEW: " << glewGetErrorString(err) << std::endl;
        throw std::logic_error("Failed to initialize GLEW");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glfwSetKeyCallback(window, ::keyCallback);
    glfwSetWindowSizeCallback(window, ::sizeCallback);
    glfwSetMouseButtonCallback(window, ::mouseButtonCallback);
    glfwSetCursorPosCallback(window, ::cursorPositionCallback);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    resized(width, height);
    config->init(window);
}

int MainWindow::runEventLoop()
{
    auto lastFrame = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
        renderContext.render();
        config->draw();
        glfwSwapBuffers(window);

        // limit frame rate
        std::chrono::duration<double> frameTime = std::chrono::high_resolution_clock::now() - lastFrame;
        if (frameTime.count() < 0.01)
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(0.01) - frameTime);
        }

        if (isFocused())
        {
            glm::vec3 move{ 0.0f };
            if (isKeyPressed(GLFW_KEY_W))
            {
                move.x += 1.0f;
            }
            if (isKeyPressed(GLFW_KEY_S))
            {
                move.x -= 1.0f;
            }
            if (isKeyPressed(GLFW_KEY_A))
            {
                move.y += 1.0f;
            }
            if (isKeyPressed(GLFW_KEY_D))
            {
                move.y -= 1.0f;
            }
            if (isKeyPressed(GLFW_KEY_SPACE))
            {
                move.z += 1.0f;
            }
            if (isKeyPressed(GLFW_KEY_LEFT_CONTROL))
            {
                move.z -= 1.0f;
            }
            if (glm::length(move) > 0.01f)
                renderContext.moveCameraKeyboard(glm::normalize(move) * config->moveSpeed * 0.001f);
        }

        glfwPollEvents();
    }

    config->shutdown();
    return 0;
}

void MainWindow::resized(int width, int height)
{
    glViewport(0, 0, width, height);
    renderContext.setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

bool MainWindow::isKeyPressed(int key)
{
    int state = glfwGetKey(window, key);
    return state >= GLFW_PRESS;
}

void MainWindow::keyCallback(int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if (isControllingCamera)
        {
            isControllingCamera = false;
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void MainWindow::mouseButtonCallback(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1)
    {
        if (action == GLFW_PRESS && isFocused() && !config->wantsMouseInput())
        {
            isControllingCamera = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastMousePosition = { static_cast<float>(xpos), static_cast<float>(ypos) };
        }
    }
}

void MainWindow::cursorPositionCallback(double xpos, double ypos)
{
    if (isControllingCamera)
    {
        glm::vec2 mousePosition { static_cast<float>(xpos), static_cast<float>(ypos) };
        glm::vec2 difference = mousePosition - lastMousePosition ;
        renderContext.rotateCamera(glm::vec3(difference.y, difference.x, 0.0f) * 0.001f * config->sensitivity);

        lastMousePosition = mousePosition;
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

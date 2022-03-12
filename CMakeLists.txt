cmake_minimum_required(VERSION 3.21)
project(maptool)

set(CMAKE_CXX_STANDARD 14)

#set(SFML_STATIC_LIBRARIES TRUE)
#set(SFML_DIR deps/SFML-2.5.1/lib/cmake/SFML/)
#find_package(SFML 2 COMPONENTS system window graphics REQUIRED)
find_package(GLU)

add_executable(maptool
        src/main.cpp
        src/game_types.h
        src/Shader.cpp
        src/Shader.h
        src/MainWindow.cpp
        src/MainWindow.h
        src/RenderContext.cpp
        src/RenderContext.h
        src/Camera.cpp
        src/Camera.h
        src/RenderScene.h
        src/TestRenderScene.cpp
        src/TestRenderScene.h
        src/OWModel.cpp
        src/OWModel.h
        src/imgui/imgui.cpp
        src/imgui/imgui_draw.cpp
        src/imgui/imgui_tables.cpp
        src/imgui/imgui_widgets.cpp
        src/imgui/imgui_impl_opengl3.cpp
        src/imgui/imgui_impl_glfw.cpp
        src/ConfigurationMenu.cpp
        src/ConfigurationMenu.h)
target_include_directories(maptool PUBLIC
#        ${PROJECT_SOURCE_DIR}/deps/SFML-2.5.1/include/
        ${PROJECT_SOURCE_DIR}/deps/glfw-3.3.6.bin.WIN64/include/
        ${PROJECT_SOURCE_DIR}/deps/glm/
        ${PROJECT_SOURCE_DIR}/deps/glew-2.1.0/include/)

target_link_libraries(maptool
#        sfml-system
#        sfml-window
#        sfml-graphics
        ${GLU_LIBRARY}
        ${PROJECT_SOURCE_DIR}/deps/glew-2.1.0/lib/Release/x64/glew32.lib
        ${PROJECT_SOURCE_DIR}/deps/glfw-3.3.6.bin.WIN64/lib-vc2019/glfw3.lib)

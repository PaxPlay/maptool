#include "ConfigurationMenu.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

static ImGuiIO *io = nullptr;

void ConfigurationMenu::init(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    initialized = true;
}

void ConfigurationMenu::shutdown()
{
    if (!initialized)
        return;
    initialized = false;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ConfigurationMenu::draw()
{
    if (!initialized)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");

    if (ImGui::CollapsingHeader("Input"))
    {
        ImGui::DragFloat("Sensitivity", &sensitivity, 0.01, 0.01f, 10.0f);
        ImGui::DragFloat("Move Speed", &moveSpeed, 0.1, 0.1f, 250.0f);
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ConfigurationMenu::wantsMouseInput() const
{
    if (!initialized)
        return false;
    return io->WantCaptureMouse;
}

static ConfigurationMenu configMenu;
ConfigurationMenu *config = &configMenu;

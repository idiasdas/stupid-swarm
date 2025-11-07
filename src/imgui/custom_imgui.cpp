#include "custom_imgui.hpp"
#include "imgui.h"
#include "log.h"

CustomImgui::CustomImgui(GLFWwindow* window, OpenGLContext* context)
{
    _is_open = true;
    _context = context;
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init("#version 430");
}

void CustomImgui::new_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Enable docking
    ImGuiIO& io = ImGui::GetIO();
    ImGuiDockNodeFlags dockspace_flags = 0 | ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockspace_flags);
    io.DisplaySize = ImVec2(_context->get_window_width(), _context->get_window_height());
    glViewport(0, 0, _context->get_window_width(), _context->get_window_height());
}

void CustomImgui::update()
{
    ImGui::ShowDemoWindow();
    if (_is_open) {
        ImGui::Begin("Hello, World!", &_is_open, ImGuiWindowFlags_MenuBar);
    }
    ImGui::End();
}

void CustomImgui::draw()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CustomImgui::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

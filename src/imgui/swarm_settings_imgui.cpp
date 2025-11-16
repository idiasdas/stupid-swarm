#include "swarm_settings_imgui.hpp"
#include "imgui.h"
#include "imgui/custom_imgui.hpp"
#include "imgui_internal.h"
#include "log.h"
#include <algorithm>

namespace {
static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

}

SwarmSettingsImgui::SwarmSettingsImgui(GLFWwindow* window, OpenGLContext* context)
    : CustomImgui(window, context)
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 0.0f;
    style.WindowRounding = 0.f;

    for (int i = 0; i < 100; i++)
        _fps[i] = 0.f;
}

void SwarmSettingsImgui::Update()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowSize(ImVec2(450, 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
    auto itemsWidth = std::min(ImGui::GetWindowWidth() * 0.35f, 150.f);

    ImGui::Begin("Control Pane", nullptr, ImGuiWindowFlags_None);
    if (ImGui::BeginTabBar("##TabBar")) {
        if (ImGui::BeginTabItem("Settings")) {

            ImGui::SeparatorText("Simulation Control");

            if (ImGui::Button(_paused ? "Play" : "Pause", { itemsWidth, 20.f })) {
                _paused = !_paused;
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset", { itemsWidth, 20.f })) {
                _reset = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Quit", { itemsWidth, 20.f })) {
                _quit = true;
            }

            ImGui::PushItemWidth(itemsWidth);
            ImGui::SliderInt("Number of particles", &_nbParticles, 0, 10000);
            ImGui::PopItemWidth();

            ImGui::SeparatorText("Style Editing");
            ImGui::ColorEdit3("Particles Color", (float*)&_colorParticles);
            ImGui::SameLine();
            HelpMarker(
                "Click on the color square to open a color picker.\n"
                "CTRL+click on individual component to input value.\n");
            ImGui::ColorEdit3("Goal Color", (float*)&_colorGoal);
            ImGui::SameLine();
            HelpMarker(
                "Click on the color square to open a color picker.\n"
                "CTRL+click on individual component to input value.\n");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Logs")) {
            // ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            _log.Draw();
            ImGui::EndTabItem();
        }
    }

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x, viewport->WorkPos.y), ImGuiCond_Always, ImVec2(1.0f, 0.0f));

    ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
    ImGui::PlotLines("FPS", _fps, 100);
    ImGui::SameLine();
    ImGui::Text("%.2f", _fps[99]);
    ImGui::End();

    ImGui::ShowDemoWindow();
}

void SwarmSettingsImgui::Reset()
{
    _nbParticles = 10000;
    _paused = true;
    _reset = false;
}

void SwarmSettingsImgui::LogFPS(float fps)
{
    for (int i = 0; i < 99; i++)
        _fps[i] = _fps[i + 1];

    _fps[99] = fps;
}

#include "swarm_settings_imgui.hpp"
#include "imgui.h"
#include "imgui/custom_imgui.hpp"
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
    style.WindowRounding = 12.f;
}

void SwarmSettingsImgui::Update()
{
    ImGui::ShowDemoWindow();
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    auto itemsWidth = std::min(ImGui::GetWindowWidth() * 0.50f, 150.f);

    if (_isOpen) {
        ImGui::Begin("Settings", &_isOpen, ImGuiWindowFlags_None);

        ImGui::SeparatorText("Simulation Control");

        if (ImGui::Button(_paused ? "Play" : "Pause", { itemsWidth, 20.f })) {
            _paused = !_paused;
        }

        ImGui::PushItemWidth(itemsWidth);
        ImGui::SliderInt("Number of particles", &_nbParticles, 0, 10000);
        ImGui::PopItemWidth();

        ImGui::SeparatorText("Style Editing");
        ImGui::ColorEdit3("MyColor##1", (float*)&_colorParticles);
        ImGui::SameLine();
        HelpMarker(
            "Click on the color square to open a color picker.\n"
            "CTRL+click on individual component to input value.\n");
    }
    ImGui::End();
}

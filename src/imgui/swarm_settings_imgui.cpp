#include "swarm_settings_imgui.hpp"
#include "imgui.h"

void SwarmSettingsImgui::Update()
{
    ImGui::ShowDemoWindow();
    if (_isOpen) {
        ImGui::Begin("Settings", &_isOpen, ImGuiWindowFlags_MenuBar);

        auto windowSize = ImGui::GetWindowSize();
        windowSize[1] = 20.f;
        if (ImGui::Button(_paused ? "Play" : "Pause", windowSize)) {
            _paused = !_paused;
        }
        ImGui::SliderInt("Number of particles", &_nbParticles, 0, 10000);
    }
    ImGui::End();
}

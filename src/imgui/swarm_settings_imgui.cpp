#include "swarm_settings_imgui.hpp"
#include "imgui.h"

void SwarmSettingsImgui::update()
{
    ImGui::ShowDemoWindow();
    if (_is_open) {
        ImGui::Begin("Settings", &_is_open, ImGuiWindowFlags_MenuBar);

        auto window_size = ImGui::GetWindowSize();
        window_size[1] = 20.f;
        if (ImGui::Button(_paused ? "Play" : "Pause", window_size)) {
            _paused = !_paused;
        }
        ImGui::SliderInt("Number of particles", &_nb_particles, 0, 10000);
    }
    ImGui::End();
}

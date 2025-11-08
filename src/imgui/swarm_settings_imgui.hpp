#pragma once

#include "custom_imgui.hpp"

class SwarmSettingsImgui : public CustomImgui {
public:
    SwarmSettingsImgui(GLFWwindow* window, OpenGLContext* context);
    void Update() override;
    bool IsPaused() { return _paused; }
    void SetNbParticles(int nbParticles) { _nbParticles = (nbParticles > _maxNbParticles) ? _maxNbParticles : nbParticles; }
    int GetNbParticles() { return _nbParticles; }

private:
    bool _paused = true;
    int _nbParticles;
    int _maxNbParticles = 10000;
    ImVec4 _colorParticles = ImVec4(.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 250.0f / 255.0f);
};

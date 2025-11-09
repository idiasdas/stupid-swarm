#pragma once

#include "custom_imgui.hpp"

class SwarmSettingsImgui : public CustomImgui {
public:
    SwarmSettingsImgui(GLFWwindow* window, OpenGLContext* context);
    void Update() override;
    void Reset();

    void SetNbParticles(int nbParticles) { _nbParticles = (nbParticles > _maxNbParticles) ? _maxNbParticles : nbParticles; }
    int GetNbParticles() { return _nbParticles; }
    glm::vec3 GetParticlesColor() { return glm::vec3({ _colorParticles.x, _colorParticles.y, _colorParticles.z }); }
    glm::vec3 GetGoalColor() { return glm::vec3({ _colorGoal.x, _colorGoal.y, _colorGoal.z }); }

    bool IsPaused() { return _paused; }
    bool IsReset() { return _reset; }

private:
    bool _paused = true;
    bool _reset = false;
    int _nbParticles;
    int _maxNbParticles = 10000;
    ImVec4 _colorParticles = ImVec4(.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 250.0f / 255.0f);
    ImVec4 _colorGoal = ImVec4(255.f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 250.0f / 255.0f);
};

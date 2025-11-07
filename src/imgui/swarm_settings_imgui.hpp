#pragma once

#include "custom_imgui.hpp"

class SwarmSettingsImgui : public CustomImgui {
public:
    using CustomImgui::CustomImgui;
    void Update() override;
    bool IsPaused() { return _paused; }
    void SetNbParticles(int nbParticles) { _nbParticles = (nbParticles > _maxNbParticles) ? _maxNbParticles : nbParticles; }
    int GetNbParticles() { return _nbParticles; }

private:
    bool _paused = true;
    int _nbParticles;
    int _maxNbParticles = 10000;
};

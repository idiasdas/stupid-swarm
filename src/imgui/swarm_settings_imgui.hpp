#pragma once

#include "custom_imgui.hpp"

class SwarmSettingsImgui : public CustomImgui {
public:
    using CustomImgui::CustomImgui;
    void update() override;
    bool is_paused() { return _paused; }
    void set_nb_particles(int nb_particles) { _nb_particles = (nb_particles > _max_nb_particles) ? _max_nb_particles : nb_particles; }
    int get_nb_particles() { return _nb_particles; }

private:
    bool _paused = true;
    int _nb_particles;
    int _max_nb_particles = 10000;
};

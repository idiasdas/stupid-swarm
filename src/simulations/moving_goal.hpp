#pragma once

#include "imgui/swarm_settings_imgui.hpp"
#include "particle.hpp"
#include "renderer/camera.h"
#include "renderer/model.h"
#include "renderer/opengl-context.h"
#include "renderer/shader.h"
#include <random>

class MovingGoalSimulation {

public:
    MovingGoalSimulation();
    void Run();
    void Reset();

private:
    OpenGLContext _context;
    Camera _camera;
    Model _axesLines;
    std::vector<Particle> _particles;
    Particle _goal;
    Shader _colorShader;
    double _lastTime;
    int _framesCount;
    int _nbParticlesAlive;
    glm::vec2 _goalDirection;
    SwarmSettingsImgui _gui;
};

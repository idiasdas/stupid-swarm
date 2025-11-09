#include "moving_goal.hpp"

#include "log.h"

Camera* gCamera = nullptr;
void EventManager(Event& event)
{
    gCamera->OnEvent(event);
    if (event.GetEventType() != EventType::MOUSE_MOVE)
        LOG_INFO(event.ToString());
}

MovingGoalSimulation::MovingGoalSimulation()
    : _context(OpenGLContext("Stupid Swarm", 1280, 720, EventManager))
    , _camera(Camera(&_context))
    , _goal(Particle({ 10.f, 10.f }, 0.4f, ParticleType::GOAL))
    , _colorShader(Shader("shaders/color.vertexShader", "shaders/color.fragmentShader"))
    , _gui(_context.GetWindowHandle(), &_context)
{
    gCamera = &_camera;
    _gui.SetNbParticles(10000);
    std::random_device rd; // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(-1.f, 1.f);
    std::vector<float> bufferLines = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 1.0f
    };

    _axesLines.BufferVertices(bufferLines);
    _axesLines.BufferIndices({ 0, 1, 2, 3, 4, 5 });
    int nbCircles = 10000;
    float alpha = 0.0f;
    float radius = 0.0f;
    for (int i = 0; i < nbCircles; i++) {
        _particles.push_back(Particle({ radius * glm::cos(alpha), radius * glm::sin(alpha) }, 0.1f, ParticleType::DRONE));
        radius += 0.001f;
        alpha += 2 * glm::pi<float>() / 180;
    }
    _lastTime = glfwGetTime();
    _framesCount = 0;
    _nbParticlesAlive = 0;
    _goalDirection = { -1.f, -1.f };
}
void MovingGoalSimulation::Run()
{
    static glm::vec3 lastParticesColor = _gui.GetParticlesColor();
    static glm::vec3 lastGoalColor = _gui.GetParticlesColor();
    do {
        _framesCount++;
        double curTime = glfwGetTime();
        double delta = curTime - _lastTime;

        if (delta > 1.0f) {
            LOG_INFO("FPS: {0}", _framesCount / delta);
            _lastTime = curTime;
            _framesCount = 0;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _axesLines.DrawLines(_colorShader, _camera.GetProjectionMatrix() * _camera.GetViewMatrix() * _axesLines.GetModelMatrix());

        if (!_gui.IsPaused()) {
            if (glm::distance(_goal.GetPosition(), { 0.f, 0.f }) >= 10.f)
                _goalDirection = glm::normalize(glm::vec2({ 0.f, 0.f }) - _goal.GetPosition());

            _goal.MoveTowards(_goalDirection, 5.f);
            _goal.RotateAround(1.f, { 0.f, 0.f, 1.f }, 0.2f);
            for (auto& particle : _particles) {
                glm::vec2 direction = glm::normalize(_goal.GetPosition() - particle.GetPosition());
                particle.MoveTowards(direction, 2.f);
                if (particle.IsEnabled() && glm::distance(particle.GetPosition(), _goal.GetPosition()) < _goal.GetSize()) {
                    particle.Kill();
                    _gui.SetNbParticles(_gui.GetNbParticles() - 1);
                    LOG_INFO("KILL");
                }
            }
        }

        _nbParticlesAlive = 0;
        for (auto& particle : _particles) {
            if (particle.IsEnabled())
                _nbParticlesAlive++;
        }

        auto it = _particles.begin();
        while (it != _particles.end() && _nbParticlesAlive != _gui.GetNbParticles()) {
            if (_nbParticlesAlive < _gui.GetNbParticles() && !(*it).IsEnabled()) {
                (*it).Enable();
                _nbParticlesAlive++;
            } else if (_nbParticlesAlive > _gui.GetNbParticles() && (*it).IsEnabled()) {
                (*it).Kill();
                _nbParticlesAlive--;
            }
            it++;
        }

        _goal.Update();
        _goal.Draw(_colorShader, _camera.GetProjectionMatrix() * _camera.GetViewMatrix() * _goal.GetModelMatrix());
        if (lastParticesColor != _gui.GetParticlesColor()) {
            lastParticesColor = _gui.GetParticlesColor();
            for (auto& particle : _particles)
                particle.UpdateVerticesColor(lastParticesColor);
        }
        if (lastGoalColor != _gui.GetGoalColor()) {
            lastGoalColor = _gui.GetGoalColor();
            _goal.UpdateVerticesColor(lastGoalColor);
        }
        for (auto& particle : _particles) {
            particle.Update();
            particle.Draw(_colorShader, _camera.GetProjectionMatrix() * _camera.GetViewMatrix() * particle.GetModelMatrix());
        }

        _gui.NewFrame();
        _gui.Update();
        _gui.Draw();

        glfwSwapBuffers(_context.GetWindowHandle());
        glfwPollEvents();
    } while (glfwGetKey(_context.GetWindowHandle(), GLFW_KEY_Q) != GLFW_PRESS && glfwWindowShouldClose(_context.GetWindowHandle()) == 0);

    _gui.Shutdown();
    glfwTerminate();
}

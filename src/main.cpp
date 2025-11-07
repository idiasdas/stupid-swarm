#include <glad/glad.h>
#include <random>

#include "imgui/swarm_settings_imgui.hpp"
#include "log.h"
#include "particle.hpp"
#include "renderer/camera.h"
#include "renderer/model.h"
#include "renderer/opengl-context.h"
#include "renderer/shader.h"

Camera* gCamera = nullptr;
void EventManager(Event& event)
{
    gCamera->OnEvent(event);
    if (event.GetEventType() != EventType::MOUSE_MOVE)
        LOG_INFO(event.ToString());
}

int main()
{
    Log::Init();
    OpenGLContext context("Stupid Swarm", 1280, 720, EventManager);

    Camera camera(&context);
    gCamera = &camera;

    Model axes_lines;
    SwarmSettingsImgui gui(context.GetWindowHandle(), &context);
    gui.SetNbParticles(10000);

    std::random_device rd; // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(-1.f, 1.f);

    std::vector<float> buffer_lines = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 1.0f
    };

    axes_lines.BufferVertices(buffer_lines);
    axes_lines.BufferIndices({ 0, 1, 2, 3, 4, 5 });
    std::vector<Particle> particles;
    int nb_circles = 10000;
    float alpha = 0.0f;
    float radius = 0.0f;
    for (int i = 0; i < nb_circles; i++) {
        particles.push_back(Particle({ radius * glm::cos(alpha), radius * glm::sin(alpha) }, 0.1f, ParticleType::DRONE));
        radius += 0.001f;
        alpha += 2 * glm::pi<float>() / 180;
    }
    Particle goal({ 10.f, 10.f }, 0.4f, ParticleType::GOAL);

    Shader color_shader("shaders/color.vertexShader", "shaders/color.fragmentShader");
    double last_time = glfwGetTime();
    int frames_count = 0;
    int nb_particles_alive = 0;

    glm::vec2 goal_direction = { -1.f, -1.f };
    do {
        frames_count++;
        double cur_time = glfwGetTime();
        double delta = cur_time - last_time;

        if (delta > 1.0f) {
            LOG_INFO("FPS: {0}", frames_count / delta);
            last_time = cur_time;
            frames_count = 0;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        axes_lines.DrawLines(color_shader, camera.GetProjectionMatrix() * camera.GetViewMatrix() * axes_lines.GetModelMatrix());

        if (!gui.IsPaused()) {
            if (glm::distance(goal.get_position(), { 0.f, 0.f }) >= 10.f)
                goal_direction = glm::normalize(glm::vec2({ 0.f, 0.f }) - goal.get_position());

            goal.move_towards(goal_direction, 5.f);
            goal.rotate_around(1.f, { 0.f, 0.f, 1.f }, 0.2f);
            for (auto& particle : particles) {
                glm::vec2 direction = glm::normalize(goal.get_position() - particle.get_position());
                particle.move_towards(direction, 2.f);
                // particle.rotate_around(10.f, { 0.f, 0.f, 1.f }, 0.5f);
                if (particle.is_enabled() && glm::distance(particle.get_position(), goal.get_position()) < goal.get_size()) {
                    particle.kill();
                    gui.SetNbParticles(gui.GetNbParticles() - 1);
                    LOG_INFO("KILL");
                }
            }
        }

        nb_particles_alive = 0;
        for (auto& particle : particles) {
            if (particle.is_enabled())
                nb_particles_alive++;
        }

        auto it = particles.begin();
        while (it != particles.end() && nb_particles_alive != gui.GetNbParticles()) {
            if (nb_particles_alive < gui.GetNbParticles() && !(*it).is_enabled()) {
                (*it).enable();
                nb_particles_alive++;
            } else if (nb_particles_alive > gui.GetNbParticles() && (*it).is_enabled()) {
                (*it).kill();
                nb_particles_alive--;
            }
            it++;
        }

        // for (int i = 0; i < particles.size(); i++) {
        //     if (i < gui.get_nb_particles())
        //         particles[i].enable();
        //     else
        //         particles[i].kill();
        // }

        goal.update();
        goal.draw(color_shader, camera.GetProjectionMatrix() * camera.GetViewMatrix() * goal.get_model_matrix());
        for (auto& particle : particles) {
            particle.update();
            particle.draw(color_shader, camera.GetProjectionMatrix() * camera.GetViewMatrix() * particle.get_model_matrix());
        }

        gui.NewFrame();
        gui.Update();
        gui.Draw();

        glfwSwapBuffers(context.GetWindowHandle());
        glfwPollEvents();

    } while (glfwGetKey(context.GetWindowHandle(), GLFW_KEY_Q) != GLFW_PRESS && glfwWindowShouldClose(context.GetWindowHandle()) == 0);

    gui.Shutdown();
    glfwTerminate();

    return 0;
}

#include <glad/glad.h>
#include <random>

#include "imgui/custom_imgui.hpp"
#include "log.h"
#include "particle.hpp"
#include "renderer/camera.h"
#include "renderer/model.h"
#include "renderer/opengl-context.h"
#include "renderer/shader.h"

Camera* g_camera = nullptr;
void event_manager(Event& event)
{
    g_camera->on_event(event);
    if (event.get_event_type() != EventType::mouse_move)
        LOG_INFO(event.to_string());
}

int main()
{
    Log::init();
    OpenGLContext context("Stupid Swarm", 1280, 720, event_manager);

    Camera camera(&context);
    g_camera = &camera;

    Model axes_lines;
    CustomImgui gui(context.get_window_handle(), &context);

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

    axes_lines.buffer_vertices(buffer_lines);
    axes_lines.buffer_indices({ 0, 1, 2, 3, 4, 5 });
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

        axes_lines.draw_lines(color_shader, camera.get_projection_matrix() * camera.get_view_matrix() * axes_lines.get_model_matrix());

        if (glm::distance(goal.get_position(), { 0.f, 0.f }) >= 10.f)
            goal_direction = glm::normalize(glm::vec2({ 0.f, 0.f }) - goal.get_position());

        goal.move_towards(goal_direction, 5.f);
        goal.rotate_around(1.f, { 0.f, 0.f, 1.f }, 0.2f);
        goal.update();
        goal.draw(color_shader, camera.get_projection_matrix() * camera.get_view_matrix() * goal.get_model_matrix());
        for (auto& particle : particles) {
            glm::vec2 direction = glm::normalize(goal.get_position() - particle.get_position());
            particle.move_towards(direction, 2.f);
            // particle.rotate_around(10.f, { 0.f, 0.f, 1.f }, 0.5f);
            if (glm::distance(particle.get_position(), goal.get_position()) < 0.4f) {
                particle.kill();
                LOG_INFO("KILL");
            }
            particle.update();
            particle.draw(color_shader, camera.get_projection_matrix() * camera.get_view_matrix() * particle.get_model_matrix());
        }

        gui.new_frame();
        gui.update();
        gui.draw();

        glfwSwapBuffers(context.get_window_handle());
        glfwPollEvents();

    } while (glfwGetKey(context.get_window_handle(), GLFW_KEY_Q) != GLFW_PRESS && glfwWindowShouldClose(context.get_window_handle()) == 0);

    gui.shutdown();
    glfwTerminate();

    return 0;
}

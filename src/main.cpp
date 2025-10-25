#include <glad/glad.h>

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
    LOG_INFO(event.to_string());
}

int main()
{
    Log::init();
    OpenGLContext context("Stupid Swarm", 1280, 720, event_manager);

    Camera camera(&context);
    g_camera = &camera;

    Model axes_lines;

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
    int nb_circles = 100;
    float alpha = 0.0f;
    float radius = 0.0f;
    for (int i = 0; i < nb_circles; i++) {
        particles.push_back(Particle({ radius * glm::cos(alpha), radius * glm::sin(alpha) }, 0.1f, ParticleType::DRONE));
        radius += 0.1f;
        alpha += 2 * glm::pi<float>() / 24;
    }

    Shader color_shader("shaders/color.vertexShader", "shaders/color.fragmentShader");

    double last_time = glfwGetTime();
    int frames_count = 0;

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

        glm::mat4 MVP = camera.get_projection_matrix() * camera.get_view_matrix() * axes_lines.get_model_matrix();
        axes_lines.draw_lines(color_shader, MVP);
        for (auto circle : particles)
            circle.draw(color_shader, MVP);
        glfwSwapBuffers(context.get_window_handle());
        glfwPollEvents();

    } while (glfwGetKey(context.get_window_handle(), GLFW_KEY_Q) != GLFW_PRESS && glfwWindowShouldClose(context.get_window_handle()) == 0);

    glfwTerminate();

    return 0;
}

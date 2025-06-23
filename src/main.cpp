#include <glad/glad.h>

#include "log.h"
#include "renderer/camera.h"
#include "renderer/model.h"
#include "renderer/opengl-context.h"
#include "renderer/shader.h"

void event_manager(Event& event) { LOG_INFO(event.to_string()); }

int main()
{
    Log::init();
    OpenGLContext context("Stupid Swarm", 1280, 720, event_manager);

    Camera camera(&context);

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

        axes_lines.draw_lines(color_shader, camera.get_projection_matrix() * camera.get_view_matrix() * axes_lines.get_model_matrix());

        glfwSwapBuffers(context.get_window_handle());
        glfwPollEvents();

    } while (glfwGetKey(context.get_window_handle(), GLFW_KEY_Q) != GLFW_PRESS && glfwWindowShouldClose(context.get_window_handle()) == 0);

    glfwTerminate();

    return 0;
}

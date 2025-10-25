#include "particle.hpp"

#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
#include "log.h"

#include <cstdint>
#include <vector>

#define PI glm::pi<float>()
#define NB_SLICES 20

Particle::Particle(glm::vec2 position, const float size, ParticleType type)
    : m_position(position)
    , m_initial_position(position)
    , m_type(type)
    , m_last_update_time(glfwGetTime())
{
    glm::vec3 color;
    switch (type) {
    case ParticleType::DRONE:
        color = { 0.f, 1.f, 0.f };
        break;
    case ParticleType::GOAL:
        color = { 1.f, 0.f, 0.f };
        break;
    case ParticleType::OBSTACLE:
        color = { 1.f, 1.f, 1.f };
        break;
    }

    std::vector<float> vertices_buffer;
    for (int i = 0; i < NB_SLICES; i++) {
        const float alpha = 2 * PI / NB_SLICES;
        vertices_buffer.push_back(position[0]);
        vertices_buffer.push_back(position[1]);
        vertices_buffer.push_back(0.0f);
        vertices_buffer.push_back(color[0]);
        vertices_buffer.push_back(color[1]);
        vertices_buffer.push_back(color[2]);
        vertices_buffer.push_back(position[0] + size * glm::cos(i * alpha));
        vertices_buffer.push_back(position[1] + size * glm::sin(i * alpha));
        vertices_buffer.push_back(0.0f);
        vertices_buffer.push_back(color[0]);
        vertices_buffer.push_back(color[1]);
        vertices_buffer.push_back(color[2]);
        vertices_buffer.push_back(position[0] + size * glm::cos((i + 1) * alpha));
        vertices_buffer.push_back(position[1] + size * glm::sin((i + 1) * alpha));
        vertices_buffer.push_back(0.0f);
        vertices_buffer.push_back(color[0]);
        vertices_buffer.push_back(color[1]);
        vertices_buffer.push_back(color[2]);
    }
    m_model.buffer_vertices(vertices_buffer);

    std::vector<uint32_t> indices_buffer;
    for (uint32_t i = 0; i < (uint32_t)vertices_buffer.size() / 6; i++) {
        indices_buffer.push_back(i);
    }
    m_model.buffer_indices(indices_buffer);

    LOG_INFO("Initialized Particle");
}

void Particle::draw(const Shader& shader, const glm::mat4 MVP) const
{
    m_model.draw(shader, MVP);
}

void Particle::move_towards(glm::vec2 direction, float speed)
{
    double cur_time = glfwGetTime();
    double delta = cur_time - m_last_update_time;
    glm::vec2 movement = { delta * direction[0] * speed, delta * direction[1] * speed };
    m_model.translate({ movement[0], movement[1], 0.f });
    m_position = m_position + movement;
}

void Particle::translate(glm::vec2 position)
{
    m_model.translate({ position[0], position[1], 0.f });
    m_position = position;
}

void Particle::rotate(const float rotation_degree, const glm::vec3& rotation_axis)
{
    m_model.rotate(rotation_degree, rotation_axis);
    glm::vec4 position = { m_position[0], m_position[1], 0.f, 0.f };
    position = m_model.get_model_matrix() * position;
    m_position = { position[0], position[1] };
}

void Particle::rotate_towards(const float rotation_degree, const glm::vec3& rotation_axis, const float speed)
{
    double cur_time = glfwGetTime();
    double delta = cur_time - m_last_update_time;
    rotate(rotation_degree * delta * speed, rotation_axis);
}

void Particle::update()
{
    m_last_update_time = glfwGetTime();
}

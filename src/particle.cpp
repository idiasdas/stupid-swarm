#include "particle.hpp"

#include "glm/fwd.hpp"

#include <cstdint>
#include <vector>

#define PI glm::pi<float>()
#define NB_SLICES 20

Particle::Particle(glm::vec2 position, const float size, ParticleType type)
    : m_position(position)
    , m_type(type)
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
    m_circle.buffer_vertices(vertices_buffer);

    std::vector<uint32_t> indices_buffer;
    for (uint32_t i = 0; i < (uint32_t)vertices_buffer.size() / 6; i++) {
        indices_buffer.push_back(i);
    }
    m_circle.buffer_indices(indices_buffer);
}

void Particle::draw(const Shader& shader, const glm::mat4 MVP) const
{
    m_circle.draw(shader, MVP);
}

void Particle::move(const glm::vec2 direction)
{
    // m_circle
}

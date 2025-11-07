#include "particle.hpp"

#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdint>
#include <vector>

#define PI glm::pi<float>()
#define NB_SLICES 20

Particle::Particle(glm::vec2 position, const float size, ParticleType type)
    : _enabled(true)
    , _initialPosition(position)
    , _type(type)
    , _lastUpdateTime(glfwGetTime())
    , _size(size)
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
        vertices_buffer.push_back(0.0f);
        vertices_buffer.push_back(0.0f);
        vertices_buffer.push_back(0.0f);
        vertices_buffer.push_back(color[0]);
        vertices_buffer.push_back(color[1]);
        vertices_buffer.push_back(color[2]);
        vertices_buffer.push_back(0.0f + size * glm::cos(i * alpha));
        vertices_buffer.push_back(0.0f + size * glm::sin(i * alpha));
        vertices_buffer.push_back(0.0f);
        vertices_buffer.push_back(color[0]);
        vertices_buffer.push_back(color[1]);
        vertices_buffer.push_back(color[2]);
        vertices_buffer.push_back(0.0f + size * glm::cos((i + 1) * alpha));
        vertices_buffer.push_back(0.0f + size * glm::sin((i + 1) * alpha));
        vertices_buffer.push_back(0.0f);
        vertices_buffer.push_back(color[0]);
        vertices_buffer.push_back(color[1]);
        vertices_buffer.push_back(color[2]);
    }
    _model.BufferVertices(vertices_buffer);

    std::vector<uint32_t> indices_buffer;
    for (uint32_t i = 0; i < (uint32_t)vertices_buffer.size() / 6; i++) {
        indices_buffer.push_back(i);
    }
    _model.BufferIndices(indices_buffer);
    _model.Translate({ position[0], position[1], 0.f });
}

void Particle::Draw(const Shader& shader, const glm::mat4 MVP) const
{
    if (!_enabled)
        return;
    _model.Draw(shader, MVP);
}

void Particle::MoveTowards(glm::vec2 direction, float speed)
{
    if (!_enabled)
        return;
    double cur_time = glfwGetTime();
    double delta = cur_time - _lastUpdateTime;
    glm::vec2 movement = { delta * direction[0] * speed, delta * direction[1] * speed };
    _model.Translate({ movement[0], movement[1], 0.f });
}

void Particle::Translate(glm::vec2 position)
{
    if (!_enabled)
        return;
    _model.Translate({ position[0], position[1], 0.f });
}

void Particle::Rotate(const float rotationDegree, const glm::vec3& rotationAxis)
{
    if (!_enabled)
        return;
    _model.Rotate(rotationDegree, rotationAxis);
}

void Particle::RotateAround(const float rotationDegree, const glm::vec3& rotationAxis, const float speed)
{
    if (!_enabled)
        return;
    double cur_time = glfwGetTime();
    double delta = cur_time - _lastUpdateTime;
    _model.SetTranslationMatrix(glm::rotate(glm::mat4(1), (float)(rotationDegree * delta * speed), rotationAxis) * _model.GetTranslationMatrix());
}

void Particle::Update()
{
    if (!_enabled)
        return;
    _lastUpdateTime = glfwGetTime();
}

glm::vec2 Particle::GetPosition() const
{
    return glm::vec2(_model.GetModelMatrix()[3]);
}

#pragma once

#include "glm/fwd.hpp"
#include "renderer/model.h"

enum class ParticleType {
    DRONE,
    GOAL,
    OBSTACLE
};

class Particle {

public:
    Particle(glm::vec2 position, const float size, ParticleType type);
    void draw(const Shader& shader, const glm::mat4 MVP) const;
    void translate(glm::vec2 position);
    void move_towards(glm::vec2 direction, float speed);
    void rotate(const float rotation_degree, const glm::vec3& rotation_axis);
    void rotate_around(const float rotation_degree, const glm::vec3& rotation_axis, const float speed);
    void kill() { m_enabled = false; }
    void enable() { m_enabled = true; }
    glm::mat4 get_model_matrix() const { return m_model.GetModelMatrix(); }
    glm::vec2 get_position() const;
    glm::vec2 get_initial_position() const { return m_initial_position; }
    bool is_enabled() { return m_enabled; }
    void update();
    float get_size() { return m_size; }

private:
    bool m_enabled;
    Model m_model;
    float m_size;
    glm::vec2 m_initial_position;
    ParticleType m_type;
    double m_last_update_time;
};

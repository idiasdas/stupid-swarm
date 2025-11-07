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
    void Draw(const Shader& shader, const glm::mat4 MVP) const;
    void Translate(glm::vec2 position);
    void MoveTowards(glm::vec2 direction, float speed);
    void Rotate(const float rotationDegree, const glm::vec3& rotationAxis);
    void RotateAround(const float rotationDegree, const glm::vec3& rotationAxis, const float speed);
    void Kill() { _enabled = false; }
    void Enable() { _enabled = true; }
    glm::mat4 GetModelMatrix() const { return _model.GetModelMatrix(); }
    glm::vec2 GetPosition() const;
    glm::vec2 GetInitialPosition() const { return _initialPosition; }
    bool IsEnabled() { return _enabled; }
    void Update();
    float GetSize() { return _size; }

private:
    bool _enabled;
    Model _model;
    float _size;
    glm::vec2 _initialPosition;
    ParticleType _type;
    double _lastUpdateTime;
};

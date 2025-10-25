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
    void move(const glm::vec2 direction);

private:
    Model m_circle;
    glm::vec2 m_position;
    ParticleType m_type;
};

#include "circle.h"
#include "glm/fwd.hpp"

#include <cstdint>
#include <vector>

#define PI glm::pi<float>()
#define NB_SLICES 20

Model create_circle(const glm::vec2 pos, const float radius, const glm::vec3 color)
{
    Model circle;
    std::vector<float> vertices_buffer;
    for (int i = 0; i < NB_SLICES; i++) {
        const float alpha = 2 * PI / NB_SLICES;
        vertices_buffer.push_back(pos[0]);
        vertices_buffer.push_back(pos[1]);
        vertices_buffer.push_back(0.0f);
        vertices_buffer.push_back(color[0]);
        vertices_buffer.push_back(color[1]);
        vertices_buffer.push_back(color[2]);
        vertices_buffer.push_back(pos[0] + radius * glm::cos(i * alpha));
        vertices_buffer.push_back(pos[1] + radius * glm::sin(i * alpha));
        vertices_buffer.push_back(0.0f);
        vertices_buffer.push_back(color[0]);
        vertices_buffer.push_back(color[1]);
        vertices_buffer.push_back(color[2]);
        vertices_buffer.push_back(pos[0] + radius * glm::cos((i + 1) * alpha));
        vertices_buffer.push_back(pos[1] + radius * glm::sin((i + 1) * alpha));
        vertices_buffer.push_back(0.0f);
        vertices_buffer.push_back(color[0]);
        vertices_buffer.push_back(color[1]);
        vertices_buffer.push_back(color[2]);
    }
    circle.buffer_vertices(vertices_buffer);

    std::vector<uint32_t> indices_buffer;
    for (uint32_t i = 0; i < (uint32_t)vertices_buffer.size() / 6; i++) {
        indices_buffer.push_back(i);
    }
    circle.buffer_indices(indices_buffer);

    return circle;
}

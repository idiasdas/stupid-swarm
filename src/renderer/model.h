#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

class Model {
public:
    Model();

    void BufferVertices(const std::vector<float>& buffer);
    void UpdateBufferVertices(const std::vector<float>& buffer);
    void BufferIndices(const std::vector<uint32_t>& buffer);
    void Draw(const Shader& shader, const glm::mat4 MVP) const;
    void DrawLines(const Shader& shader, const glm::mat4 MVP) const;
    void Scale(const glm::vec3& scaleVector);
    void Rotate(const float rotationDegree, const glm::vec3& rotationAxis);
    void Translate(const glm::vec3& translationVector);
    void SetScaleMatrix(const glm::mat4& scaleMatrix) { _scaleMatrix = scaleMatrix; }
    void SetRotationMatrix(const glm::mat4& rotationMatrix) { _rotationMatrix = rotationMatrix; }
    void SetTranslationMatrix(const glm::mat4& translationMatrix) { _translationMatrix = translationMatrix; }

    glm::mat4 GetScaleMatrix() const { return _scaleMatrix; }
    glm::mat4 GetRotationMatrix() const { return _rotationMatrix; }
    glm::mat4 GetTranslationMatrix() const { return _translationMatrix; }
    glm::mat4 GetModelMatrix() const { return _translationMatrix * _rotationMatrix * _scaleMatrix; }

private:
    uint32_t _vertexArrayID;
    uint32_t _vertexBufferID;
    uint32_t _indexBufferID;

    uint32_t _indicesCount;

    glm::mat4 _scaleMatrix;
    glm::mat4 _rotationMatrix;
    glm::mat4 _translationMatrix;
};

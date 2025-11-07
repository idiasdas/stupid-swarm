#include "model.h"

Model::Model()
{
    glGenVertexArrays(1, &_vertexArrayID);
    _scaleMatrix = glm::mat4(1);
    _rotationMatrix = glm::mat4(1);
    _translationMatrix = glm::mat4(1);
    _indicesCount = 0;
}

void Model::BufferVertices(const std::vector<float>& buffer)
{
    glBindVertexArray(_vertexArrayID);
    glCreateBuffers(1, &_vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), &buffer[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
    glVertexAttribPointer(
        0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        6 * sizeof(float), // stride
        (void*)0 // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::UpdateBufferVertices(const std::vector<float>& buffer)
{
    glBindVertexArray(_vertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), &buffer[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::BufferIndices(const std::vector<uint32_t>& buffer)
{
    _indicesCount = buffer.size();

    glBindVertexArray(_vertexArrayID);
    glCreateBuffers(1, &_indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicesCount * sizeof(uint32_t), buffer.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::Draw(const Shader& shader, const glm::mat4 MVP) const
{
    shader.Bind();
    uint32_t matrixID = shader.GetUniformLocation("MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    glBindVertexArray(_vertexArrayID);
    glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Model::DrawLines(const Shader& shader, const glm::mat4 MVP) const
{
    shader.Bind();
    uint32_t matrixID = shader.GetUniformLocation("MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    glBindVertexArray(_vertexArrayID);
    glDrawElements(GL_LINES, _indicesCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Model::Scale(const glm::vec3& scaleVector)
{
    _scaleMatrix = glm::scale(glm::mat4(1), scaleVector) * _scaleMatrix;
}

void Model::Rotate(const float rotationDegree, const glm::vec3& rotationAxis)
{
    _rotationMatrix = glm::rotate(glm::mat4(1), rotationDegree, rotationAxis) * _rotationMatrix;
}

void Model::Translate(const glm::vec3& translationVector)
{
    _translationMatrix = glm::translate(glm::mat4(1), translationVector) * _translationMatrix;
}

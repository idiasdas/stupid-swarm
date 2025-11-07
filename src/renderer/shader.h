#pragma once

#include <glad/glad.h>
#include <stdlib.h>
#include <string>

#ifndef EXECUTABLE_UTILS_PATH
#define EXECUTABLE_UTILS_PATH "utils/"
#endif

class Shader {
public:
    Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
    void Bind() const;
    void Unbind() const;
    uint32_t GetUniformLocation(const char* uniformName) const { return glGetUniformLocation(_rendererID, uniformName); }

private:
    uint32_t _rendererID;
};

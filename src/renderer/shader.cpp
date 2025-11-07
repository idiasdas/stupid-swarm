#include "shader.h"
#include "log.h"

#include <fstream>

Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
    uint32_t vertezShaderID = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(EXECUTABLE_UTILS_PATH + vertexFilePath, std::ios::in);
    if (vertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << vertexShaderStream.rdbuf();
        vertexShaderCode = sstr.str();
        vertexShaderStream.close();
    } else {
        LOG_ERROR("Impossible to open {0}.", EXECUTABLE_UTILS_PATH + vertexFilePath);
        getchar();
        exit(EXIT_FAILURE);
    }

    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(EXECUTABLE_UTILS_PATH + fragmentFilePath, std::ios::in);
    if (fragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << fragmentShaderStream.rdbuf();
        fragmentShaderCode = sstr.str();
        fragmentShaderStream.close();
    } else {
        LOG_ERROR("Impossible to open {0}.", EXECUTABLE_UTILS_PATH + fragmentFilePath);
        getchar();
        exit(EXIT_FAILURE);
    }

    int result = GL_FALSE;
    int infoLogLength;

    // Compile Vertex Shader
    LOG_INFO("Compiling shader: {0}", EXECUTABLE_UTILS_PATH + vertexFilePath);
    char const* vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertezShaderID, 1, &vertexSourcePointer, NULL);
    glCompileShader(vertezShaderID);

    // Check Vertex Shader
    glGetShaderiv(vertezShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertezShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> vertexShaderErrorMsg(infoLogLength + 1);
        glGetShaderInfoLog(vertezShaderID, infoLogLength, NULL, &vertexShaderErrorMsg[0]);
        printf("%s\n", &vertexShaderErrorMsg[0]);
    }

    // Compile Fragment Shader
    LOG_INFO("Compiling shader: {0}", EXECUTABLE_UTILS_PATH + fragmentFilePath);
    char const* fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
    glCompileShader(fragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> fragmentShaderErrorMsg(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMsg[0]);
        LOG_ERROR("{0}", fragmentShaderErrorMsg.data());
    }

    // Link the program
    LOG_INFO("Linking program.");
    uint32_t programID = glCreateProgram();
    glAttachShader(programID, vertezShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
        LOG_ERROR("{0}", ProgramErrorMessage.data());
    }

    glDetachShader(programID, vertezShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertezShaderID);
    glDeleteShader(fragmentShaderID);

    _rendererID = programID;
}

void Shader::Bind() const
{
    glUseProgram(_rendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

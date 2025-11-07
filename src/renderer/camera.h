#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "events.h"
#include "opengl-context.h"

enum class CameraState {
    WAITINPUT,
    MOVE
};

class Camera {
private:
    OpenGLContext* _openGLContext;

    float _horizontalAngle;
    float _verticalAngle;
    float _radius;
    float _FoV;
    float _mouseSensitivity;

    glm::mat4 _viewMatrix;
    glm::mat4 _projectionMatrix;
    glm::vec3 _position;

public:
    Camera(OpenGLContext* const openGLContext);
    glm::mat4 GetViewMatrix() const { return _viewMatrix; }
    glm::mat4 GetProjectionMatrix() const { return _projectionMatrix; }
    glm::vec3 GetCameraPosition() const { return _position; }
    void OnEvent(Event& event);
    void SphericalMove(double horizontalMove, double verticalMove, double radiusMove);
};

#include "camera.h"

#define PI glm::pi<float>()

Camera::Camera(OpenGLContext* const openGLContext)
{
    _openGLContext = openGLContext;
    _horizontalAngle = 0.0f;
    _verticalAngle = 0.0f;
    _radius = 15.0f;
    _FoV = 45.0f;
    _position = glm::vec3(0.f, 0.f, 15.f);
    float aspect = (float)openGLContext->GetWindowWidth() / (float)openGLContext->GetWindowHeight();
    _projectionMatrix = glm::perspective(glm::radians(_FoV), aspect, 0.1f, 120.0f);
    _mouseSensitivity = 0.005f;
    SphericalMove(0, 0, 0);
}

void Camera::OnEvent(Event& event)
{
    static CameraState sState = CameraState::WAITINPUT;
    static float sMouseXPos = 0.0f;
    static float sMouseYPos = 0.0f;

    if (event.GetEventType() == EventType::MOUSE_MOVE) {
        if (sState == CameraState::MOVE) {
            float xPos = ((MouseMoveEvent*)&event)->GetX();
            float yPos = ((MouseMoveEvent*)&event)->GetY();
            SphericalMove(_mouseSensitivity * (sMouseXPos - xPos), -_mouseSensitivity * (sMouseYPos - yPos), 0);
            sMouseXPos = xPos;
            sMouseYPos = yPos;
        }
    } else if (event.GetEventType() == EventType::MOUSE_BUTTON_RELEASE) {
        sState = CameraState::WAITINPUT;
    } else if (event.GetEventType() == EventType::MOUSE_BUTTON_PRESS) {
        if (((MouseButtonPressEvent*)&event)->GetButton() == GLFW_MOUSE_BUTTON_2) {
            sState = CameraState::MOVE;
            sMouseXPos = ((MouseButtonPressEvent*)&event)->GetXPos();
            sMouseYPos = ((MouseButtonPressEvent*)&event)->GetYPos();
        }
    } else if (event.GetEventType() == EventType::MOUSE_SCROLL) {
        SphericalMove(0, 0, -((MouseScrollEvent*)&event)->GetYOffset());
    } else if (event.GetEventType() == EventType::WINDOW_RESIZE) {
        float aspect = (float)_openGLContext->GetWindowWidth() / (float)_openGLContext->GetWindowHeight();
        _projectionMatrix = glm::perspective(glm::radians(_FoV), aspect, 0.1f, 120.0f);
    }
}

void Camera::SphericalMove(double horizontalMove, double verticalMove, double radiusMove)
{
    _horizontalAngle += horizontalMove;
    _verticalAngle += verticalMove;
    _radius += 2 * radiusMove;
    _verticalAngle = std::min(_verticalAngle, PI / 2.f);
    _verticalAngle = std::max(_verticalAngle, -PI / 2.f);

    if (_radius < 10.f)
        _radius = 10.f;
    else if (_radius > 100.f)
        _radius = 100.f;

    _position = glm::vec3(_radius * cos(_verticalAngle) * sin(_horizontalAngle),
        _radius * sin(_verticalAngle),
        _radius * cos(_verticalAngle) * cos(_horizontalAngle));

    glm::vec3 right = glm::vec3(
        sin(_horizontalAngle - PI / 2.0f),
        0,
        cos(_horizontalAngle - PI / 2.0f));

    glm::vec3 up = glm::cross(right, glm::normalize(_position));

    _viewMatrix = glm::lookAt(
        _position, // Camera is here
        -_position, // and looks towards the origin
        up // Head is up
    );
}

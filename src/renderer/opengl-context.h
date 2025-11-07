#pragma once

#include <string>

#include "events.h"
#include "glad/glad.h"

#include "GLFW/glfw3.h"

class OpenGLContext {
public:
    OpenGLContext(const std::string& windowName, const int windowWidth, const int windowHeight, void (*funcEventManager)(Event& event));

    inline std::string GetWindowName() const { return _windowName; }
    inline int GetWindowWidth() const { return _windowWidth; }
    inline int GetWindowHeight() const { return _windowHeight; }
    inline void SetWindowWidth(int windowWidth) { _windowWidth = windowWidth; }
    inline void SetWindowHeight(int windowHeight) { _windowHeight = windowHeight; }
    inline GLFWwindow* GetWindowHandle() const { return _window; }

private:
    void SetEventsCallbacks();
    void RunEventManager(Event& event) const { _funcEventManager(event); }

private:
    GLFWwindow* _window;
    std::string _windowName;
    int _windowWidth;
    int _windowHeight;
    void (*_funcEventManager)(Event& event);
};

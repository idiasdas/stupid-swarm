#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "renderer/opengl-context.h"

class CustomImgui {
public:
    CustomImgui(GLFWwindow* window, OpenGLContext* context);
    void NewFrame();
    virtual void Update();
    void Draw();
    void Shutdown();

protected:
    bool _isOpen;
    OpenGLContext* _context;
};

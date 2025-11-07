#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "renderer/opengl-context.h"

class CustomImgui {
public:
    CustomImgui(GLFWwindow* window, OpenGLContext* context);
    void new_frame();
    virtual void update();
    void draw();
    void shutdown();

protected:
    bool _is_open;
    OpenGLContext* _context;
};

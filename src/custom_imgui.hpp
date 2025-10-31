#pragma once

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class CustomImgui {
public:
    void init(GLFWwindow* window);
    virtual void update();
    void draw();
    void shutdown();
};

#include "opengl-context.h"
#include "log.h"

OpenGLContext::OpenGLContext(const std::string& windowName, const int windowWidth, const int windowHeight, void (*funcEventManager)(Event& event))
{
    _windowName = windowName;
    _windowWidth = windowWidth;
    _windowHeight = windowHeight;
    _funcEventManager = funcEventManager;

    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW.");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_windowWidth, _windowHeight, _windowName.c_str(), NULL, NULL);

    if (_window == nullptr) {
        LOG_ERROR("Failed to open GLFW window.");
        LOG_ERROR(" - If you have an Intel GPU, they are not 4.6 compatible.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(_window);

    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_FALSE);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPos(_window, windowHeight / 2, windowWidth / 2);
    glfwSwapInterval(1); // Vsync (0 == off, 1 == on)
    glfwPollEvents();

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!status) {
        LOG_ERROR("Failed to initialize glad.");
        exit(EXIT_FAILURE);
    }

    LOG_INFO("OpenGL Info:");
    LOG_INFO(" - Vendor:    {0}", (const char*)glGetString(GL_VENDOR));
    LOG_INFO(" - Renderer:  {0}", (const char*)glGetString(GL_RENDERER));
    LOG_INFO(" - Version:   {0}", (const char*)glGetString(GL_VERSION));

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it is closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Do not draw polygons if the camera is inside them
    glEnable(GL_CULL_FACE);

    glfwSetWindowUserPointer(_window, this);
    SetEventsCallbacks();
}

void OpenGLContext::SetEventsCallbacks()
{
    glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        (void) scancode;
        (void) mods;
        OpenGLContext* context = (OpenGLContext *) glfwGetWindowUserPointer(window);
        if (action == GLFW_PRESS) {
            KeyPressEvent event(key);
            context->RunEventManager(event);
        }
        else if (action == GLFW_RELEASE) {
            KeyReleaseEvent event(key);
            context->RunEventManager(event);
        } });

    glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos) {
        OpenGLContext* context = (OpenGLContext *) glfwGetWindowUserPointer(window);
        MouseMoveEvent event(xpos, ypos);
        context->RunEventManager(event); });

    glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
        (void) mods;
        OpenGLContext* context = (OpenGLContext *) glfwGetWindowUserPointer(window);
        if (action == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            MouseButtonPressEvent event(button, xpos, ypos);
            context->RunEventManager(event);
        }
        else if (action == GLFW_RELEASE) {
            MouseButtonReleaseEvent event(button);
            context->RunEventManager(event);
        } });

    glfwSetScrollCallback(_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        (void) xoffset;
        OpenGLContext* context = (OpenGLContext *) glfwGetWindowUserPointer(window);
        MouseScrollEvent event(yoffset);
        context->RunEventManager(event); });

    glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
        OpenGLContext* context = (OpenGLContext *) glfwGetWindowUserPointer(window);
        context->SetWindowHeight(height);
        context->SetWindowWidth(width);

        glViewport(0, 0, width, height);
        WindowResizeEvent event(width, height);
        context->RunEventManager(event); });
}

#pragma once

#include <sstream>
#include <string>

#include "glm/glm.hpp"

enum class EventType {
    NONE = 0,
    WINDOW_CLOSE,
    WINDOW_RESIZE,
    KEY_PRESS,
    KEY_RELEASE,
    MOUSE_BUTTON_PRESS,
    MOUSE_BUTTON_RELEASE,
    MOUSE_MOVE,
    MOUSE_SCROLL,
    RAY
};

#define EVENT_CLASS_TYPE(type)                                   \
    static EventType GetStaticType() { return EventType::type; } \
    virtual EventType GetEventType() const override { return GetStaticType(); }

class Event {
public:
    virtual EventType GetEventType() const = 0;
    virtual std::string ToString() const = 0;
};

class WindowCloseEvent : public Event {
public:
    WindowCloseEvent() { }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowCloseEvent";
        return ss.str();
    }

    EVENT_CLASS_TYPE(WINDOW_CLOSE)
};

class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(int width, int heigth)
        : _width(width)
        , _height(heigth)
    {
    }

    inline int GetWidth() const { return _width; }
    inline int GetHeight() const { return _height; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent (" << _width << ", " << _height << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(WINDOW_RESIZE)

private:
    int _width, _height;
};

class KeyPressEvent : public Event {
public:
    KeyPressEvent(int key)
        : _key(key)
    {
    }

    inline int GetKey() const { return _key; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyPressEvent (" << _key << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(KEY_PRESS)

private:
    int _key;
};

class KeyReleaseEvent : public Event {
public:
    KeyReleaseEvent(int key)
        : _key(key)
    {
    }

    inline int GetKey() const { return _key; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyReleaseEvent (" << _key << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(KEY_RELEASE)

private:
    int _key;
};

class MouseButtonPressEvent : public Event {
public:
    MouseButtonPressEvent(int button, float xPos, float yPos)
        : _button(button)
        , _xPos(xPos)
        , _yPos(yPos)
    {
    }

    inline int GetButton() const { return _button; }
    inline double GetXPos() const { return _xPos; }
    inline double GetYPos() const { return _yPos; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonPressEvent (" << _button << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESS)

private:
    int _button;
    double _xPos, _yPos;
};

class MouseButtonReleaseEvent : public Event {
public:
    MouseButtonReleaseEvent(int button)
        : _button(button)
    {
    }

    inline int GetButton() const { return _button; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonReleaseEvent (" << _button << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASE)

private:
    int _button;
};

class MouseMoveEvent : public Event {
public:
    MouseMoveEvent(int xPos, int yPos)
        : _xPos(xPos)
        , _yPos(yPos)
    {
    }

    inline int GetX() const { return _xPos; }
    inline int GetY() const { return _yPos; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseMoveEvent (" << _xPos << ", " << _yPos << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(MOUSE_MOVE)

private:
    int _xPos, _yPos;
};

class MouseScrollEvent : public Event {
public:
    MouseScrollEvent(int yOffset)
        : _yOffset(yOffset)
    {
    }

    inline int GetYOffset() const { return _yOffset; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseScrollEvent (" << _yOffset << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(MOUSE_SCROLL)

private:
    int _yOffset;
};

class RayEvent : public Event {
public:
    RayEvent(glm::vec3 origin, glm::vec3 direction)
        : _origin(origin)
        , _direction(direction)
    {
    }

    glm::vec3 GetOrigin() { return _origin; }
    glm::vec3 GetDirection() { return _direction; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "RayEvent (" << _origin.x << ", " << _origin.y << ", " << _origin.z << ") (" << _direction.x << ", " << _direction.y << ", " << _direction.z << ")";
        return ss.str();
    }

    EVENT_CLASS_TYPE(RAY)

private:
    glm::vec3 _origin;
    glm::vec3 _direction;
};

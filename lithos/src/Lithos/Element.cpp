// lithos/src/Lithos/Element.cpp
#include "Lithos/Element.hpp"

namespace Lithos {
    void Element::SetPosition(const float x, const float y) {
        this->x = x;
        this->y = y;
    }

    void Element::SetSize(const float width, const float height) {
        this->width = width;
        this->height = height;
    }

    void Element::SetVisible(const bool visible) {
        this->visible = visible;
    }

    bool Element::HitTest(int px, int py) const {
        return visible &&
               px >= x && px <= x + width &&
               py >= y && py <= y + height;
    }

    void Element::OnResize(const std::function<void(Element*, int, int)>& callback) {
        resizeCallback = callback;
    }

    void Element::OnClickDown(const std::function<void(Element*, MouseButton)>& callback) {
        clickDownCallback = callback;
    }

    void Element::OnClickUp(const std::function<void(Element*, MouseButton)>& callback) {
        clickUpCallback = callback;
    }

    void Element::OnMouseMove(const std::function<void(Element*, int, int)>& callback) {
        mouseMoveCallback = callback;
    }

    void Element::OnKeyDown(const std::function<void(Element*, int)>& callback) {
        keyDownCallback = callback;
    }

    void Element::OnKeyUp(const std::function<void(Element*, int)>& callback) {
        keyUpCallback = callback;
    }

    bool Element::OnEvent(const Event& event) {
        // WindowResize
        if (event.type == EventType::WindowResize && resizeCallback) {
            resizeCallback(this, event.windowWidth, event.windowHeight);
            return false;
        }

        // MouseDown
        if (event.type == EventType::MouseDown && HitTest(event.mouseX, event.mouseY)) {
            if (clickDownCallback) {
                clickDownCallback(this, event.button);
                return true;
            }
        }

        // MouseUp
        if (event.type == EventType::MouseUp && HitTest(event.mouseX, event.mouseY)) {
            if (clickUpCallback) {
                clickUpCallback(this, event.button);
                return true;
            }
            return false;
        }

        // MouseMove
        if (event.type == EventType::MouseMove && mouseMoveCallback) {
            if (HitTest(event.mouseX, event.mouseY)) {
                mouseMoveCallback(this, event.mouseX, event.mouseY);
                return false;
            }
        }

        // KeyDown
        if (event.type == EventType::KeyDown && keyDownCallback) {
            keyDownCallback(this, event.key);
            return true;
        }

        // KeyUp
        if (event.type == EventType::KeyUp && keyUpCallback) {
            keyUpCallback(this, event.key);
            return true;
        }

        return false;
    }
}
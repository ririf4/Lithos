/*
    Copyright 2026 RiriFa

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 */

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

    void Element::SetVisible(const bool visible) { this->visible = visible; }

    bool Element::HitTest(int px, int py) const {
        return visible &&
            px >= x && px <= x + width &&
            py >= y && py <= y + height;
    }

    void Element::OnResize(const std::function<void(Element*, int, int)>& callback) { resizeCallback = callback; }

    void Element::OnClickDown(const std::function<void(Element*, MouseButton)>& callback) { clickDownCallback = callback; }

    void Element::OnClickUp(const std::function<void(Element*, MouseButton)>& callback) { clickUpCallback = callback; }

    void Element::OnMouseMove(const std::function<void(Element*, int, int)>& callback) { mouseMoveCallback = callback; }

    void Element::OnKeyDown(const std::function<void(Element*, int)>& callback) { keyDownCallback = callback; }

    void Element::OnKeyUp(const std::function<void(Element*, int)>& callback) { keyUpCallback = callback; }

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

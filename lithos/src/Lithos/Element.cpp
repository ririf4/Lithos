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
}
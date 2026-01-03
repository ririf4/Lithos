// Elements/RectElement.cpp
#include "Lithos/Elements/RectElement.hpp"

#include <iostream>

#include "include/core/SkCanvas.h"
#include "include/core/SkPaint.h"
#include "include/core/SkRRect.h"

namespace Lithos {
    RectElement::RectElement()
        : color(SK_ColorWHITE),
          cornerRadius(0.0f) {}

    RectElement::RectElement(const float x, const float y, const float width, const float height, const SkColor color)
        : color(color), cornerRadius(0.0f) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    void RectElement::Draw(SkCanvas* canvas) const {
        if (!visible) return;

        std::cout << "Drawing rect: " << width << "x" << height << std::endl;

        SkPaint paint;
        paint.setColor(color);
        paint.setAntiAlias(true);

        if (cornerRadius > 0) {
            SkRRect rrect;
            rrect.setRectXY(SkRect::MakeXYWH(x, y, width, height), cornerRadius, cornerRadius);
            canvas->drawRRect(rrect, paint);
        }
        else { canvas->drawRect(SkRect::MakeXYWH(x, y, width, height), paint); }
    }

    void RectElement::SetColor(const SkColor color) { this->color = color; }

    void RectElement::SetCornerRadius(const float radius) { this->cornerRadius = radius; }
}

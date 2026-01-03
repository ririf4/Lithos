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

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

#include "Lithos/Elements/CircleElement.hpp"
#include "include/core/SkCanvas.h"
#include "include/core/SkPaint.h"

namespace Lithos {
    CircleElement::CircleElement()
        : color(SK_ColorWHITE),
          radius(0.0f) {}

    CircleElement::CircleElement(const float centerX, const float centerY, const float radius, const SkColor color)
        : color(color), radius(radius) {
        this->x = centerX;
        this->y = centerY;
        this->width = radius * 2.0f;
        this->height = radius * 2.0f;
    }

    void CircleElement::Draw(SkCanvas* canvas) const {
        if (!visible) return;

        SkPaint paint;
        paint.setColor(color);
        paint.setAntiAlias(true);

        canvas->drawCircle(x, y, radius, paint);
    }

    void CircleElement::SetColor(const SkColor color) {
        this->color = color;
    }

    void CircleElement::SetRadius(const float radius) {
        this->radius = radius;
        this->width = radius * 2.0f;
        this->height = radius * 2.0f;
    }

    void CircleElement::SetCenter(const float centerX, const float centerY) {
        this->x = centerX;
        this->y = centerY;
    }
}
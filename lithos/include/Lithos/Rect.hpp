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

#pragma once

namespace Lithos {
    struct Rect {
        float x, y, width, height;

        Rect() : x(0), y(0), width(0), height(0) {}
        Rect(const float x, const float y, const float width, const float height)
            : x(x), y(y), width(width), height(height) {}

        float Left() const { return x; }
        float Top() const { return y; }
        float Right() const { return x + width; }
        float Bottom() const { return y + height; }

        bool Contains(const float px, const float py) const {
            return px >= x && px <= x + width &&
                   py >= y && py <= y + height;
        }
    };
}
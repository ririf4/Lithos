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
#include "Lithos/Core/Geometry.hpp"

namespace Lithos {
    // ========== RectGeometry ==========
    RectGeometry::RectGeometry(float x, float y, float w, float h)
        : x(x), y(y), width(w), height(h) {}

    bool RectGeometry::ContainsPointFast(float px, float py) const {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }

    bool RectGeometry::ContainsPoint(float px, float py) const {
        return ContainsPointFast(px, py); // Same for rectangles
    }

    void RectGeometry::GetBounds(float& left, float& top, float& right, float& bottom) const {
        left = x;
        top = y;
        right = x + width;
        bottom = y + height;
    }

    ID2D1Geometry* RectGeometry::GetD2DGeometry(ID2D1Factory* factory) const {
        if (cachedGeometry && cachedFactory == factory) {
            return cachedGeometry.Get();
        }

        cachedFactory = factory;
        cachedGeometry->Release();

        factory->CreateRectangleGeometry(
            D2D1::RectF(x, y, x + width, y + height),
            &cachedGeometry
        );

        return cachedGeometry.Get();
    }

    void RectGeometry::Update(float newX, float newY, float newWidth, float newHeight) {
        x = newX;
        y = newY;
        width = newWidth;
        height = newHeight;
        cachedGeometry->Release(); // Invalidate cache
    }

    float RectGeometry::Area() const {
        return width * height;
    }

    bool RectGeometry::Intersects(const Geometry& other) const {
        float ol, ot, or_, ob;
        other.GetBounds(ol, ot, or_, ob);

        return !(x + width < ol || or_ < x ||
                 y + height < ot || ob < y);
    }

    // ========== CircleGeometry ==========
    CircleGeometry::CircleGeometry(float cx, float cy, float r)
        : centerX(cx), centerY(cy), radius(r) {}

    bool CircleGeometry::ContainsPointFast(float px, float py) const {
        // Bounding box check
        return px >= centerX - radius && px <= centerX + radius &&
               py >= centerY - radius && py <= centerY + radius;
    }

    bool CircleGeometry::ContainsPoint(float px, float py) const {
        const float dx = px - centerX;
        const float dy = py - centerY;
        return (dx * dx + dy * dy) <= (radius * radius);
    }

    void CircleGeometry::GetBounds(float& left, float& top, float& right, float& bottom) const {
        left = centerX - radius;
        top = centerY - radius;
        right = centerX + radius;
        bottom = centerY + radius;
    }

    ID2D1Geometry* CircleGeometry::GetD2DGeometry(ID2D1Factory* factory) const {
        if (cachedGeometry && cachedFactory == factory) {
            return cachedGeometry.Get();
        }

        cachedFactory = factory;
        cachedGeometry->Release();

        factory->CreateEllipseGeometry(
            D2D1::Ellipse(D2D1::Point2F(centerX, centerY), radius, radius),
            &cachedGeometry
        );

        return cachedGeometry.Get();
    }

    void CircleGeometry::Update(float x, float y, float w, float h) {
        centerX = x + w / 2.0f;
        centerY = y + h / 2.0f;
        radius = std::min(w, h) / 2.0f;
        cachedGeometry->Release();
    }

    void CircleGeometry::SetRadius(float r) {
        radius = r;
        cachedGeometry->Release();
    }

    float CircleGeometry::Area() const {
        return 3.14159265359f * radius * radius;
    }

    bool CircleGeometry::Intersects(const Geometry& other) const {
        // Simplified: use bounding boxes
        float ol, ot, or_, ob;
        other.GetBounds(ol, ot, or_, ob);

        float left = centerX - radius;
        float top = centerY - radius;
        float right = centerX + radius;
        float bottom = centerY + radius;

        return !(right < ol || or_ < left || bottom < ot || ob < top);
    }
}
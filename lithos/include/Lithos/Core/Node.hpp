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
#include <d2d1_1.h>
#include <memory>
#include <vector>
#include "Event.hpp"
#include "Rect.hpp"
#include "Style.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class LITHOS_API Node {
    public:
        Node();
        virtual ~Node() = default;

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        Node(Node&&) = default;
        Node& operator=(Node&&) = default;

        void AddChild(std::unique_ptr<Node> child);
        Node* GetParent() { return parent; }
        const Node* GetParent() const { return parent; }
        const std::vector<std::unique_ptr<Node>>& GetChildren() const { return children; }

        Node& SetPosition(float x, float y);
        Node& SetSize(float width, float height);
        Node& SetWidth(float width);
        Node& SetHeight(float height);
        Node& SetPadding(float padding);
        Node& SetMargin(float margin);
        Node& SetBackgroundColor(Color color);
        Node& SetBorderColor(Color color);
        Node& SetBorderWidth(float width);
        Node& SetBorderRadius(float radius);
        Node& SetVisible(bool visible);
        Node& SetOpacity(float opacity);
        Node& SetShadow(float offsetX, float offsetY, float blur, Color color);
        Node& SetShadowEnabled(bool enabled);

        float GetX() const { return bounds.x; }
        float GetY() const { return bounds.y; }
        float GetWidth() const { return bounds.width; }
        float GetHeight() const { return bounds.height; }
        bool IsVisible() const { return visible; }
        const Rect& GetBounds() const { return bounds; }

        virtual void Layout();
        virtual void Draw(ID2D1DeviceContext* rt);

        virtual bool OnEvent(const Event& event);

        bool HitTest(float x, float y) const;

    protected:
        Node* parent;
        std::vector<std::unique_ptr<Node>> children;
        Style style;
        Rect bounds;
        bool visible;
        bool isDirty;
        bool isLayouting;

        void MarkDirty();
        void RequestLayout();
    };
}
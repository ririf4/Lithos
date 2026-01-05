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

#include "Lithos/Node.hpp"
#include <algorithm>
#include <d2d1.h>

namespace Lithos {
    Node::Node()
        : parent(nullptr),
          visible(true),
          isDirty(true) {}

    void Node::AddChild(std::unique_ptr<Node> child) {
        child->parent = this;
        children.push_back(std::move(child));
        RequestLayout();
    }

    Node& Node::SetPosition(const float x, const float y) {
        style.left = x;
        style.top = y;
        RequestLayout();
        return *this;
    }

    Node& Node::SetSize(const float width, const float height) {
        style.width = width;
        style.height = height;
        RequestLayout();
        return *this;
    }

    Node& Node::SetWidth(const float width) {
        style.width = width;
        RequestLayout();
        return *this;
    }

    Node& Node::SetHeight(const float height) {
        style.height = height;
        RequestLayout();
        return *this;
    }

    Node& Node::SetPadding(const float padding) {
        style.padding = padding;
        style.paddingTop = padding;
        style.paddingRight = padding;
        style.paddingBottom = padding;
        style.paddingLeft = padding;
        RequestLayout();
        return *this;
    }

    Node& Node::SetMargin(const float margin) {
        style.margin = margin;
        style.marginTop = margin;
        style.marginRight = margin;
        style.marginBottom = margin;
        style.marginLeft = margin;
        RequestLayout();
        return *this;
    }

    Node& Node::SetBackgroundColor(const Color color) {
        style.backgroundColor = color;
        MarkDirty();
        return *this;
    }

    Node& Node::SetBorderColor(const Color color) {
        style.borderColor = color;
        MarkDirty();
        return *this;
    }

    Node& Node::SetBorderWidth(const float width) {
        style.borderWidth = width;
        MarkDirty();
        return *this;
    }

    Node& Node::SetBorderRadius(const float radius) {
        style.borderRadius = radius;
        MarkDirty();
        return *this;
    }

    Node& Node::SetVisible(const bool visible) {
        this->visible = visible;
        MarkDirty();
        return *this;
    }

    Node& Node::SetOpacity(const float opacity) {
        style.opacity = std::clamp(opacity, 0.0f, 1.0f);
        MarkDirty();
        return *this;
    }

    void Node::Layout() {
        if (style.width > 0) {
            bounds.width = style.width;
        }
        if (style.height > 0) {
            bounds.height = style.height;
        }

        if (style.position == Position::Absolute) {
            bounds.x = style.left;
            bounds.y = style.top;
        }

        for (const auto& child : children) {
            child->Layout();
        }
    }

    void Node::Draw(ID2D1RenderTarget* rt) {
        if (!visible) return;

        if (style.backgroundColor.a > 0) {
            ID2D1SolidColorBrush* pBrush = nullptr;
            rt->CreateSolidColorBrush(
                D2D1::ColorF(
                    style.backgroundColor.r,
                    style.backgroundColor.g,
                    style.backgroundColor.b,
                    style.backgroundColor.a * style.opacity
                ),
                &pBrush
            );

            if (pBrush) {
                if (style.borderRadius > 0) {
                    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
                        D2D1::RectF(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height),
                        style.borderRadius,
                        style.borderRadius
                    );
                    rt->FillRoundedRectangle(roundedRect, pBrush);
                } else {
                    rt->FillRectangle(
                        D2D1::RectF(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height),
                        pBrush
                    );
                }
                pBrush->Release();
            }
        }

        if (style.borderWidth > 0 && style.borderColor.a > 0) {
            ID2D1SolidColorBrush* pBrush = nullptr;
            rt->CreateSolidColorBrush(
                D2D1::ColorF(
                    style.borderColor.r,
                    style.borderColor.g,
                    style.borderColor.b,
                    style.borderColor.a * style.opacity
                ),
                &pBrush
            );

            if (pBrush) {
                if (style.borderRadius > 0) {
                    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
                        D2D1::RectF(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height),
                        style.borderRadius,
                        style.borderRadius
                    );
                    rt->DrawRoundedRectangle(roundedRect, pBrush, style.borderWidth);
                } else {
                    rt->DrawRectangle(
                        D2D1::RectF(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height),
                        pBrush,
                        style.borderWidth
                    );
                }
                pBrush->Release();
            }
        }

        for (const auto& child : children) {
            child->Draw(rt);
        }

        isDirty = false;
    }

    bool Node::OnEvent(const Event& event) {
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            if ((*it)->OnEvent(event)) {
                return true;
            }
        }
        return false;
    }

    bool Node::HitTest(const float x, const float y) const {
        return visible && bounds.Contains(x, y);
    }

    void Node::MarkDirty() {
        isDirty = true;
        if (parent) {
            parent->MarkDirty();
        }
    }

    void Node::RequestLayout() {
        MarkDirty();
        // TODO: Layout
    }
}
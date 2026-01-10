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
#include <d2d1_1.h>
#include <iostream>

namespace Lithos {
    Node::Node()
        : parent(nullptr),
          visible(true),
          isDirty(true),
          isLayouting(false) {}

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

    Node& Node::SetShadow(const float offsetX, const float offsetY, const float blur, const Color color) {
        style.shadowEnabled = true;
        style.shadowOffsetX = offsetX;
        style.shadowOffsetY = offsetY;
        style.shadowBlur = blur;
        style.shadowColor = color;
        MarkDirty();
        return *this;
    }

    Node& Node::SetShadowEnabled(const bool enabled) {
        style.shadowEnabled = enabled;
        MarkDirty();
        return *this;
    }

    void Node::Layout() {
        if (isLayouting) return;
        isLayouting = true;

        if (style.width > 0) { bounds.width = style.width; }
        if (style.height > 0) { bounds.height = style.height; }

        if (style.position == Position::Absolute) {
            bounds.x = style.left;
            bounds.y = style.top;
        }
        else {
            if (parent) {
                bounds.x = parent->bounds.x + style.left;
                bounds.y = parent->bounds.y + style.top;
            }
            else {
                bounds.x = style.left;
                bounds.y = style.top;
            }
        }

        for (const auto& child : children) { child->Layout(); }

        isDirty = false;
        isLayouting = false;
    }

    void Node::Draw(ID2D1DeviceContext* dc) {
        if (!visible) return;

        if (style.shadowEnabled && style.shadowBlur > 0 && style.shadowColor.a > 0) {
            ID2D1Bitmap1* contentBitmap = nullptr;
            D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
                D2D1_BITMAP_OPTIONS_TARGET,
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
            );

            dc->CreateBitmap(
                D2D1::SizeU(
                    static_cast<UINT32>(bounds.width + style.shadowBlur * 4),
                    static_cast<UINT32>(bounds.height + style.shadowBlur * 4)
                ),
                nullptr,
                0,
                bitmapProps,
                &contentBitmap
            );

            if (contentBitmap) {
                ID2D1Image* oldTarget = nullptr;
                dc->GetTarget(&oldTarget);
                dc->SetTarget(contentBitmap);
                //dc->BeginDraw();
                dc->Clear(D2D1::ColorF(0, 0, 0, 0));

                const float tempX = style.shadowBlur * 2;
                const float tempY = style.shadowBlur * 2;

                ID2D1SolidColorBrush* pBrush = nullptr;
                dc->CreateSolidColorBrush(
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
                            D2D1::RectF(tempX, tempY, tempX + bounds.width, tempY + bounds.height),
                            style.borderRadius,
                            style.borderRadius
                        );
                        dc->FillRoundedRectangle(roundedRect, pBrush);
                    }
                    else {
                        dc->FillRectangle(
                            D2D1::RectF(tempX, tempY, tempX + bounds.width, tempY + bounds.height),
                            pBrush
                        );
                    }
                    pBrush->Release();
                }

                //dc->EndDraw();
                dc->SetTarget(oldTarget);

                ID2D1Effect* shadowEffect = nullptr;
                dc->CreateEffect(CLSID_D2D1Shadow, &shadowEffect);

                if (shadowEffect) {
                    shadowEffect->SetInput(0, contentBitmap);
                    shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, style.shadowBlur);
                    shadowEffect->SetValue(
                        D2D1_SHADOW_PROP_COLOR,
                        D2D1::Vector4F(
                            style.shadowColor.r,
                            style.shadowColor.g,
                            style.shadowColor.b,
                            style.shadowColor.a * style.opacity
                        )
                    );

                    ID2D1Image* outputImage = nullptr;
                    shadowEffect->GetOutput(&outputImage);

                    dc->DrawImage(
                        outputImage,
                        D2D1::Point2F(
                            bounds.x + style.shadowOffsetX - style.shadowBlur * 2,
                            bounds.y + style.shadowOffsetY - style.shadowBlur * 2
                        )
                    );

                    if (outputImage) outputImage->Release();
                    shadowEffect->Release();
                }

                if (oldTarget) oldTarget->Release();
                contentBitmap->Release();
            }
        }

        if (style.backgroundColor.a > 0) {
            ID2D1SolidColorBrush* pBrush = nullptr;
            dc->CreateSolidColorBrush(
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
                    dc->FillRoundedRectangle(roundedRect, pBrush);
                }
                else {
                    dc->FillRectangle(
                        D2D1::RectF(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height),
                        pBrush
                    );
                }
                pBrush->Release();
            }
        }

        if (style.borderWidth > 0 && style.borderColor.a > 0) {
            ID2D1SolidColorBrush* pBrush = nullptr;
            dc->CreateSolidColorBrush(
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
                    dc->DrawRoundedRectangle(roundedRect, pBrush, style.borderWidth);
                }
                else {
                    dc->DrawRectangle(
                        D2D1::RectF(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height),
                        pBrush,
                        style.borderWidth
                    );
                }
                pBrush->Release();
            }
        }

        for (const auto& child : children) { child->Draw(dc); }

        isDirty = false;
    }

    bool Node::OnEvent(const Event& event) {
        for (auto it = children.rbegin(); it != children.rend(); ++it) {if ((*it)->OnEvent(event)) { return true; } }
        return false;
    }

    bool Node::HitTest(const float x, const float y) const { return visible && bounds.Contains(x, y); }

    void Node::MarkDirty() {
        isDirty = true;
        if (parent) { parent->MarkDirty(); }
    }

    void Node::RequestLayout() {
        if (isLayouting) return;

        MarkDirty();

        auto root = this;
        while (root->parent) { root = root->parent; }
        root->Layout();
    }
}

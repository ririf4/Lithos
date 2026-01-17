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

#include "Lithos/Components/Button.hpp"
#include <iostream>
#include "Lithos/Components/TextNode.hpp"

namespace Lithos {
    Button::Button()
        : hoverCursor(CursorType::Hand),
          disabledCursor(CursorType::No),
          textNodePtr(nullptr),
          label(""),
          state(ButtonState::Normal),
          normalColor(Color(0.2f, 0.4f, 0.8f)),      // Blue
          hoverColor(Color(0.3f, 0.5f, 0.9f)),       // Light Blue
          pressedColor(Color(0.15f, 0.3f, 0.6f)),    // Dark Blue
          disabledColor(Color(0.7f, 0.7f, 0.7f)),    // Gray
          textColor(Colors::White),
          fontSize(16.0f),
          clickCallback(nullptr) {
        SetBackgroundColor(normalColor);
        SetBorderRadius(6);
    }

    Button::Button(const std::string& label)
        : Button() {
        SetLabel(label);
    }

    Button& Button::SetLabel(const std::string& label) {
        this->label = label;

        // If text node doesn't exist, create it
        if (!textNodePtr) {
            auto textNode = std::make_unique<TextNode>(label);

            TextNode* tempPtr = textNode.get();

            textNode->SetFontSize(fontSize)
                     .SetTextColor(textColor)
                     .SetTextAlign(TextAlign::Center);

            AddChild(std::move(textNode));

            textNodePtr = tempPtr;
        } else {
            // Update existing text node
            textNodePtr->SetText(label);
        }

        RequestLayout();
        return *this;
    }

    Button& Button::SetNormalColor(Color color) {
        normalColor = color;
        UpdateAppearance();
        return *this;
    }

    Button& Button::SetHoverColor(Color color) {
        hoverColor = color;
        UpdateAppearance();
        return *this;
    }

    Button& Button::SetPressedColor(Color color) {
        pressedColor = color;
        UpdateAppearance();
        return *this;
    }

    Button& Button::SetDisabledColor(Color color) {
        disabledColor = color;
        UpdateAppearance();
        return *this;
    }

    Button& Button::SetTextColor(Color color) {
        textColor = color;

        if (textNodePtr) {
            textNodePtr->SetTextColor(color);
        }

        return *this;
    }

    Button& Button::SetFontSize(float size) {
        fontSize = size;

        if (textNodePtr) {
            textNodePtr->SetFontSize(size);
        }

        return *this;
    }

    Button& Button::SetEnabled(bool enabled) {
        state = enabled ? ButtonState::Normal : ButtonState::Disabled;
        UpdateAppearance();
        return *this;
    }

    Button& Button::SetOnClick(const ClickCallback& callback) {
        clickCallback = callback;
        return *this;
    }

    Button& Button::SetHoverCursor(CursorType cursor) {
        hoverCursor = cursor;
        return *this;
    }

    Button& Button::SetDisabledCursor(CursorType cursor) {
        disabledCursor = cursor;
        return *this;
    }

    void Button::UpdateAppearance() {
        SetBackgroundColor(GetCurrentColor());
        MarkDirty();
    }

    Color Button::GetCurrentColor() const {
        switch (state) {
            case ButtonState::Hover:
                return hoverColor;
            case ButtonState::Pressed:
                return pressedColor;
            case ButtonState::Disabled:
                return disabledColor;
            case ButtonState::Normal:
            default:
                return normalColor;
        }
    }

    void Button::Draw(ID2D1DeviceContext* rt) {
        Container::Draw(rt);
    }

    bool Button::OnEvent(const Event& event) {
        bool isInside = HitTest(static_cast<float>(event.mouseX), static_cast<float>(event.mouseY));

        // Mouse move - update hover state and cursor
        if (event.type == EventType::MouseMove) {
            if (isInside) {
                if (state == ButtonState::Disabled) {
                    // Disabled state: show disabled cursor
                    HCURSOR cursor = LoadCursor(nullptr, GetWindowsCursor(disabledCursor));
                    ::SetCursor(cursor);
                    return false;
                } else if (state == ButtonState::Normal) {
                    // Normal → Hover: show hover cursor
                    state = ButtonState::Hover;
                    UpdateAppearance();

                    HCURSOR cursor = LoadCursor(nullptr, GetWindowsCursor(hoverCursor));
                    ::SetCursor(cursor);
                }
            } else if (state == ButtonState::Hover) {
                // Hover → Normal: reset cursor
                state = ButtonState::Normal;
                UpdateAppearance();

                HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW);
                ::SetCursor(cursor);
            }
        }

        // Disabled state - ignore all other events
        if (state == ButtonState::Disabled) {
            return false;
        }

        // Mouse down - pressed state
        if (event.type == EventType::MouseDown && event.button == MouseButton::Left) {
            if (isInside) {
                state = ButtonState::Pressed;
                UpdateAppearance();
                return true;
            }
        }

        // Mouse up - click detection
        if (event.type == EventType::MouseUp && event.button == MouseButton::Left) {
            if (state == ButtonState::Pressed) {
                if (isInside) {
                    // Click detected!
                    state = ButtonState::Hover;
                    UpdateAppearance();

                    if (clickCallback) {
                        clickCallback(this);
                    }
                } else {
                    state = ButtonState::Normal;
                    UpdateAppearance();
                }
                return true;
            }
        }

        // Forward to children (in case button contains interactive elements)
        return Container::OnEvent(event);
    }
}
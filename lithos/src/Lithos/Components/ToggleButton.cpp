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

#include "Lithos/Components/ToggleButton.hpp"
#include "Lithos/PCH.hpp"
#include "Lithos/Core/Color.hpp"

namespace Lithos {
    ToggleButton::ToggleButton()
        : checked(false),
          toggleStyle(ToggleStyle::Checkbox),
          state(ToggleState::Normal),
          checkedColor(Color(0.2f, 0.4f, 0.8f)),      // Blue
          uncheckedColor(Color(0.9f, 0.9f, 0.9f)),    // Light Gray
          checkmarkColor(White),
          switchOnColor(Color(0.2f, 0.8f, 0.4f)),     // Green
          switchOffColor(Color(0.7f, 0.7f, 0.7f)),    // Gray
          thumbColor(White),
          hoverCursor(CursorType::Hand),
          disabledCursor(CursorType::No),
          onChangeCallback(nullptr) {
        // Default size for checkbox
        SetSize(24, 24);
        SetBorderWidth(2);
        SetBorderColor(Color(0.6f, 0.6f, 0.6f));
        SetBorderRadius(4);
        UpdateAppearance();
    }

    ToggleButton::ToggleButton(ToggleStyle style)
        : ToggleButton() {
        toggleStyle = style;

        if (style == ToggleStyle::Switch) {
            // Default size for switch
            SetSize(50, 28);
            SetBorderRadius(14);
        }

        UpdateAppearance();
    }

    ToggleButton& ToggleButton::SetChecked(bool checked) {
        if (this->checked != checked) {
            this->checked = checked;
            UpdateAppearance();
        }
        return *this;
    }

    ToggleButton& ToggleButton::SetEnabled(bool enabled) {
        state = enabled ? ToggleState::Normal : ToggleState::Disabled;
        UpdateAppearance();
        return *this;
    }

    ToggleButton& ToggleButton::SetToggleStyle(ToggleStyle style) {
        if (toggleStyle != style) {
            toggleStyle = style;

            // Adjust default size based on style
            if (style == ToggleStyle::Checkbox) {
                SetSize(24, 24);
                SetBorderRadius(4);
            } else {
                SetSize(50, 28);
                SetBorderRadius(14);
            }

            UpdateAppearance();
        }
        return *this;
    }

    ToggleButton& ToggleButton::SetCheckedColor(Color color) {
        checkedColor = color;
        UpdateAppearance();
        return *this;
    }

    ToggleButton& ToggleButton::SetUncheckedColor(Color color) {
        uncheckedColor = color;
        UpdateAppearance();
        return *this;
    }

    ToggleButton& ToggleButton::SetCheckmarkColor(Color color) {
        checkmarkColor = color;
        UpdateAppearance();
        return *this;
    }

    ToggleButton& ToggleButton::SetSwitchOnColor(Color color) {
        switchOnColor = color;
        UpdateAppearance();
        return *this;
    }

    ToggleButton& ToggleButton::SetSwitchOffColor(Color color) {
        switchOffColor = color;
        UpdateAppearance();
        return *this;
    }

    ToggleButton& ToggleButton::SetThumbColor(Color color) {
        thumbColor = color;
        UpdateAppearance();
        return *this;
    }

    ToggleButton& ToggleButton::SetOnChange(const OnChangeCallback& callback) {
        onChangeCallback = callback;
        return *this;
    }

    ToggleButton& ToggleButton::SetHoverCursor(CursorType cursor) {
        hoverCursor = cursor;
        return *this;
    }

    ToggleButton& ToggleButton::SetDisabledCursor(CursorType cursor) {
        disabledCursor = cursor;
        return *this;
    }

    void ToggleButton::UpdateAppearance() {
        if (toggleStyle == ToggleStyle::Checkbox) {
            // Checkbox: change background based on checked state
            SetBackgroundColor(checked ? checkedColor : uncheckedColor);
        } else {
            // Switch: change background based on checked state
            SetBackgroundColor(checked ? switchOnColor : switchOffColor);
        }

        MarkDirty();
    }

    void ToggleButton::Draw(ID2D1DeviceContext* rt) {
        // Draw background/border (from Container)
        Container::Draw(rt);

        if (toggleStyle == ToggleStyle::Checkbox) {
            DrawCheckbox(rt);
        } else {
            DrawSwitch(rt);
        }
    }

    void ToggleButton::DrawCheckbox(ID2D1DeviceContext* rt) {
        if (!checked) return;

        // Draw checkmark
        float centerX = bounds.x + bounds.width / 2.0f;
        float centerY = bounds.y + bounds.height / 2.0f;
        float size = bounds.width * 0.6f;

        DrawCheckmark(rt, centerX, centerY, size);
    }

    void ToggleButton::DrawSwitch(ID2D1DeviceContext* rt) {
        // Calculate thumb properties
        float thumbRadius = (bounds.height - 8) / 2.0f;
        float thumbY = bounds.y + bounds.height / 2.0f;

        // Thumb position: left when unchecked, right when checked
        float thumbX;
        if (checked) {
            thumbX = bounds.x + bounds.width - thumbRadius - 4;
        } else {
            thumbX = bounds.x + thumbRadius + 4;
        }

        // Draw thumb (circle)
        ID2D1SolidColorBrush* thumbBrush = nullptr;
        rt->CreateSolidColorBrush(
            D2D1::ColorF(
                thumbColor.r,
                thumbColor.g,
                thumbColor.b,
                thumbColor.a * style.opacity
            ),
            &thumbBrush
        );

        if (thumbBrush) {
            D2D1_ELLIPSE ellipse = D2D1::Ellipse(
                D2D1::Point2F(thumbX, thumbY),
                thumbRadius,
                thumbRadius
            );
            rt->FillEllipse(ellipse, thumbBrush);

            // Add subtle shadow to thumb
            ID2D1SolidColorBrush* shadowBrush = nullptr;
            rt->CreateSolidColorBrush(
                D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.2f),
                &shadowBrush
            );

            if (shadowBrush) {
                D2D1_ELLIPSE shadowEllipse = D2D1::Ellipse(
                    D2D1::Point2F(thumbX, thumbY + 1),
                    thumbRadius,
                    thumbRadius
                );
                rt->DrawEllipse(shadowEllipse, shadowBrush, 1.0f);
                shadowBrush->Release();
            }

            thumbBrush->Release();
        }
    }

    void ToggleButton::DrawCheckmark(ID2D1DeviceContext* rt, float centerX, float centerY, float size) {
        // Create checkmark path (✓)
        ID2D1Factory* factory = nullptr;
        rt->GetFactory(&factory);

        if (!factory) return;

        ID2D1PathGeometry* pathGeometry = nullptr;
        factory->CreatePathGeometry(&pathGeometry);

        if (pathGeometry) {
            ID2D1GeometrySink* sink = nullptr;
            pathGeometry->Open(&sink);

            if (sink) {
                // Checkmark coordinates (relative to center)
                float scale = size / 20.0f;

                // Start point (bottom of left stroke)
                D2D1_POINT_2F start = D2D1::Point2F(
                    centerX - 6 * scale,
                    centerY + 1 * scale
                );

                // Middle point (bottom of checkmark)
                D2D1_POINT_2F middle = D2D1::Point2F(
                    centerX - 1 * scale,
                    centerY + 6 * scale
                );

                // End point (top of right stroke)
                D2D1_POINT_2F end = D2D1::Point2F(
                    centerX + 7 * scale,
                    centerY - 5 * scale
                );

                sink->BeginFigure(start, D2D1_FIGURE_BEGIN_HOLLOW);
                sink->AddLine(middle);
                sink->AddLine(end);
                sink->EndFigure(D2D1_FIGURE_END_OPEN);
                sink->Close();
                sink->Release();
            }

            // Draw the checkmark
            ID2D1SolidColorBrush* brush = nullptr;
            rt->CreateSolidColorBrush(
                D2D1::ColorF(
                    checkmarkColor.r,
                    checkmarkColor.g,
                    checkmarkColor.b,
                    checkmarkColor.a * style.opacity
                ),
                &brush
            );

            if (brush) {
                rt->DrawGeometry(pathGeometry, brush, 3.0f);
                brush->Release();
            }

            pathGeometry->Release();
        }

        factory->Release();
    }

    bool ToggleButton::OnEvent(const Event& event) {
        bool isInside = HitTest(static_cast<float>(event.mouseX), static_cast<float>(event.mouseY));

        // Mouse move - update hover state and cursor
        if (event.type == EventType::MouseMove) {
            if (isInside) {
                if (state == ToggleState::Disabled) {
                    HCURSOR cursor = LoadCursor(nullptr, GetWindowsCursor(disabledCursor));
                    ::SetCursor(cursor);
                    return false;
                } else if (state == ToggleState::Normal) {
                    state = ToggleState::Hover;
                    HCURSOR cursor = LoadCursor(nullptr, GetWindowsCursor(hoverCursor));
                    ::SetCursor(cursor);
                }
            } else if (state == ToggleState::Hover) {
                state = ToggleState::Normal;
                HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW);
                ::SetCursor(cursor);
            }
        }

        // Disabled state - ignore all other events
        if (state == ToggleState::Disabled) {
            return false;
        }

        // Mouse down
        if (event.type == EventType::MouseDown && event.button == MouseButton::Left) {
            if (isInside) {
                state = ToggleState::Pressed;
                return true;
            }
        }

        // Mouse up - toggle!
        if (event.type == EventType::MouseUp && event.button == MouseButton::Left) {
            if (state == ToggleState::Pressed && isInside) {
                // Toggle the state
                checked = !checked;
                state = ToggleState::Hover;

                // Update appearance
                UpdateAppearance();

                // Call callback
                if (onChangeCallback) {
                    onChangeCallback(this, checked);
                }

                return true;
            } else if (state == ToggleState::Pressed) {
                state = ToggleState::Normal;
            }
        }

        return Container::OnEvent(event);
    }
}
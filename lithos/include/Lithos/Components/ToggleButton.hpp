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
#include <functional>
#include "Lithos/Core/Event.hpp"
#include "Lithos/Layout/Container.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    enum class ToggleStyle {
        Checkbox,
        Switch
    };

    enum class ToggleState {
        Normal,
        Hover,
        Pressed,
        Disabled
    };

    class LITHOS_API ToggleButton : public Container {
        public:
            ToggleButton();
            explicit ToggleButton(ToggleStyle style);
            ~ToggleButton() override = default;

            ToggleButton(const ToggleButton&) = delete;
            ToggleButton& operator=(const ToggleButton&) = delete;
            ToggleButton(ToggleButton&&) = delete;
            ToggleButton& operator=(ToggleButton&&) = delete;

            // State
            ToggleButton& SetChecked(bool checked);
            bool IsChecked() const { return checked; }
            
            ToggleButton& SetEnabled(bool enabled);
            bool IsEnabled() const { return state != ToggleState::Disabled; }

            // Style
            ToggleButton& SetToggleStyle(ToggleStyle style);
            ToggleStyle GetToggleStyle() const { return toggleStyle; }

            // Checkbox style colors
            ToggleButton& SetCheckedColor(Color color);
            ToggleButton& SetUncheckedColor(Color color);
            ToggleButton& SetCheckmarkColor(Color color);

            // Switch style colors
            ToggleButton& SetSwitchOnColor(Color color);
            ToggleButton& SetSwitchOffColor(Color color);
            ToggleButton& SetThumbColor(Color color);

            // Callback
            using OnChangeCallback = std::function<void(ToggleButton*, bool)>;
            ToggleButton& SetOnChange(const OnChangeCallback& callback);

            // Cursor
            ToggleButton& SetHoverCursor(CursorType cursor);
            ToggleButton& SetDisabledCursor(CursorType cursor);

            void Draw(ID2D1DeviceContext* rt) override;
            bool OnEvent(const Event& event) override;

        private:
            bool checked;
            ToggleStyle toggleStyle;
            ToggleState state;

            // Checkbox style
            Color checkedColor;
            Color uncheckedColor;
            Color checkmarkColor;

            // Switch style
            Color switchOnColor;
            Color switchOffColor;
            Color thumbColor;

            CursorType hoverCursor;
            CursorType disabledCursor;

            OnChangeCallback onChangeCallback;

            void UpdateAppearance();
            void DrawCheckbox(ID2D1DeviceContext* rt);
            void DrawSwitch(ID2D1DeviceContext* rt);
            void DrawCheckmark(ID2D1DeviceContext* rt, float x, float y, float size);
    };
}
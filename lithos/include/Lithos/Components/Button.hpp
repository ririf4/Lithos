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
#include <string>
#include "Lithos/Core/Event.hpp"
#include "Lithos/Core/Color.hpp"
#include "Lithos/Layout/Container.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class TextNode;

    enum class ButtonState {
        Normal,
        Hover,
        Pressed,
        Disabled
    };

    class LITHOS_API Button : public Container {
        public:
            Button();
            explicit Button(const std::string& label);
            ~Button() override = default;

            Button(const Button&) = delete;
            Button& operator=(const Button&) = delete;
            Button(Button&&) = delete;
            Button& operator=(Button&&) = delete;

            // Label
            Button& SetLabel(const std::string& label);
            const std::string& GetLabel() const { return label; }

            // State styling
            Button& SetNormalColor(Color color);
            Button& SetHoverColor(Color color);
            Button& SetPressedColor(Color color);
            Button& SetDisabledColor(Color color);

            Button& SetTextColor(Color color);
            Button& SetFontSize(float size);

            // State management
            Button& SetEnabled(bool enabled);
            bool IsEnabled() const { return state != ButtonState::Disabled; }
            ButtonState GetState() const { return state; }

            // Callback
            using ClickCallback = std::function<void(Button*)>;
            Button& SetOnClick(const ClickCallback& callback);

            // Cursor
            Button& SetHoverCursor(CursorType cursor);
            Button& SetDisabledCursor(CursorType cursor);

            CursorType GetHoverCursor() const { return hoverCursor; }
            CursorType GetDisabledCursor() const { return disabledCursor; }

            void Draw(ID2D1DeviceContext* rt) override;
            bool OnEvent(const Event& event) override;

        private:
            CursorType hoverCursor;
            CursorType disabledCursor;

            TextNode* textNodePtr;

            std::string label;
            ButtonState state;

            Color normalColor;
            Color hoverColor;
            Color pressedColor;
            Color disabledColor;

            Color textColor;
            float fontSize;

            ClickCallback clickCallback;

            void UpdateAppearance();
            Color GetCurrentColor() const;
    };
}
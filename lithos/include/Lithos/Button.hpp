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
#include "Container.hpp"
#include "Event.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class TextNode;

    /**
     * @brief Button states for visual feedback
     */
    enum class ButtonState {
        Normal,    ///< Default state
        Hover,     ///< Mouse hovering over button
        Pressed,   ///< Mouse button pressed down
        Disabled   ///< Button is disabled (non-interactive)
    };

    /**
     * @brief Interactive button component with state management
     *
     * Button provides a complete interactive button implementation with:
     * - Four visual states (Normal, Hover, Pressed, Disabled)
     * - State-specific color customization
     * - Click event handling with callbacks
     * - Integrated text label (TextNode)
     * - Custom cursor support
     * - Automatic state transitions based on mouse events
     *
     * Button extends Container, so it can contain additional child elements
     * and use flexbox layout for positioning the label.
     *
     * @note The button contains an internal TextNode for the label.
     */
    class LITHOS_API Button : public Container {
        public:
            Button();

            /**
             * @brief Constructs a button with a text label
             * @param label Initial button label text
             */
            explicit Button(const std::string& label);

            ~Button() override = default;

            Button(const Button&) = delete;
            Button& operator=(const Button&) = delete;
            Button(Button&&) = delete;
            Button& operator=(Button&&) = delete;

            // ========== Label Configuration ==========

            /** @brief Sets the button label text */
            Button& SetLabel(const std::string& label);
            /** @brief Gets the current button label */
            const std::string& GetLabel() const { return label; }

            // ========== State-Specific Colors ==========

            /** @brief Sets color for Normal state */
            Button& SetNormalColor(Color color);
            /** @brief Sets color for Hover state */
            Button& SetHoverColor(Color color);
            /** @brief Sets color for Pressed state */
            Button& SetPressedColor(Color color);
            /** @brief Sets color for Disabled state */
            Button& SetDisabledColor(Color color);

            /** @brief Sets text color for the label */
            Button& SetTextColor(Color color);
            /** @brief Sets font size for the label */
            Button& SetFontSize(float size);

            // ========== State Management ==========

            /** @brief Enables or disables the button */
            Button& SetEnabled(bool enabled);
            /** @brief Checks if the button is enabled */
            bool IsEnabled() const { return state != ButtonState::Disabled; }
            /** @brief Gets the current button state */
            ButtonState GetState() const { return state; }

            // ========== Event Handling ==========

            /** @brief Callback function type for click events */
            using ClickCallback = std::function<void(Button*)>;

            /** @brief Sets the click event callback */
            Button& SetOnClick(const ClickCallback& callback);

            // ========== Cursor Configuration ==========

            /** @brief Sets cursor type when hovering (default: Hand) */
            Button& SetHoverCursor(CursorType cursor);
            /** @brief Sets cursor type when disabled (default: No) */
            Button& SetDisabledCursor(CursorType cursor);

            /** @brief Gets the hover cursor type */
            CursorType GetHoverCursor() const { return hoverCursor; }
            /** @brief Gets the disabled cursor type */
            CursorType GetDisabledCursor() const { return disabledCursor; }

            /** @brief Renders the button with current state color */
            void Draw(ID2D1DeviceContext* rt) override;
            /** @brief Handles mouse events for state transitions and clicks */
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
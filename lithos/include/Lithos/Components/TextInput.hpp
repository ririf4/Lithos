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
#include <string>
#include <functional>
#include <chrono>
#include "Lithos/Core/Node.hpp"
#include "Lithos/Core/Event.hpp"
#include "Lithos/Core/Color.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

struct IDWriteTextFormat;
struct IDWriteTextLayout;

namespace Lithos {
    enum class TextInputState {
        Normal,
        Hover,
        Focused,
        Disabled
    };

    class LITHOS_API TextInput : public Node {
        public:
            TextInput();
            explicit TextInput(const std::string& placeholder);
            ~TextInput() override;

            TextInput(const TextInput&) = delete;
            TextInput& operator=(const TextInput&) = delete;
            TextInput(TextInput&&) = delete;
            TextInput& operator=(TextInput&&) = delete;

            // Text content
            TextInput& SetText(const std::string& text);
            const std::string& GetText() const { return text; }

            // Placeholder
            TextInput& SetPlaceholder(const std::string& placeholder);
            const std::string& GetPlaceholder() const { return placeholderText; }

            // Input mode
            TextInput& SetPasswordMode(bool enabled);
            bool IsPasswordMode() const { return isPasswordMode; }

            TextInput& SetMultiLine(bool enabled);
            bool IsMultiLine() const { return isMultiLine; }

            TextInput& SetMaxLength(int length);
            int GetMaxLength() const { return maxLength; }

            // State
            TextInput& SetEnabled(bool enabled);
            bool IsEnabled() const { return state != TextInputState::Disabled; }

            TextInput& SetFocused(bool focused);
            bool IsFocused() const { return state == TextInputState::Focused; }

            // Styling
            TextInput& SetTextColor(Color color);
            TextInput& SetPlaceholderColor(Color color);
            TextInput& SetSelectionColor(Color color);
            TextInput& SetFontSize(float size);
            TextInput& SetFontFamily(const std::string& family);

            // State colors
            TextInput& SetNormalBorderColor(Color color);
            TextInput& SetHoverBorderColor(Color color);
            TextInput& SetFocusedBorderColor(Color color);
            TextInput& SetDisabledBackgroundColor(Color color);

            // Cursor
            TextInput& SetHoverCursor(CursorType cursor);

            // Callback
            using OnChangeCallback = std::function<void(TextInput*, const std::string&)>;
            TextInput& SetOnChange(const OnChangeCallback& callback);

            void Layout() override;
            void Draw(ID2D1DeviceContext* rt) override;
            bool OnEvent(const Event& event) override;

        private:
            // Text data
            std::string text;
            std::string placeholderText;

            // Input mode
            bool isPasswordMode;
            bool isMultiLine;
            int maxLength;

            // State
            TextInputState state;

            // Cursor and selection
            size_t cursorPosition;
            size_t selectionStart;
            size_t selectionEnd;
            bool hasSelection;

            // Cursor blinking
            std::chrono::steady_clock::time_point lastBlinkTime;
            bool cursorVisible;
            static constexpr int CURSOR_BLINK_MS = 530;

            // Colors
            Color textColor;
            Color placeholderColor;
            Color selectionColor;
            Color normalBorderColor;
            Color hoverBorderColor;
            Color focusedBorderColor;
            Color disabledBackgroundColor;

            CursorType hoverCursor;

            // DirectWrite resources
            IDWriteTextFormat* textFormat;
            IDWriteTextLayout* textLayout;

            // Font properties
            std::string fontFamily;
            float fontSize;

            // Callback
            OnChangeCallback onChangeCallback;

            // Helper methods
            void CreateTextFormat();
            void CreateTextLayout();
            void UpdateTextLayout();
            void ReleaseResources();

            void UpdateAppearance();
            void UpdateCursorBlink();
            void ResetCursorBlink();

            // Text editing
            void InsertCharacter(wchar_t ch);
            void DeleteSelection();
            void DeleteCharacterBefore();
            void DeleteCharacterAfter();
            void MoveCursor(int delta, bool selecting);
            void MoveCursorToStart(bool selecting);
            void MoveCursorToEnd(bool selecting);
            void SelectAll();
            void Copy();
            void Paste();
            void Cut();

            // Rendering helpers
            std::string GetDisplayText() const;
            float GetCursorXPosition() const;
            void DrawSelection(ID2D1DeviceContext* rt);
            void DrawCursor(ID2D1DeviceContext* rt);
            void DrawText(ID2D1DeviceContext* rt);

            // Hit testing for cursor positioning
            size_t GetCharacterIndexFromPoint(float x, float y);

            // UTF-8 helpers
            size_t Utf8Length(const std::string& str) const;
            size_t Utf8ByteIndexFromCharIndex(const std::string& str, size_t charIndex) const;
            std::wstring Utf8ToWString(const std::string& utf8) const;
            std::string WStringToUtf8(const std::wstring& wstr) const;
    };
}

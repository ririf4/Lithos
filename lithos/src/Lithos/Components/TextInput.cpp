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

#define NOMINMAX

#include "Lithos/Components/TextInput.hpp"
#include "Lithos/PCH.hpp"
#include "Lithos/Core/Window.hpp"
#include <dwrite.h>
#include <iostream>

namespace Lithos {
    namespace {
        // Global DirectWrite factory
        IDWriteFactory* GetDWriteFactory() {
            static IDWriteFactory* g_dwriteFactory = nullptr;
            if (!g_dwriteFactory) {
                DWriteCreateFactory(
                    DWRITE_FACTORY_TYPE_SHARED,
                    __uuidof(IDWriteFactory),
                    reinterpret_cast<IUnknown**>(&g_dwriteFactory)
                );
            }
            return g_dwriteFactory;
        }
    }

    TextInput::TextInput()
        : text(""),
          placeholderText(""),
          isPasswordMode(false),
          isMultiLine(false),
          maxLength(0),
          state(TextInputState::Normal),
          cursorPosition(0),
          selectionStart(0),
          selectionEnd(0),
          hasSelection(false),
          cursorVisible(true),
          textColor(Color(0.0f, 0.0f, 0.0f)),
          placeholderColor(Color(0.6f, 0.6f, 0.6f)),
          selectionColor(Color(0.4f, 0.6f, 1.0f, 0.4f)),
          normalBorderColor(Color(0.7f, 0.7f, 0.7f)),
          hoverBorderColor(Color(0.5f, 0.5f, 0.5f)),
          focusedBorderColor(Color(0.2f, 0.4f, 0.8f)),
          disabledBackgroundColor(Color(0.95f, 0.95f, 0.95f)),
          hoverCursor(CursorType::IBeam),
          textFormat(nullptr),
          textLayout(nullptr),
          fontFamily("Segoe UI"),
          fontSize(14.0f),
          onChangeCallback(nullptr) {

        // Default styling
        SetBackgroundColor(White);
        SetBorderWidth(1);
        SetBorderColor(normalBorderColor);
        SetBorderRadius(4);
        style.padding = 8;

        lastBlinkTime = std::chrono::steady_clock::now();

        CreateTextFormat();
    }

    TextInput::TextInput(const std::string& placeholder)
        : TextInput() {
        SetPlaceholder(placeholder);
    }

    TextInput::~TextInput() {
        ReleaseResources();
    }

    // ========== Public API Methods ==========

    TextInput& TextInput::SetText(const std::string& text) {
        this->text = text;
        cursorPosition = Utf8Length(text);
        hasSelection = false;
        UpdateTextLayout();
        MarkDirty();
        return *this;
    }

    TextInput& TextInput::SetPlaceholder(const std::string& placeholder) {
        placeholderText = placeholder;
        MarkDirty();
        return *this;
    }

    TextInput& TextInput::SetPasswordMode(bool enabled) {
        isPasswordMode = enabled;
        UpdateTextLayout();
        MarkDirty();
        return *this;
    }

    TextInput& TextInput::SetMultiLine(bool enabled) {
        isMultiLine = enabled;
        UpdateTextLayout();
        MarkDirty();
        return *this;
    }

    TextInput& TextInput::SetMaxLength(int length) {
        maxLength = length;
        return *this;
    }

    TextInput& TextInput::SetEnabled(bool enabled) {
        state = enabled ? TextInputState::Normal : TextInputState::Disabled;
        UpdateAppearance();
        return *this;
    }

    TextInput& TextInput::SetFocused(bool focused) {
        if (focused && state != TextInputState::Disabled) {
            state = TextInputState::Focused;
            ResetCursorBlink();
        } else if (state == TextInputState::Focused) {
            state = TextInputState::Normal;
        }
        UpdateAppearance();
        return *this;
    }

    TextInput& TextInput::SetTextColor(Color color) {
        textColor = color;
        MarkDirty();
        return *this;
    }

    TextInput& TextInput::SetPlaceholderColor(Color color) {
        placeholderColor = color;
        MarkDirty();
        return *this;
    }

    TextInput& TextInput::SetSelectionColor(Color color) {
        selectionColor = color;
        MarkDirty();
        return *this;
    }

    TextInput& TextInput::SetFontSize(float size) {
        fontSize = size;
        CreateTextFormat();
        UpdateTextLayout();
        MarkDirty();
        return *this;
    }

    TextInput& TextInput::SetFontFamily(const std::string& family) {
        fontFamily = family;
        CreateTextFormat();
        UpdateTextLayout();
        MarkDirty();
        return *this;
    }

    TextInput& TextInput::SetNormalBorderColor(Color color) {
        normalBorderColor = color;
        UpdateAppearance();
        return *this;
    }

    TextInput& TextInput::SetHoverBorderColor(Color color) {
        hoverBorderColor = color;
        UpdateAppearance();
        return *this;
    }

    TextInput& TextInput::SetFocusedBorderColor(Color color) {
        focusedBorderColor = color;
        UpdateAppearance();
        return *this;
    }

    TextInput& TextInput::SetDisabledBackgroundColor(Color color) {
        disabledBackgroundColor = color;
        UpdateAppearance();
        return *this;
    }

    TextInput& TextInput::SetHoverCursor(CursorType cursor) {
        hoverCursor = cursor;
        return *this;
    }

    TextInput& TextInput::SetOnChange(const OnChangeCallback& callback) {
        onChangeCallback = callback;
        return *this;
    }

    // ========== Resource Management ==========

    void TextInput::CreateTextFormat() {
        if (textFormat) {
            textFormat->Release();
            textFormat = nullptr;
        }

        IDWriteFactory* factory = GetDWriteFactory();
        if (!factory) return;

        std::wstring wFontFamily = Utf8ToWString(fontFamily);

        HRESULT hr = factory->CreateTextFormat(
            wFontFamily.c_str(),
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            fontSize,
            L"en-us",
            &textFormat
        );

        if (SUCCEEDED(hr) && textFormat) {
            textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
            textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            textFormat->SetWordWrapping(isMultiLine ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);
        }
    }

    void TextInput::CreateTextLayout() {
        if (textLayout) {
            textLayout->Release();
            textLayout = nullptr;
        }

        IDWriteFactory* factory = GetDWriteFactory();
        if (!factory || !textFormat) return;

        std::wstring wText = Utf8ToWString(GetDisplayText());

        float maxWidth = std::max(0.0f, bounds.width - style.padding * 2);
        float maxHeight = std::max(0.0f, bounds.height - style.padding * 2);

        HRESULT hr = factory->CreateTextLayout(
            wText.c_str(),
            static_cast<UINT32>(wText.length()),
            textFormat,
            maxWidth,
            maxHeight,
            &textLayout
        );
    }

    void TextInput::UpdateTextLayout() {
        CreateTextLayout();
    }

    void TextInput::ReleaseResources() {
        if (textLayout) {
            textLayout->Release();
            textLayout = nullptr;
        }
        if (textFormat) {
            textFormat->Release();
            textFormat = nullptr;
        }
    }

    void TextInput::UpdateAppearance() {
        switch (state) {
            case TextInputState::Focused:
                SetBorderColor(focusedBorderColor);
                SetBackgroundColor(White);
                break;
            case TextInputState::Hover:
                SetBorderColor(hoverBorderColor);
                SetBackgroundColor(White);
                break;
            case TextInputState::Disabled:
                SetBorderColor(normalBorderColor);
                SetBackgroundColor(disabledBackgroundColor);
                break;
            case TextInputState::Normal:
            default:
                SetBorderColor(normalBorderColor);
                SetBackgroundColor(White);
                break;
        }
        MarkDirty();
    }

    void TextInput::UpdateCursorBlink() {
        if (state != TextInputState::Focused) return;

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastBlinkTime
        ).count();

        if (elapsed >= CURSOR_BLINK_MS) {
            cursorVisible = !cursorVisible;
            lastBlinkTime = now;
            MarkDirty();
        }
    }

    void TextInput::ResetCursorBlink() {
        cursorVisible = true;
        lastBlinkTime = std::chrono::steady_clock::now();
    }

    // ========== Text Editing Operations ==========

    void TextInput::InsertCharacter(wchar_t ch) {
        if (hasSelection) {
            DeleteSelection();
        }

        if (maxLength > 0 && Utf8Length(text) >= static_cast<size_t>(maxLength)) {
            return;
        }

        std::wstring wch(1, ch);
        std::string utf8Char = WStringToUtf8(wch);

        size_t byteIndex = Utf8ByteIndexFromCharIndex(text, cursorPosition);
        text.insert(byteIndex, utf8Char);

        cursorPosition++;
        ResetCursorBlink();
        UpdateTextLayout();
        MarkDirty();

        if (onChangeCallback) {
            onChangeCallback(this, text);
        }
    }

    void TextInput::DeleteSelection() {
        if (!hasSelection) return;

        size_t start = std::min(selectionStart, selectionEnd);
        size_t end = std::max(selectionStart, selectionEnd);

        size_t startByteIndex = Utf8ByteIndexFromCharIndex(text, start);
        size_t endByteIndex = Utf8ByteIndexFromCharIndex(text, end);

        text.erase(startByteIndex, endByteIndex - startByteIndex);

        cursorPosition = start;
        hasSelection = false;
        ResetCursorBlink();
        UpdateTextLayout();
        MarkDirty();

        if (onChangeCallback) {
            onChangeCallback(this, text);
        }
    }

    void TextInput::DeleteCharacterBefore() {
        if (cursorPosition == 0) return;

        size_t byteIndex = Utf8ByteIndexFromCharIndex(text, cursorPosition - 1);
        size_t nextByteIndex = Utf8ByteIndexFromCharIndex(text, cursorPosition);

        text.erase(byteIndex, nextByteIndex - byteIndex);
        cursorPosition--;

        ResetCursorBlink();
        UpdateTextLayout();
        MarkDirty();

        if (onChangeCallback) {
            onChangeCallback(this, text);
        }
    }

    void TextInput::DeleteCharacterAfter() {
        size_t textLen = Utf8Length(text);
        if (cursorPosition >= textLen) return;

        size_t byteIndex = Utf8ByteIndexFromCharIndex(text, cursorPosition);
        size_t nextByteIndex = Utf8ByteIndexFromCharIndex(text, cursorPosition + 1);

        text.erase(byteIndex, nextByteIndex - byteIndex);

        ResetCursorBlink();
        UpdateTextLayout();
        MarkDirty();

        if (onChangeCallback) {
            onChangeCallback(this, text);
        }
    }

    void TextInput::MoveCursor(int delta, bool selecting) {
        size_t textLen = Utf8Length(text);
        size_t newPosition = cursorPosition;

        if (delta < 0 && cursorPosition > 0) {
            newPosition = cursorPosition - 1;
        } else if (delta > 0 && cursorPosition < textLen) {
            newPosition = cursorPosition + 1;
        }

        if (selecting) {
            if (!hasSelection) {
                selectionStart = cursorPosition;
                hasSelection = true;
            }
            selectionEnd = newPosition;
        } else {
            hasSelection = false;
        }

        cursorPosition = newPosition;
        ResetCursorBlink();
        MarkDirty();
    }

    void TextInput::MoveCursorToStart(bool selecting) {
        size_t newPosition = 0;

        if (selecting) {
            if (!hasSelection) {
                selectionStart = cursorPosition;
                hasSelection = true;
            }
            selectionEnd = newPosition;
        } else {
            hasSelection = false;
        }

        cursorPosition = newPosition;
        ResetCursorBlink();
        MarkDirty();
    }

    void TextInput::MoveCursorToEnd(bool selecting) {
        size_t newPosition = Utf8Length(text);

        if (selecting) {
            if (!hasSelection) {
                selectionStart = cursorPosition;
                hasSelection = true;
            }
            selectionEnd = newPosition;
        } else {
            hasSelection = false;
        }

        cursorPosition = newPosition;
        ResetCursorBlink();
        MarkDirty();
    }

    void TextInput::SelectAll() {
        if (text.empty()) return;

        selectionStart = 0;
        selectionEnd = Utf8Length(text);
        hasSelection = true;
        cursorPosition = selectionEnd;
        MarkDirty();
    }

    void TextInput::Copy() {
        if (!hasSelection) return;

        size_t start = std::min(selectionStart, selectionEnd);
        size_t end = std::max(selectionStart, selectionEnd);

        size_t startByteIndex = Utf8ByteIndexFromCharIndex(text, start);
        size_t endByteIndex = Utf8ByteIndexFromCharIndex(text, end);

        std::string selectedText = text.substr(startByteIndex, endByteIndex - startByteIndex);
        std::wstring wideText = Utf8ToWString(selectedText);

        if (OpenClipboard(nullptr)) {
            EmptyClipboard();

            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (wideText.length() + 1) * sizeof(wchar_t));
            if (hMem) {
                wchar_t* pMem = static_cast<wchar_t*>(GlobalLock(hMem));
                memcpy(pMem, wideText.c_str(), (wideText.length() + 1) * sizeof(wchar_t));
                GlobalUnlock(hMem);
                SetClipboardData(CF_UNICODETEXT, hMem);
            }

            CloseClipboard();
        }
    }

    void TextInput::Paste() {
        if (!OpenClipboard(nullptr)) return;

        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData) {
            wchar_t* pData = static_cast<wchar_t*>(GlobalLock(hData));
            if (pData) {
                std::wstring wideText(pData);

                for (wchar_t ch : wideText) {
                    if (ch == L'\r') continue;
                    if (!isMultiLine && ch == L'\n') continue;
                    InsertCharacter(ch);
                }

                GlobalUnlock(hData);
            }
        }

        CloseClipboard();
    }

    void TextInput::Cut() {
        Copy();
        DeleteSelection();
    }

    // ========== Rendering ==========

    std::string TextInput::GetDisplayText() const {
        if (text.empty()) {
            return "";
        }

        if (!isPasswordMode) {
            return text;
        }

        // Replace each character with bullet
        size_t charCount = Utf8Length(text);
        std::string maskedText;
        maskedText.reserve(charCount * 3);

        for (size_t i = 0; i < charCount; i++) {
            maskedText += "\xe2\x97\x8f";  // UTF-8 for '●'
        }

        return maskedText;
    }

    float TextInput::GetCursorXPosition() const {
        if (!textLayout) return bounds.x + style.padding;

        DWRITE_HIT_TEST_METRICS metrics;
        float x, y;

        textLayout->HitTestTextPosition(
            static_cast<UINT32>(cursorPosition),
            false,
            &x,
            &y,
            &metrics
        );

        return bounds.x + style.padding + x;
    }

    void TextInput::DrawSelection(ID2D1DeviceContext* rt) {
        if (!hasSelection || !textLayout) return;

        size_t start = std::min(selectionStart, selectionEnd);
        size_t end = std::max(selectionStart, selectionEnd);

        if (start >= end) return;

        UINT32 actualHitTestCount = 0;
        textLayout->HitTestTextRange(
            static_cast<UINT32>(start),
            static_cast<UINT32>(end - start),
            bounds.x + style.padding,
            bounds.y + style.padding,
            nullptr,
            0,
            &actualHitTestCount
        );

        if (actualHitTestCount == 0) return;

        std::vector<DWRITE_HIT_TEST_METRICS> hitTestMetrics(actualHitTestCount);
        textLayout->HitTestTextRange(
            static_cast<UINT32>(start),
            static_cast<UINT32>(end - start),
            bounds.x + style.padding,
            bounds.y + style.padding,
            hitTestMetrics.data(),
            actualHitTestCount,
            &actualHitTestCount
        );

        ID2D1SolidColorBrush* selectionBrush = nullptr;
        rt->CreateSolidColorBrush(
            D2D1::ColorF(selectionColor.r, selectionColor.g, selectionColor.b, selectionColor.a),
            &selectionBrush
        );

        if (selectionBrush) {
            for (const auto& metrics : hitTestMetrics) {
                D2D1_RECT_F rect = D2D1::RectF(
                    metrics.left,
                    metrics.top,
                    metrics.left + metrics.width,
                    metrics.top + metrics.height
                );
                rt->FillRectangle(rect, selectionBrush);
            }
            selectionBrush->Release();
        }
    }

    void TextInput::DrawCursor(ID2D1DeviceContext* rt) {
        if (state != TextInputState::Focused || !cursorVisible || !textLayout) return;

        float cursorX = GetCursorXPosition();

        DWRITE_TEXT_METRICS metrics;
        textLayout->GetMetrics(&metrics);
        float lineHeight = metrics.height;

        ID2D1SolidColorBrush* cursorBrush = nullptr;
        rt->CreateSolidColorBrush(
            D2D1::ColorF(textColor.r, textColor.g, textColor.b),
            &cursorBrush
        );

        if (cursorBrush) {
            rt->DrawLine(
                D2D1::Point2F(cursorX, bounds.y + style.padding),
                D2D1::Point2F(cursorX, bounds.y + style.padding + lineHeight),
                cursorBrush,
                1.5f
            );
            cursorBrush->Release();
        }
    }

    void TextInput::DrawText(ID2D1DeviceContext* rt) {
        if (!textLayout) return;

        ID2D1SolidColorBrush* brush = nullptr;

        if (text.empty() && !placeholderText.empty()) {
            // Draw placeholder
            std::wstring wPlaceholder = Utf8ToWString(placeholderText);

            rt->CreateSolidColorBrush(
                D2D1::ColorF(placeholderColor.r, placeholderColor.g, placeholderColor.b, placeholderColor.a),
                &brush
            );

            if (brush && textFormat) {
                rt->DrawTextW(
                    wPlaceholder.c_str(),
                    static_cast<UINT32>(wPlaceholder.length()),
                    textFormat,
                    D2D1::RectF(
                        bounds.x + style.padding,
                        bounds.y + style.padding,
                        bounds.x + bounds.width - style.padding,
                        bounds.y + bounds.height - style.padding
                    ),
                    brush
                );
                brush->Release();
            }
        } else {
            // Draw actual text
            rt->CreateSolidColorBrush(
                D2D1::ColorF(textColor.r, textColor.g, textColor.b, textColor.a),
                &brush
            );

            if (brush) {
                rt->DrawTextLayout(
                    D2D1::Point2F(bounds.x + style.padding, bounds.y + style.padding),
                    textLayout,
                    brush
                );
                brush->Release();
            }
        }
    }

    // ========== Hit Testing ==========

    size_t TextInput::GetCharacterIndexFromPoint(float x, float y) {
        if (!textLayout) return 0;

        BOOL isTrailingHit = FALSE;
        BOOL isInside = FALSE;
        DWRITE_HIT_TEST_METRICS metrics;

        textLayout->HitTestPoint(
            x - bounds.x - style.padding,
            y - bounds.y - style.padding,
            &isTrailingHit,
            &isInside,
            &metrics
        );

        size_t index = metrics.textPosition;
        if (isTrailingHit) {
            index += metrics.length;
        }

        return std::min(index, Utf8Length(text));
    }

    // ========== Layout and Drawing ==========

    void TextInput::Layout() {
        UpdateTextLayout();
        Node::Layout();
    }

    void TextInput::Draw(ID2D1DeviceContext* rt) {
        if (!visible) return;

        // Update cursor blink
        UpdateCursorBlink();

        // Draw background, border, shadow
        Node::Draw(rt);

        // Draw selection, text, and cursor
        DrawSelection(rt);
        DrawText(rt);
        DrawCursor(rt);
    }

    // ========== Event Handling ==========

    bool TextInput::OnEvent(const Event& event) {
        bool isInside = HitTest(static_cast<float>(event.mouseX), static_cast<float>(event.mouseY));

        // Mouse move - update hover state and cursor
        if (event.type == EventType::MouseMove) {
            if (isInside) {
                if (state == TextInputState::Disabled) {
                    HCURSOR cursor = LoadCursor(nullptr, IDC_NO);
                    ::SetCursor(cursor);
                } else if (state == TextInputState::Normal) {
                    state = TextInputState::Hover;
                    UpdateAppearance();
                    HCURSOR cursor = LoadCursor(nullptr, GetWindowsCursor(hoverCursor));
                    ::SetCursor(cursor);
                } else {
                    HCURSOR cursor = LoadCursor(nullptr, GetWindowsCursor(hoverCursor));
                    ::SetCursor(cursor);
                }
            } else if (state == TextInputState::Hover) {
                state = TextInputState::Normal;
                UpdateAppearance();
            }
        }

        // Mouse down - focus and position cursor
        if (event.type == EventType::MouseDown && event.button == MouseButton::Left) {
            if (isInside && state != TextInputState::Disabled) {
                state = TextInputState::Focused;
                UpdateAppearance();
                ResetCursorBlink();

                // Position cursor at click location
                cursorPosition = GetCharacterIndexFromPoint(
                    static_cast<float>(event.mouseX),
                    static_cast<float>(event.mouseY)
                );
                hasSelection = false;

                // Note: Need to get Window instance to call SetFocusedNode
                // This requires passing Window* or using a global/static reference
                // For now, leave focus management to be handled externally

                return true;
            } else if (state == TextInputState::Focused) {
                // Clicked outside - lose focus
                state = TextInputState::Normal;
                UpdateAppearance();
                hasSelection = false;
                return false;
            }
        }

        // Keyboard events (only when focused)
        if (state != TextInputState::Focused) {
            return false;
        }

        if (event.type == EventType::Char) {
            wchar_t ch = event.character;

            // Filter control characters (except newline in multiline mode)
            if (ch < 32 && !(isMultiLine && ch == L'\n')) {
                return true;
            }

            InsertCharacter(ch);
            return true;
        }

        if (event.type == EventType::KeyDown) {
            bool ctrlPressed = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
            bool shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;

            switch (event.key) {
                case VK_LEFT:
                    MoveCursor(-1, shiftPressed);
                    return true;
                case VK_RIGHT:
                    MoveCursor(1, shiftPressed);
                    return true;
                case VK_HOME:
                    MoveCursorToStart(shiftPressed);
                    return true;
                case VK_END:
                    MoveCursorToEnd(shiftPressed);
                    return true;
                case VK_BACK:
                    if (hasSelection) {
                        DeleteSelection();
                    } else {
                        DeleteCharacterBefore();
                    }
                    return true;
                case VK_DELETE:
                    if (hasSelection) {
                        DeleteSelection();
                    } else {
                        DeleteCharacterAfter();
                    }
                    return true;
                case 'A':
                    if (ctrlPressed) {
                        SelectAll();
                        return true;
                    }
                    break;
                case 'C':
                    if (ctrlPressed) {
                        Copy();
                        return true;
                    }
                    break;
                case 'V':
                    if (ctrlPressed) {
                        Paste();
                        return true;
                    }
                    break;
                case 'X':
                    if (ctrlPressed) {
                        Cut();
                        return true;
                    }
                    break;
            }
        }

        return false;
    }

    // ========== UTF-8 Helper Functions ==========

    size_t TextInput::Utf8Length(const std::string& str) const {
        size_t len = 0;
        for (size_t i = 0; i < str.length(); ) {
            unsigned char c = str[i];
            if (c < 0x80) {
                i += 1;
            } else if ((c & 0xE0) == 0xC0) {
                i += 2;
            } else if ((c & 0xF0) == 0xE0) {
                i += 3;
            } else if ((c & 0xF8) == 0xF0) {
                i += 4;
            } else {
                i += 1;
            }
            len++;
        }
        return len;
    }

    size_t TextInput::Utf8ByteIndexFromCharIndex(const std::string& str, size_t charIndex) const {
        size_t byteIndex = 0;
        size_t currentChar = 0;

        while (byteIndex < str.length() && currentChar < charIndex) {
            unsigned char c = str[byteIndex];
            if (c < 0x80) {
                byteIndex += 1;
            } else if ((c & 0xE0) == 0xC0) {
                byteIndex += 2;
            } else if ((c & 0xF0) == 0xE0) {
                byteIndex += 3;
            } else if ((c & 0xF8) == 0xF0) {
                byteIndex += 4;
            } else {
                byteIndex += 1;
            }
            currentChar++;
        }

        return byteIndex;
    }

    std::wstring TextInput::Utf8ToWString(const std::string& utf8) const {
        if (utf8.empty()) return L"";

        int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
        std::wstring result(size, 0);
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &result[0], size);

        // Remove null terminator
        if (!result.empty() && result.back() == L'\0') {
            result.pop_back();
        }

        return result;
    }

    std::string TextInput::WStringToUtf8(const std::wstring& wstr) const {
        if (wstr.empty()) return "";

        int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string result(size, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size, nullptr, nullptr);

        // Remove null terminator
        if (!result.empty() && result.back() == '\0') {
            result.pop_back();
        }

        return result;
    }
}

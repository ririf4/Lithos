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
#include "../Core/Node.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    /**
     * @brief Text rendering node with DirectWrite integration
     *
     * TextNode provides high-quality text rendering using DirectWrite.
     * Features include:
     * - Rich typography (font family, size, weight, style)
     * - Text alignment (left, center, right, justify)
     * - Word wrapping and line limiting
     * - Automatic size calculation based on text content
     * - Full style inheritance from Node (background, borders, shadows)
     *
     * @note Text layout is cached and only recreated when properties change.
     */
    class LITHOS_API TextNode : public Node {
    public:
        TextNode();

        /**
         * @brief Constructs a TextNode with initial text content
         * @param text Initial text string (UTF-8 encoded)
         */
        explicit TextNode(const std::string& text);

        ~TextNode() override;

        TextNode(const TextNode&) = delete;
        TextNode& operator=(const TextNode&) = delete;
        TextNode(TextNode&&) = default;
        TextNode& operator=(TextNode&&) = default;

        // ========== Text Content ==========

        /**
         * @brief Sets the text content
         * @param text UTF-8 encoded text string
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetText(const std::string& text);

        /**
         * @brief Gets the current text content
         * @return Const reference to the text string
         */
        const std::string& GetText() const { return text; }

        // ========== Font Properties ==========

        /** @brief Sets font family name (e.g., "Arial", "Segoe UI") */
        TextNode& SetFontFamily(const std::string& family);
        /** @brief Sets font size in points */
        TextNode& SetFontSize(float size);
        /** @brief Sets font weight (Thin, Light, Normal, Bold, etc.) */
        TextNode& SetFontWeight(FontWeight weight);
        /** @brief Sets font style (Normal, Italic, Oblique) */
        TextNode& SetFontStyle(FontStyle fontStyle);

        // ========== Text Styling ==========

        /** @brief Sets text color */
        TextNode& SetTextColor(Color color);
        /** @brief Sets text alignment (Left, Center, Right, Justify) */
        TextNode& SetTextAlign(TextAlign align);

        // ========== Text Wrapping ==========

        /**
         * @brief Enables or disables word wrapping
         * @param wrap true to enable word wrapping, false for single line
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetWordWrap(bool wrap);

        /**
         * @brief Sets maximum number of lines to display
         * @param maxLines Maximum line count (0 = unlimited)
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetMaxLines(int maxLines);

        /** @brief Checks if word wrapping is enabled */
        bool GetWordWrap() const { return wordWrap; }
        /** @brief Gets the maximum line limit */
        int GetMaxLines() const { return maxLines; }

        // ========== Node Method Overrides (Type-Safe Chaining) ==========
        // Non-virtual function shadowing is intentional for performance and type safety

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4266)  // Suppress non-virtual function hiding warning
#endif

        /**
         * @brief Sets position - returns TextNode& for type-safe chaining
         * @param x X coordinate
         * @param y Y coordinate
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetPosition(float x, float y) {
            SetPositionInternal(x, y);
            return *this;
        }

        /**
         * @brief Sets right offset - returns TextNode& for type-safe chaining
         * @param right Right offset
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetRight(float right) {
            SetRightInternal(right);
            return *this;
        }

        /**
         * @brief Sets bottom offset - returns TextNode& for type-safe chaining
         * @param bottom Bottom offset
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetBottom(float bottom) {
            SetBottomInternal(bottom);
            return *this;
        }

        /**
         * @brief Sets size - returns TextNode& for type-safe chaining
         * @param width Width
         * @param height Height
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetSize(float width, float height) {
            SetSizeInternal(width, height);
            return *this;
        }

        /**
         * @brief Sets width - returns TextNode& for type-safe chaining
         * @param width Width
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetWidth(float width) {
            SetWidthInternal(width);
            return *this;
        }

        /**
         * @brief Sets height - returns TextNode& for type-safe chaining
         * @param height Height
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetHeight(float height) {
            SetHeightInternal(height);
            return *this;
        }

        /**
         * @brief Sets background color - returns TextNode& for type-safe chaining
         * @param color Background color
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetBackgroundColor(Color color) {
            SetBackgroundColorInternal(color);
            return *this;
        }

        /**
         * @brief Sets border color - returns TextNode& for type-safe chaining
         * @param color Border color
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetBorderColor(Color color) {
            SetBorderColorInternal(color);
            return *this;
        }

        /**
         * @brief Sets border width - returns TextNode& for type-safe chaining
         * @param width Border width
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetBorderWidth(float width) {
            SetBorderWidthInternal(width);
            return *this;
        }

        /**
         * @brief Sets border radius - returns TextNode& for type-safe chaining
         * @param radius Border radius
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetBorderRadius(float radius) {
            SetBorderRadiusInternal(radius);
            return *this;
        }

        /**
         * @brief Sets opacity - returns TextNode& for type-safe chaining
         * @param opacity Opacity (0.0-1.0)
         * @return Reference to this TextNode for method chaining
         */
        TextNode& SetOpacity(float opacity) {
            SetOpacityInternal(opacity);
            return *this;
        }

#ifdef _MSC_VER
#pragma warning(pop)
#endif

        /**
         * @brief Calculates text layout and updates bounds
         */
        void Layout() override;

        /**
         * @brief Renders the text with background, borders, and shadows
         * @param rt Direct2D device context
         */
        void Draw(ID2D1DeviceContext* rt) override;

    private:
        std::string text;

        // Text wrapping
        bool wordWrap;
        int maxLines;

        // DirectWrite resources
        IDWriteTextFormat* textFormat;
        IDWriteTextLayout* textLayout;

        // Performance optimization: cached text brush
        mutable ID2D1SolidColorBrush* cachedTextBrush;  ///< Cached text brush
        mutable Color cachedTextColor;                   ///< Cached text color

        void CreateTextFormat();
        void CreateTextLayout();
        void UpdateTextLayout();
        void ReleaseResources();

        // Cached dimensions
        float cachedTextWidth;
        float cachedTextHeight;
    };
}
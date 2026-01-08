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
#include "Lithos/Node.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class LITHOS_API TextNode : public Node {
    public:
        TextNode();
        explicit TextNode(const std::string& text);
        ~TextNode() override;

        TextNode(const TextNode&) = delete;
        TextNode& operator=(const TextNode&) = delete;
        TextNode(TextNode&&) = default;
        TextNode& operator=(TextNode&&) = default;

        // Text content
        TextNode& SetText(const std::string& text);
        const std::string& GetText() const { return text; }

        // Font properties (delegates to style)
        TextNode& SetFontFamily(const std::string& family);
        TextNode& SetFontSize(float size);
        TextNode& SetFontWeight(FontWeight weight);
        TextNode& SetFontStyle(FontStyle fontStyle);

        // Text styling (delegates to style)
        TextNode& SetTextColor(Color color);
        TextNode& SetTextAlign(TextAlign align);

        // Text wrapping
        TextNode& SetWordWrap(bool wrap);
        TextNode& SetMaxLines(int maxLines);

        bool GetWordWrap() const { return wordWrap; }
        int GetMaxLines() const { return maxLines; }

        void Layout() override;
        void Draw(ID2D1DeviceContext* rt) override;

    private:
        std::string text;

        // Text wrapping
        bool wordWrap;
        int maxLines;

        // DirectWrite resources
        IDWriteTextFormat* textFormat;
        IDWriteTextLayout* textLayout;

        void CreateTextFormat();
        void CreateTextLayout();
        void UpdateTextLayout();
        void ReleaseResources();

        // Cached dimensions
        float cachedTextWidth;
        float cachedTextHeight;
    };
}
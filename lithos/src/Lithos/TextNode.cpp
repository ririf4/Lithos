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

#include "Lithos/TextNode.hpp"
#include <iostream>

namespace Lithos {
    namespace {
        // Global DirectWrite factory (initialized once)
        IDWriteFactory* g_dwriteFactory = nullptr;

        IDWriteFactory* GetDWriteFactory() {
            if (!g_dwriteFactory) {
                DWriteCreateFactory(
                    DWRITE_FACTORY_TYPE_SHARED,
                    __uuidof(IDWriteFactory),
                    reinterpret_cast<IUnknown**>(&g_dwriteFactory)
                );
            }
            return g_dwriteFactory;
        }

        std::wstring ToWString(const std::string& utf8) {
            if (utf8.empty()) return L"";
            const int size_needed = MultiByteToWideChar(
                CP_UTF8, 0,
                utf8.data(), static_cast<int>(utf8.size()),
                nullptr, 0
            );
            std::wstring wstr(size_needed, 0);
            MultiByteToWideChar(
                CP_UTF8, 0,
                utf8.data(), static_cast<int>(utf8.size()),
                wstr.data(), size_needed
            );
            return wstr;
        }
    }

    TextNode::TextNode()
        : Node(),
          text(""),
          wordWrap(true),
          maxLines(0),
          textFormat(nullptr),
          textLayout(nullptr),
          cachedTextWidth(0),
          cachedTextHeight(0) {
    }

    TextNode::TextNode(const std::string& text)
        : Node(),
          text(text),
          wordWrap(true),
          maxLines(0),
          textFormat(nullptr),
          textLayout(nullptr),
          cachedTextWidth(0),
          cachedTextHeight(0) {
        CreateTextFormat();
        CreateTextLayout();
    }

    TextNode::~TextNode() {
        ReleaseResources();
    }

    TextNode& TextNode::SetText(const std::string& text) {
        if (this->text != text) {
            this->text = text;
            UpdateTextLayout();
            MarkDirty();
        }
        return *this;
    }

    TextNode& TextNode::SetFontFamily(const std::string& family) {
        if (style.fontFamily != family) {
            style.fontFamily = family;
            CreateTextFormat();
            UpdateTextLayout();
            MarkDirty();
        }
        return *this;
    }

    TextNode& TextNode::SetFontSize(float size) {
        if (style.fontSize != size) {
            style.fontSize = size;
            CreateTextFormat();
            UpdateTextLayout();
            MarkDirty();
        }
        return *this;
    }

    TextNode& TextNode::SetFontWeight(FontWeight weight) {
        if (style.fontWeight != weight) {
            style.fontWeight = weight;
            CreateTextFormat();
            UpdateTextLayout();
            MarkDirty();
        }
        return *this;
    }

    TextNode& TextNode::SetFontStyle(FontStyle fontStyle) {
        if (style.fontStyle != fontStyle) {
            style.fontStyle = fontStyle;
            CreateTextFormat();
            UpdateTextLayout();
            MarkDirty();
        }
        return *this;
    }

    TextNode& TextNode::SetTextColor(Color color) {
        style.textColor = color;
        MarkDirty();
        return *this;
    }

    TextNode& TextNode::SetTextAlign(TextAlign align) {
        if (style.textAlign != align) {
            style.textAlign = align;
            if (textFormat) {
                DWRITE_TEXT_ALIGNMENT dwriteAlign;
                switch (align) {
                    case TextAlign::Left:
                        dwriteAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
                        break;
                    case TextAlign::Center:
                        dwriteAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
                        break;
                    case TextAlign::Right:
                        dwriteAlign = DWRITE_TEXT_ALIGNMENT_TRAILING;
                        break;
                    default:
                        dwriteAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
                        break;
                }
                textFormat->SetTextAlignment(dwriteAlign);
                UpdateTextLayout();
            }
            MarkDirty();
        }
        return *this;
    }

    TextNode& TextNode::SetWordWrap(bool wrap) {
        if (wordWrap != wrap) {
            wordWrap = wrap;
            if (textFormat) {
                textFormat->SetWordWrapping(
                    wrap ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP
                );
                UpdateTextLayout();
            }
            MarkDirty();
        }
        return *this;
    }

    TextNode& TextNode::SetMaxLines(int maxLines) {
        if (this->maxLines != maxLines) {
            this->maxLines = maxLines;
            UpdateTextLayout();
            MarkDirty();
        }
        return *this;
    }

    void TextNode::CreateTextFormat() {
        if (textFormat) {
            textFormat->Release();
            textFormat = nullptr;
        }

        auto* factory = GetDWriteFactory();
        if (!factory) {
            std::cerr << "[Lithos Error] DirectWrite factory not available" << std::endl;
            return;
        }

        std::wstring wFontFamily = ToWString(style.fontFamily);

        HRESULT hr = factory->CreateTextFormat(
            wFontFamily.c_str(),
            nullptr,
            static_cast<DWRITE_FONT_WEIGHT>(style.fontWeight),
            static_cast<DWRITE_FONT_STYLE>(style.fontStyle),
            DWRITE_FONT_STRETCH_NORMAL,
            style.fontSize,
            L"en-us",
            &textFormat
        );

        if (FAILED(hr)) {
            std::cerr << "[Lithos Error] Failed to create text format" << std::endl;
            return;
        }

        // Set text alignment
        DWRITE_TEXT_ALIGNMENT dwriteAlign;
        switch (style.textAlign) {
            case TextAlign::Left:
                dwriteAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
                break;
            case TextAlign::Center:
                dwriteAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
                break;
            case TextAlign::Right:
                dwriteAlign = DWRITE_TEXT_ALIGNMENT_TRAILING;
                break;
            default:
                dwriteAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
                break;
        }
        textFormat->SetTextAlignment(dwriteAlign);

        // Set word wrapping
        textFormat->SetWordWrapping(
            wordWrap ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP
        );
    }

    void TextNode::CreateTextLayout() {
        if (!textFormat) {
            CreateTextFormat();
        }

        if (textLayout) {
            textLayout->Release();
            textLayout = nullptr;
        }

        auto* factory = GetDWriteFactory();
        if (!factory || !textFormat) {
            return;
        }

        std::wstring wText = ToWString(text);

        // Use current bounds width for layout, or large value if not set
        float layoutWidth = bounds.width > 0 ? bounds.width : 10000.0f;
        float layoutHeight = bounds.height > 0 ? bounds.height : 10000.0f;

        HRESULT hr = factory->CreateTextLayout(
            wText.c_str(),
            static_cast<UINT32>(wText.length()),
            textFormat,
            layoutWidth,
            layoutHeight,
            &textLayout
        );

        if (FAILED(hr)) {
            std::cerr << "[Lithos Error] Failed to create text layout" << std::endl;
            return;
        }

        // Set text alignment on layout (more reliable)
        DWRITE_TEXT_ALIGNMENT dwriteAlign;
        switch (style.textAlign) {
            case TextAlign::Left:
                dwriteAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
                break;
            case TextAlign::Center:
                dwriteAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
                break;
            case TextAlign::Right:
                dwriteAlign = DWRITE_TEXT_ALIGNMENT_TRAILING;
                break;
            default:
                dwriteAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
                break;
        }
        textLayout->SetTextAlignment(dwriteAlign);

        // Get text metrics
        DWRITE_TEXT_METRICS metrics;
        textLayout->GetMetrics(&metrics);

        cachedTextWidth = metrics.width;
        cachedTextHeight = metrics.height;

        // Apply max lines if specified
        if (maxLines > 0) {
            textLayout->SetMaxHeight(metrics.height / metrics.lineCount * maxLines);
        }
    }

    void TextNode::UpdateTextLayout() {
        CreateTextLayout();
    }

    void TextNode::ReleaseResources() {
        if (textLayout) {
            textLayout->Release();
            textLayout = nullptr;
        }
        if (textFormat) {
            textFormat->Release();
            textFormat = nullptr;
        }
    }

    void TextNode::Layout() {
        // Update bounds from style first
        if (style.width > 0) {
            bounds.width = style.width;
        }
        if (style.height > 0) {
            bounds.height = style.height;
        }

        if (!textFormat) {
            CreateTextFormat();
        }

        if (!textLayout || isDirty) {
            CreateTextLayout();
        }

        // If width/height not explicitly set, use text dimensions
        if (style.width == 0) {
            bounds.width = cachedTextWidth;
        }
        if (style.height == 0) {
            bounds.height = cachedTextHeight;
        }

        Node::Layout();
    }

    void TextNode::Draw(ID2D1DeviceContext* rt) {
        if (!visible || text.empty() || !textLayout) {
            return;
        }

        // Draw background/border (from base Node)
        Node::Draw(rt);

        // Create text brush
        ID2D1SolidColorBrush* brush = nullptr;
        rt->CreateSolidColorBrush(
            D2D1::ColorF(
                style.textColor.r,
                style.textColor.g,
                style.textColor.b,
                style.textColor.a * style.opacity
            ),
            &brush
        );

        if (brush) {
            D2D1_POINT_2F origin = D2D1::Point2F(bounds.x, bounds.y);
            rt->DrawTextLayout(origin, textLayout, brush);
            brush->Release();
        }

        // Draw children if any
        for (const auto& child : children) {
            if (child->IsVisible()) {
                child->Draw(rt);
            }
        }
    }
}
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

#include "Lithos/Components/TextNode.hpp"
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
                CP_UTF8,
                0,
                utf8.data(),
                static_cast<int>(utf8.size()),
                nullptr,
                0
            );
            std::wstring wstr(size_needed, 0);
            MultiByteToWideChar(
                CP_UTF8,
                0,
                utf8.data(),
                static_cast<int>(utf8.size()),
                wstr.data(),
                size_needed
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
          cachedTextBrush(nullptr),
          cachedTextColor(Transparent),
          cachedTextWidth(0),
          cachedTextHeight(0) {}

    TextNode::TextNode(const std::string& text)
        : Node(),
          text(text),
          wordWrap(true),
          maxLines(0),
          textFormat(nullptr),
          textLayout(nullptr),
          cachedTextBrush(nullptr),
          cachedTextColor(Transparent),
          cachedTextWidth(0),
          cachedTextHeight(0) {
        CreateTextFormat();
        CreateTextLayout();
    }

    TextNode::~TextNode() {
        ReleaseResources();
        // Release cached text brush
        if (cachedTextBrush) {
            cachedTextBrush->Release();
            cachedTextBrush = nullptr;
        }
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
                    wrap
                        ? DWRITE_WORD_WRAPPING_WRAP
                        : DWRITE_WORD_WRAPPING_NO_WRAP
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
            wordWrap
                ? DWRITE_WORD_WRAPPING_WRAP
                : DWRITE_WORD_WRAPPING_NO_WRAP
        );
    }

    void TextNode::CreateTextLayout() {
        if (!textFormat) { CreateTextFormat(); }

        if (textLayout) {
            textLayout->Release();
            textLayout = nullptr;
        }

        auto* factory = GetDWriteFactory();
        if (!factory || !textFormat) { return; }

        std::wstring wText = ToWString(text);

        // Use current bounds width for layout, or large value if not set
        float layoutWidth = bounds.width > 0
                                ? bounds.width
                                : 10000.0f;
        float layoutHeight = bounds.height > 0
                                 ? bounds.height
                                 : 10000.0f;

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
        if (maxLines > 0 && metrics.lineCount > 0) {
            textLayout->SetMaxHeight(metrics.height / metrics.lineCount * maxLines);
        }
    }

    void TextNode::UpdateTextLayout() { CreateTextLayout(); }

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
        if (style.width > 0) { bounds.width = style.width; }
        if (style.height > 0) { bounds.height = style.height; }

        if (!textFormat) { CreateTextFormat(); }

        if (!textLayout || isDirty) { CreateTextLayout(); }

        // If width/height not explicitly set, use text dimensions
        if (style.width == 0) { bounds.width = cachedTextWidth; }
        if (style.height == 0) { bounds.height = cachedTextHeight; }

        Node::Layout();
    }

    void TextNode::Draw(ID2D1DeviceContext* rt) {
        if (!visible || text.empty() || !textLayout) { return; }

        // Draw shadow (if enabled)
        if (style.shadowEnabled && style.shadowBlur > 0 && style.shadowColor.a > 0) {
            ID2D1Bitmap1* contentBitmap = nullptr;
            D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
                D2D1_BITMAP_OPTIONS_TARGET,
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
            );

            rt->CreateBitmap(
                D2D1::SizeU(
                    static_cast<UINT32>(bounds.width + style.shadowBlur * 4),
                    static_cast<UINT32>(bounds.height + style.shadowBlur * 4)
                ),
                nullptr,
                0,
                bitmapProps,
                &contentBitmap
            );

            if (contentBitmap) {
                ID2D1Image* oldTarget = nullptr;
                rt->GetTarget(&oldTarget);
                rt->SetTarget(contentBitmap);
                rt->Clear(D2D1::ColorF(0, 0, 0, 0));

                const float tempX = style.shadowBlur * 2;
                const float tempY = style.shadowBlur * 2;

                ID2D1SolidColorBrush* pBrush = nullptr;
                rt->CreateSolidColorBrush(
                    D2D1::ColorF(
                        style.backgroundColor.r,
                        style.backgroundColor.g,
                        style.backgroundColor.b,
                        style.backgroundColor.a * style.opacity
                    ),
                    &pBrush
                );

                if (pBrush) {
                    if (style.borderRadius > 0) {
                        D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
                            D2D1::RectF(tempX, tempY, tempX + bounds.width, tempY + bounds.height),
                            style.borderRadius,
                            style.borderRadius
                        );
                        rt->FillRoundedRectangle(roundedRect, pBrush);
                    }
                    else {
                        rt->FillRectangle(
                            D2D1::RectF(tempX, tempY, tempX + bounds.width, tempY + bounds.height),
                            pBrush
                        );
                    }
                    pBrush->Release();
                }

                rt->SetTarget(oldTarget);

                ID2D1Effect* shadowEffect = nullptr;
                rt->CreateEffect(CLSID_D2D1Shadow, &shadowEffect);

                if (shadowEffect) {
                    shadowEffect->SetInput(0, contentBitmap);
                    shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, style.shadowBlur);
                    shadowEffect->SetValue(
                        D2D1_SHADOW_PROP_COLOR,
                        D2D1::Vector4F(
                            style.shadowColor.r,
                            style.shadowColor.g,
                            style.shadowColor.b,
                            style.shadowColor.a * style.opacity
                        )
                    );

                    ID2D1Image* outputImage = nullptr;
                    shadowEffect->GetOutput(&outputImage);

                    rt->DrawImage(
                        outputImage,
                        D2D1::Point2F(
                            bounds.x + style.shadowOffsetX - style.shadowBlur * 2,
                            bounds.y + style.shadowOffsetY - style.shadowBlur * 2
                        )
                    );

                    if (outputImage) outputImage->Release();
                    shadowEffect->Release();
                }

                if (oldTarget) oldTarget->Release();
                contentBitmap->Release();
            }
        }

        // Draw background
        if (style.backgroundColor.a > 0) {
            ID2D1SolidColorBrush* pBrush = nullptr;
            rt->CreateSolidColorBrush(
                D2D1::ColorF(
                    style.backgroundColor.r,
                    style.backgroundColor.g,
                    style.backgroundColor.b,
                    style.backgroundColor.a * style.opacity
                ),
                &pBrush
            );

            if (pBrush) {
                if (style.borderRadius > 0) {
                    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
                        D2D1::RectF(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height),
                        style.borderRadius,
                        style.borderRadius
                    );
                    rt->FillRoundedRectangle(roundedRect, pBrush);
                }
                else {
                    rt->FillRectangle(
                        D2D1::RectF(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height),
                        pBrush
                    );
                }
                pBrush->Release();
            }
        }

        // Draw border
        if (style.borderWidth > 0 && style.borderColor.a > 0) {
            ID2D1SolidColorBrush* pBrush = nullptr;
            rt->CreateSolidColorBrush(
                D2D1::ColorF(
                    style.borderColor.r,
                    style.borderColor.g,
                    style.borderColor.b,
                    style.borderColor.a * style.opacity
                ),
                &pBrush
            );

            if (pBrush) {
                if (style.borderRadius > 0) {
                    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
                        D2D1::RectF(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height),
                        style.borderRadius,
                        style.borderRadius
                    );
                    rt->DrawRoundedRectangle(roundedRect, pBrush, style.borderWidth);
                }
                else {
                    rt->DrawRectangle(
                        D2D1::RectF(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height),
                        pBrush,
                        style.borderWidth
                    );
                }
                pBrush->Release();
            }
        }

        // Draw text with cached brush
        Color effectiveTextColor(
            style.textColor.r,
            style.textColor.g,
            style.textColor.b,
            style.textColor.a * style.opacity
        );

        ID2D1SolidColorBrush* brush = GetOrCreateBrush(
            rt,
            effectiveTextColor,
            cachedTextBrush,
            cachedTextColor
        );

        if (brush) {
            D2D1_POINT_2F origin = D2D1::Point2F(bounds.x, bounds.y);
            rt->DrawTextLayout(origin, textLayout, brush);
            // Note: Don't release cached brush
        }

        // Draw children (only once)
        for (const auto& child : children) { if (child->IsVisible()) { child->Draw(rt); } }
    }
}

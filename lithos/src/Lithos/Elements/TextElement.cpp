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

#include "Lithos/Elements/TextElement.hpp"
#include "include/core/SkCanvas.h"
#include "include/core/SkFontMetrics.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkPaint.h"

#ifdef _WIN32
    #include "include/ports/SkTypeface_win.h"
#elif defined(__linux__)
    #include "include/ports/SkFontMgr_fontconfig.h"
#endif

namespace Lithos {
    TextElement::TextElement()
        : text(""),
          color(SK_ColorBLACK),
          fontSize(16.0f),
          fontFamily(""),
          alignment(Alignment::Left) {
        UpdateFont();
    }

    TextElement::TextElement(const float x, const float y, const std::string& text, const SkColor color, const float fontSize)
        : text(text),
          color(color),
          fontSize(fontSize),
          fontFamily(""),
          alignment(Alignment::Left) {
        this->x = x;
        this->y = y;
        UpdateFont();

        SkRect bounds;
        font.measureText(text.c_str(), text.size(), SkTextEncoding::kUTF8, &bounds);
        this->width = bounds.width();

        SkFontMetrics metrics;
        font.getMetrics(&metrics);
        this->height = metrics.fDescent - metrics.fAscent;
    }

    void TextElement::Draw(SkCanvas* canvas) const {
        if (!visible || text.empty()) return;

        SkPaint paint;
        paint.setColor(color);
        paint.setAntiAlias(true);

        SkRect bounds;
        font.measureText(text.c_str(), text.size(), SkTextEncoding::kUTF8, &bounds);

        float drawX = x;

        switch (alignment) {
            case Alignment::Center:
                drawX = x - bounds.width() / 2.0f;
                break;
            case Alignment::Right:
                drawX = x - bounds.width();
                break;
            case Alignment::Left:
            default:
                break;
        }

        canvas->drawSimpleText(text.c_str(), text.size(), SkTextEncoding::kUTF8,
                               drawX, y, font, paint);
    }

    void TextElement::SetText(const std::string& text) {
        this->text = text;

        SkRect bounds;
        font.measureText(text.c_str(), text.size(), SkTextEncoding::kUTF8, &bounds);
        this->width = bounds.width();

        SkFontMetrics metrics;
        font.getMetrics(&metrics);
        this->height = metrics.fDescent - metrics.fAscent;
    }

    void TextElement::SetColor(const SkColor color) {
        this->color = color;
    }

    void TextElement::SetFontSize(const float size) {
        this->fontSize = size;
        UpdateFont();
    }

    void TextElement::SetFontFamily(const std::string& family) {
        this->fontFamily = family;
        UpdateFont();
    }

    void TextElement::SetAlignment(const Alignment align) {
        this->alignment = align;
    }

    void TextElement::UpdateFont() const {
        font.setSize(fontSize);

        if (!fontFamily.empty()) {
            sk_sp<SkFontMgr> fontMgr;

            #ifdef _WIN32
            fontMgr = SkFontMgr_New_DirectWrite();
            #elif defined(__linux__)
            fontMgr = SkFontMgr_New_FontConfig(nullptr);
            #endif

            const sk_sp<SkTypeface> typeface = fontMgr->matchFamilyStyle(
                fontFamily.c_str(),
                SkFontStyle::Normal()
            );

            if (typeface) {
                font.setTypeface(typeface);
            }
        }
    }
}
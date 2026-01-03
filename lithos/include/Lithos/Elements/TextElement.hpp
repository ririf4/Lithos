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
#include <include/core/SkColor.h>
#include <include/core/SkFont.h>
#include "Lithos/Element.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class LITHOS_API TextElement : public Element {
        public:
            TextElement();
            TextElement(float x, float y, const std::string& text, SkColor color, float fontSize = 16.0f);
            virtual ~TextElement() = default;

            void Draw(SkCanvas* canvas) const override;

            void SetText(const std::string& text);
            const std::string& GetText() const { return text; }

            void SetColor(SkColor color);
            SkColor GetColor() const { return color; }

            void SetFontSize(float size);
            float GetFontSize() const { return fontSize; }

            void SetFontFamily(const std::string& family);
            const std::string& GetFontFamily() const { return fontFamily; }

            enum class Alignment {
                Left,
                Center,
                Right
            };

            void SetAlignment(Alignment align);
            Alignment GetAlignment() const { return alignment; }

        private:
            std::string text;
            SkColor color;
            float fontSize;
            std::string fontFamily;
            Alignment alignment;

            mutable SkFont font;
            void UpdateFont() const;
    };
}
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

// Elements/RectElement.hpp
#pragma once
#include <include/core/SkColor.h>
#include "Lithos/Element.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class LITHOS_API RectElement : public Element {
        public:
            RectElement();
            RectElement(float x, float y, float width, float height, SkColor color);
            virtual ~RectElement() = default;

            void Draw(SkCanvas* canvas) const override;

            void SetColor(SkColor color);
            SkColor GetColor() const { return color; }

            void SetCornerRadius(float radius);
            float GetCornerRadius() const { return cornerRadius; }

        private:
            SkColor color;
            float cornerRadius;
    };
}

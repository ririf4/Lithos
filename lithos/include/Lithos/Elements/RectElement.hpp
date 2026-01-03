// Elements/RectElement.hpp
#pragma once
#include <include/core/SkColor.h>
#include "Lithos/Element.hpp"

namespace Lithos {
    class LITHOS_API RectElement : public Element {
        public:
            RectElement();
            RectElement(float x, float y, float width, float height, SkColor color);
            ~RectElement() override = default;

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

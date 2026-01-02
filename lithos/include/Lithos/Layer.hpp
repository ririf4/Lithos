#pragma once
#include <vector>
#include <memory>
#include <include/core/SkCanvas.h>

#include "Element.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    enum class BlendMode {
        Normal,
        Multiply,
        Screen,
        Overlay
        // TODO: Add more?
    };

    class LITHOS_API Layer {
    public:
        Layer();
        ~Layer();

        void SetTransparent(bool value);
        [[nodiscard]]bool IsTransparent() const;

        void SetOpacity(float value);  // 0.0 ~ 1.0
        [[nodiscard]] float GetOpacity() const;

        void SetBlendMode(BlendMode mode);
        [[nodiscard]] BlendMode GetBlendMode() const;

        void AddElement(std::unique_ptr<Element> element);
        [[nodiscard]] const std::vector<std::unique_ptr<Element>>& GetElements() const;

        void Draw(SkCanvas* canvas) const;

    private:
        bool transparent;
        float opacity;
        BlendMode blendMode;
        std::vector<std::unique_ptr<Element>> elements;
    };
}
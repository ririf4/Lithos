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
    };

    class LITHOS_API Layer {
    public:
        Layer();
        ~Layer();

        Layer(const Layer&) = delete;
        Layer& operator=(const Layer&) = delete;

        Layer(Layer&&) = default;
        Layer& operator=(Layer&&) = default;

        void SetTransparent(bool value);
        [[nodiscard]] bool IsTransparent() const;

        void SetOpacity(float value);
        [[nodiscard]] float GetOpacity() const;

        void SetBlendMode(BlendMode mode);
        [[nodiscard]] BlendMode GetBlendMode() const;

        void AddElement(std::unique_ptr<Element> element);
        [[nodiscard]] const std::vector<std::unique_ptr<Element>>& GetElements() const;

        bool HandleEvent(const Event& event);

        void Draw(SkCanvas* canvas) const;

    private:
        bool transparent;
        float opacity;
        BlendMode blendMode;
        std::vector<std::unique_ptr<Element>> elements;
    };
}
// Layer.hpp
#pragma once
#include <string>
#include <include/core/SkBlendMode.h>
#include <include/core/SkCanvas.h>

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class LITHOS_API Layer {
        public:
            Layer();
            explicit Layer(const std::string& name);
            ~Layer();

            Layer(const Layer&) = delete;
            Layer& operator=(const Layer&) = delete;
            Layer(Layer&&) = default;
            Layer& operator=(Layer&&) = default;

            void SetName(const std::string& name);
            [[nodiscard]] const std::string& GetName() const;

            void SetOpacity(float value);
            [[nodiscard]] float GetOpacity() const;

            void SetBlendMode(SkBlendMode mode);
            [[nodiscard]] SkBlendMode GetBlendMode() const;

            void Draw(SkCanvas* canvas) const;

        private:
            std::string name;
            float opacity;
            SkBlendMode blendMode;
    };
}

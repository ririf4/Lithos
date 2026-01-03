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

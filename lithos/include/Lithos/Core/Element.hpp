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
#include "../PCH.hpp"
#include "Color.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class Window;

    class LITHOS_API Element {
        public:
            Element();
            virtual ~Element();

            Element(const Element&) = delete;
            Element& operator=(const Element&) = delete;
            Element(Element&&) = default;
            Element& operator=(Element&&) = default;

        protected:
            Window* windowPtr;

            std::weak_ptr<Element> parent;
            std::vector<std::shared_ptr<Element>> children;

            // Performance optimization: cached brushes
            mutable ComPtr<ID2D1SolidColorBrush> cachedBackgroundBrush;  ///< Cached background brush
            mutable Color cachedBackgroundColor;                         ///< Cached background color
            mutable ComPtr<ID2D1SolidColorBrush> cachedBorderBrush;      ///< Cached border brush
            mutable Color cachedBorderColor;                             ///< Cached border color

            bool visible = true;
    };

    template <typename Derived>
    class ElementBase : public Element {
        public:
            Derived& setVisible(bool isVisible) {
                this->visible = isVisible;
                return static_cast<Derived&>(*this);
            }

            Derived& setBackgroundColor(const Color& color) {
                this->cachedBackgroundColor = color;
                this->cachedBackgroundBrush = nullptr;
                return static_cast<Derived&>(*this);
            }

            bool isVisible() const {
                return this->visible;
            }
    };
}
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
#include <algorithm>

#include "../PCH.hpp"
#include "Color.hpp"
#include "Geometry.hpp"
#include "Style.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class Window;
    struct MouseEvent;

    class LITHOS_API Element : public std::enable_shared_from_this<Element> {
    public:
        Element();
        virtual ~Element();

        Element(const Element&) = delete;
        Element& operator=(const Element&) = delete;
        Element(Element&&) = default;
        Element& operator=(Element&&) = default;

        // ========= Child management =========
        template<typename T>
        T& AddChild(std::shared_ptr<T> child) {
            static_assert(std::is_base_of_v<Element, T>);
            T* ptr = child.get();
            child->parent = shared_from_this();
            child->windowPtr = this->windowPtr;
            children.push_back(std::static_pointer_cast<Element>(child));
            RequestRepaint();
            return *ptr;
        }

        template<typename T, typename... Args>
        T& AddChild(Args&&... args) {
            return AddChild(std::make_shared<T>(std::forward<Args>(args)...));
        }

        // ========== Styling (宣言のみ) ==========
        Element& width(float w);
        Element& height(float h);
        Element& opacity(float o);
        Element& visible(bool v);
        Element& backgroundColor(const Color& color);
        Element& borderColor(const Color& color);
        Element& boxShadow(float offsetX, float offsetY, float blur, Color c);

        Element& borderWidth(float w);
        Element& borderRadius(float r);
        Element& margin(float all);
        Element& margin(float tb, float lr);
        Element& margin(float top, float right, float bottom, float left);
        Element& padding(float all);
        Element& padding(float tb, float lr);
        Element& padding(float top, float right, float bottom, float left);

        Element& cursor(CursorType c);

        virtual bool OnMouseEvent(MouseEvent evt);
        virtual void Draw(ID2D1DeviceContext* rt);
        bool HitTest(float x, float y) const;
        void RequestRepaint();
        void InvalidateLayout();

        // Getters
        float getX() const { return x; }
        float getY() const { return y; }

    protected:
        Window* windowPtr;

        std::weak_ptr<Element> parent;
        std::vector<std::shared_ptr<Element>> children;

        std::unique_ptr<Geometry> geometry;

        // Performance optimization: cached brushes
        mutable ComPtr<ID2D1SolidColorBrush> cachedBackgroundBrush;
        mutable Color cachedBackgroundColor;
        mutable ComPtr<ID2D1SolidColorBrush> cachedBorderBrush;
        mutable Color cachedBorderColor;

        float x, y;

        bool isVisible = true;

        Style style;

        friend class TransitionManager;
    };

    template <typename Derived>
    class ElementBase : public Element {
    public:
        Derived& width(float w) {
            style.width = w;
            InvalidateLayout();
            return static_cast<Derived&>(*this);
        }

        Derived& height(float h) {
            style.height = h;
            InvalidateLayout();
            return static_cast<Derived&>(*this);
        }

        Derived& opacity(float o) {
            style.opacity = std::clamp(o, 0.0f, 1.0f);
            RequestRepaint();
            return static_cast<Derived&>(*this);
        }

        Derived& visible(bool v) {
            isVisible = v;
            RequestRepaint();
            return static_cast<Derived&>(*this);
        }

        Derived& backgroundColor(const Color& color) {
            style.backgroundColor = color;
            cachedBackgroundBrush.Reset();
            RequestRepaint();
            return static_cast<Derived&>(*this);
        }

        Derived& borderColor(const Color& color) {
            style.borderColor = color;
            cachedBorderBrush.Reset();
            RequestRepaint();
            return static_cast<Derived&>(*this);
        }

        Derived& boxShadow(float offsetX, float offsetY, float blur, Color c) {
            style.shadowOffsetX = offsetX;
            style.shadowOffsetY = offsetY;
            style.shadowBlur    = blur;
            style.shadowColor   = c;
            RequestRepaint();
            return static_cast<Derived&>(*this);
        }

        Derived& borderWidth(float w) {
            style.borderWidth = w;
            RequestRepaint();
            return static_cast<Derived&>(*this);
        }

        Derived& borderRadius(float r) {
            style.borderRadius = r;
            RequestRepaint();
            return static_cast<Derived&>(*this);
        }

        // margin shorthand (all sides)
        Derived& margin(float all) {
            style.marginTop    = all;
            style.marginRight  = all;
            style.marginBottom = all;
            style.marginLeft   = all;
            InvalidateLayout();
            return static_cast<Derived&>(*this);
        }

        Derived& margin(float tb, float lr) {
            style.marginTop    = tb;
            style.marginBottom = tb;
            style.marginRight  = lr;
            style.marginLeft   = lr;
            InvalidateLayout();
            return static_cast<Derived&>(*this);
        }

        Derived& margin(float top, float right, float bottom, float left) {
            style.marginTop    = top;
            style.marginRight  = right;
            style.marginBottom = bottom;
            style.marginLeft   = left;
            InvalidateLayout();
            return static_cast<Derived&>(*this);
        }

        // padding shorthand (all sides)
        Derived& padding(float all) {
            style.paddingTop    = all;
            style.paddingRight  = all;
            style.paddingBottom = all;
            style.paddingLeft   = all;
            InvalidateLayout();
            return static_cast<Derived&>(*this);
        }

        Derived& padding(float tb, float lr) {
            style.paddingTop    = tb;
            style.paddingBottom = tb;
            style.paddingRight  = lr;
            style.paddingLeft   = lr;
            InvalidateLayout();
            return static_cast<Derived&>(*this);
        }

        Derived& padding(float top, float right, float bottom, float left) {
            style.paddingTop    = top;
            style.paddingRight  = right;
            style.paddingBottom = bottom;
            style.paddingLeft   = left;
            InvalidateLayout();
            return static_cast<Derived&>(*this);
        }

        Derived& cursor(CursorType c) {
            style.cursor = c;
            //TODO: Windowに伝える
            return static_cast<Derived&>(*this);
        }
    };
}
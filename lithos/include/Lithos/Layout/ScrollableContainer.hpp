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

#include "Container.hpp"
#include "Lithos/Core/Color.hpp"
#include "Lithos/Core/Event.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class LITHOS_API ScrollableContainer : public Container {
        public:
            ScrollableContainer();
            ~ScrollableContainer() override = default;

            ScrollableContainer(const ScrollableContainer&) = delete;
            ScrollableContainer& operator=(const ScrollableContainer&) = delete;
            ScrollableContainer(ScrollableContainer&&) = delete;
            ScrollableContainer& operator=(ScrollableContainer&&) = delete;

            // Scroll control
            ScrollableContainer& SetScrollX(float x);
            ScrollableContainer& SetScrollY(float y);
            ScrollableContainer& ScrollBy(float dx, float dy);

            float GetScrollX() const { return scrollX; }
            float GetScrollY() const { return scrollY; }

            // Scrollbar styling
            ScrollableContainer& SetScrollbarWidth(float width);
            ScrollableContainer& SetScrollbarColor(Color color);
            ScrollableContainer& SetScrollbarTrackColor(Color color);

            float GetScrollbarWidth() const { return scrollbarWidth; }

            // Enable/disable scrollbars
            ScrollableContainer& SetShowScrollbar(bool show);
            bool GetShowScrollbar() const { return showScrollbar; }

            // Mouse wheel sensitivity
            ScrollableContainer& SetWheelScrollSpeed(float speed);
            float GetWheelScrollSpeed() const { return wheelScrollSpeed; }

            void Layout() override;
            void Draw(ID2D1DeviceContext* rt) override;
            bool OnEvent(const Event& event) override;

        private:
            // Scroll state
            float scrollX;
            float scrollY;
            float contentWidth;
            float contentHeight;

            // Scrollbar appearance
            float scrollbarWidth;
            Color scrollbarColor;
            Color scrollbarTrackColor;
            bool showScrollbar;

            // Interaction
            float wheelScrollSpeed;
            bool isDraggingScrollbar;
            float scrollbarDragStartY;
            float scrollDragStartY;

            // Helper methods
            void CalculateContentSize();
            void ClampScroll();
            void DrawScrollbar(ID2D1DeviceContext* rt);
            float GetScrollbarHeight() const;
            float GetScrollbarY() const;
            bool IsScrollbarHit(float x, float y) const;
    };
}

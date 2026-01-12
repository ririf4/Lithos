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
    /**
     * @brief Scrollable container with vertical scrolling and scrollbar
     *
     * ScrollableContainer extends Container to provide scrolling functionality:
     * - Vertical scrolling with mouse wheel support
     * - Draggable scrollbar with visual feedback
     * - Customizable scrollbar appearance (width, colors)
     * - Automatic content size calculation
     * - Clipped rendering (content outside bounds is hidden)
     * - Scroll offset transformation for child events
     *
     * Content that exceeds the container bounds can be scrolled vertically.
     * Horizontal scrolling is supported but currently only vertical scrollbar is rendered.
     *
     * @note ScrollableContainer implements a subset of scrolling features (vertical only).
     */
    class LITHOS_API ScrollableContainer : public Container {
        public:
            ScrollableContainer();
            ~ScrollableContainer() override = default;

            ScrollableContainer(const ScrollableContainer&) = delete;
            ScrollableContainer& operator=(const ScrollableContainer&) = delete;
            ScrollableContainer(ScrollableContainer&&) = delete;
            ScrollableContainer& operator=(ScrollableContainer&&) = delete;

            // ========== Scroll Control ==========

            /** @brief Sets horizontal scroll position */
            ScrollableContainer& SetScrollX(float x);
            /** @brief Sets vertical scroll position */
            ScrollableContainer& SetScrollY(float y);
            /** @brief Scrolls by delta amounts (dx, dy) */
            ScrollableContainer& ScrollBy(float dx, float dy);

            /** @brief Gets current horizontal scroll offset */
            float GetScrollX() const { return scrollX; }
            /** @brief Gets current vertical scroll offset */
            float GetScrollY() const { return scrollY; }

            // ========== Scrollbar Styling ==========

            /** @brief Sets scrollbar width in pixels */
            ScrollableContainer& SetScrollbarWidth(float width);
            /** @brief Sets scrollbar thumb color */
            ScrollableContainer& SetScrollbarColor(Color color);
            /** @brief Sets scrollbar track background color */
            ScrollableContainer& SetScrollbarTrackColor(Color color);

            /** @brief Gets scrollbar width */
            float GetScrollbarWidth() const { return scrollbarWidth; }

            // ========== Scrollbar Visibility ==========

            /** @brief Shows or hides the scrollbar */
            ScrollableContainer& SetShowScrollbar(bool show);
            /** @brief Checks if scrollbar is visible */
            bool GetShowScrollbar() const { return showScrollbar; }

            // ========== Mouse Wheel Configuration ==========

            /** @brief Sets mouse wheel scroll sensitivity (pixels per wheel notch) */
            ScrollableContainer& SetWheelScrollSpeed(float speed);
            /** @brief Gets mouse wheel scroll speed */
            float GetWheelScrollSpeed() const { return wheelScrollSpeed; }

            /** @brief Performs layout and calculates content size */
            void Layout() override;
            /** @brief Renders with clipping and scroll offset transformation */
            void Draw(ID2D1DeviceContext* rt) override;
            /** @brief Handles scrollbar dragging and mouse wheel events */
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

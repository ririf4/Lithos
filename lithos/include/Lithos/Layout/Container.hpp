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
#include "Lithos/Core/Node.hpp"
#include "Lithos/Core/Style.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    /**
     * @brief Layout container with flexbox support
     *
     * Container extends Node to provide advanced layout capabilities:
     * - Flexbox layout engine (Row and Column directions)
     * - Alignment controls (JustifyContent and AlignItems)
     * - Gap spacing between children
     * - Position types (Relative and Absolute)
     *
     * Containers automatically arrange their children based on flex properties,
     * similar to CSS Flexbox.
     *
     * @note Container implements a subset of CSS Flexbox specification.
     */
    class LITHOS_API Container : public Node {
        public:
            Container();
            ~Container() override = default;

            // ========== Layout Configuration ==========

            /**
             * @brief Sets the display/layout type
             * @param display Display type (Block or Flex)
             * @return Reference to this Container for method chaining
             */
            Container& SetDisplay(Display display);

            /**
             * @brief Sets the positioning type
             * @param pos Position type (Relative or Absolute)
             * @return Reference to this Container for method chaining
             */
            Container& SetPositionType(Position pos);

            /**
             * @brief Sets the flex direction (Row or Column)
             * @param direction Flex direction
             * @return Reference to this Container for method chaining
             */
            Container& SetFlexDirection(FlexDirection direction);

            /**
             * @brief Sets main axis alignment
             * @param justify Justification mode (Start, End, Center, SpaceBetween, SpaceAround)
             * @return Reference to this Container for method chaining
             */
            Container& SetJustifyContent(JustifyContent justify);

            /**
             * @brief Sets cross axis alignment
             * @param align Alignment mode (Start, End, Center, Stretch)
             * @return Reference to this Container for method chaining
             */
            Container& SetAlignItems(AlignItems align);

            /**
             * @brief Sets gap between flex children
             * @param gap Gap size in pixels
             * @return Reference to this Container for method chaining
             */
            Container& SetGap(float gap);

            /**
             * @brief Sets flex wrap behavior for multi-line layouts
             * @param wrap Wrap mode (NoWrap, Wrap, WrapReverse)
             * @return Reference to this Container for method chaining
             */
            Container& SetFlexWrap(FlexWrap wrap);

            /**
             * @brief Sets alignment of lines in multi-line flex layout
             * @param align Alignment mode (Start, End, Center, SpaceBetween, SpaceAround, Stretch)
             * @return Reference to this Container for method chaining
             */
            Container& SetAlignContent(AlignContent align);

            /**
             * @brief Sets flex shrink factor
             * @param shrink Shrink factor (0 = no shrinking)
             * @return Reference to this Container for method chaining
             */
            Container& SetFlexShrink(float shrink);

            // ========== Node Method Overrides (Type-Safe Chaining) ==========
            // Non-virtual function shadowing is intentional for performance and type safety

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4266)  // Suppress non-virtual function hiding warning
#endif

            /**
             * @brief Sets position - returns Container& for type-safe chaining
             * @param x X coordinate
             * @param y Y coordinate
             * @return Reference to this Container for method chaining
             */
            Container& SetPosition(float x, float y) {
                SetPositionInternal(x, y);
                return *this;
            }

            /**
             * @brief Sets right offset - returns Container& for type-safe chaining
             * @param right Right offset
             * @return Reference to this Container for method chaining
             */
            Container& SetRight(float right) {
                SetRightInternal(right);
                return *this;
            }

            /**
             * @brief Sets bottom offset - returns Container& for type-safe chaining
             * @param bottom Bottom offset
             * @return Reference to this Container for method chaining
             */
            Container& SetBottom(float bottom) {
                SetBottomInternal(bottom);
                return *this;
            }

            /**
             * @brief Sets size - returns Container& for type-safe chaining
             * @param width Width
             * @param height Height
             * @return Reference to this Container for method chaining
             */
            Container& SetSize(float width, float height) {
                SetSizeInternal(width, height);
                return *this;
            }

            /**
             * @brief Sets width - returns Container& for type-safe chaining
             * @param width Width
             * @return Reference to this Container for method chaining
             */
            Container& SetWidth(float width) {
                SetWidthInternal(width);
                return *this;
            }

            /**
             * @brief Sets height - returns Container& for type-safe chaining
             * @param height Height
             * @return Reference to this Container for method chaining
             */
            Container& SetHeight(float height) {
                SetHeightInternal(height);
                return *this;
            }

            /**
             * @brief Sets background color - returns Container& for type-safe chaining
             * @param color Background color
             * @return Reference to this Container for method chaining
             */
            Container& SetBackgroundColor(Color color) {
                SetBackgroundColorInternal(color);
                return *this;
            }

            /**
             * @brief Sets border color - returns Container& for type-safe chaining
             * @param color Border color
             * @return Reference to this Container for method chaining
             */
            Container& SetBorderColor(Color color) {
                SetBorderColorInternal(color);
                return *this;
            }

            /**
             * @brief Sets border width - returns Container& for type-safe chaining
             * @param width Border width
             * @return Reference to this Container for method chaining
             */
            Container& SetBorderWidth(float width) {
                SetBorderWidthInternal(width);
                return *this;
            }

            /**
             * @brief Sets border radius - returns Container& for type-safe chaining
             * @param radius Border radius
             * @return Reference to this Container for method chaining
             */
            Container& SetBorderRadius(float radius) {
                SetBorderRadiusInternal(radius);
                return *this;
            }

            /**
             * @brief Sets opacity - returns Container& for type-safe chaining
             * @param opacity Opacity (0.0-1.0)
             * @return Reference to this Container for method chaining
             */
            Container& SetOpacity(float opacity) {
                SetOpacityInternal(opacity);
                return *this;
            }

#ifdef _MSC_VER
#pragma warning(pop)
#endif

            // ========== Child Management ==========

            /**
             * @brief Creates and adds a new Container as a child
             * @return Reference to the newly created Container
             */
            Container& AddContainer();

            /**
             * @brief Performs flexbox layout calculation
             */
            void Layout() override;

            /**
             * @brief Renders the container and its children
             * @param rt Direct2D device context
             */
            void Draw(ID2D1DeviceContext* rt) override;
    };
}
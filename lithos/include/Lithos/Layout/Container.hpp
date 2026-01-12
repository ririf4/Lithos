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
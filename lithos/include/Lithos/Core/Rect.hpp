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

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    /**
     * @brief Rectangle geometry structure for bounds and collision detection
     *
     * Represents a rectangle with position (x, y) and dimensions (width, height).
     * Used throughout the framework for element bounds, hit testing, and layout calculations.
     */
    struct Rect {
        float x, y, width, height; ///< Position and dimensions of the rectangle

        /**
         * @brief Default constructor - creates a zero-sized rectangle at origin
         */
        Rect() : x(0), y(0), width(0), height(0) {}

        /**
         * @brief Constructs a rectangle with specified position and size
         * @param x Left edge position
         * @param y Top edge position
         * @param width Rectangle width
         * @param height Rectangle height
         */
        Rect(const float x, const float y, const float width, const float height)
            : x(x), y(y), width(width), height(height) {}

        /**
         * @brief Gets the left edge position
         * @return X coordinate of the left edge
         */
        float Left() const { return x; }

        /**
         * @brief Gets the top edge position
         * @return Y coordinate of the top edge
         */
        float Top() const { return y; }

        /**
         * @brief Gets the right edge position
         * @return X coordinate of the right edge (x + width)
         */
        float Right() const { return x + width; }

        /**
         * @brief Gets the bottom edge position
         * @return Y coordinate of the bottom edge (y + height)
         */
        float Bottom() const { return y + height; }

        /**
         * @brief Tests if a point is inside the rectangle
         * @param px X coordinate of the point to test
         * @param py Y coordinate of the point to test
         * @return true if the point is inside or on the boundary of the rectangle
         */
        bool Contains(const float px, const float py) const {
            return px >= x && px < x + width &&
                   py >= y && py < y + height;
        }
    };
}
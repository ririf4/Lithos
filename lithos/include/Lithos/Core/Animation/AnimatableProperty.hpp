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
#include <variant>
#include <string>
#include <utility>
#include "Lithos/Core/Color.hpp"

namespace Lithos {
    /**
     * @brief Enumeration of animatable properties
     *
     * Defines which Node properties can be animated using transitions or keyframe animations.
     */
    enum class AnimatableProperty {
        // Position
        Left,           ///< X position (style.left)
        Top,            ///< Y position (style.top)
        Right,          ///< Right offset (style.right)
        Bottom,         ///< Bottom offset (style.bottom)
        Position,       ///< Both X and Y position

        // Size
        Width,          ///< Width (style.width)
        Height,         ///< Height (style.height)
        Size,           ///< Both width and height

        // Visual
        Opacity,              ///< Opacity (style.opacity)
        BackgroundColor,      ///< Background color (style.backgroundColor)
        BorderColor,          ///< Border color (style.borderColor)
        BorderWidth,          ///< Border width (style.borderWidth)
        BorderRadius,         ///< Border radius (style.borderRadius)
        TextColor,            ///< Text color (style.textColor)

        // Shadow
        ShadowOffsetX,        ///< Shadow X offset (style.shadowOffsetX)
        ShadowOffsetY,        ///< Shadow Y offset (style.shadowOffsetY)
        ShadowBlur,           ///< Shadow blur radius (style.shadowBlur)
        ShadowColor,          ///< Shadow color (style.shadowColor)

        // Padding
        Padding,              ///< Uniform padding (all sides)
        PaddingTop,           ///< Top padding (style.paddingTop)
        PaddingRight,         ///< Right padding (style.paddingRight)
        PaddingBottom,        ///< Bottom padding (style.paddingBottom)
        PaddingLeft,          ///< Left padding (style.paddingLeft)

        // Margin
        Margin,               ///< Uniform margin (all sides)
        MarginTop,            ///< Top margin (style.marginTop)
        MarginRight,          ///< Right margin (style.marginRight)
        MarginBottom,         ///< Bottom margin (style.marginBottom)
        MarginLeft            ///< Left margin (style.marginLeft)
    };

    /**
     * @brief Variant type for property values
     *
     * Can hold:
     * - float: for single values (width, height, opacity, etc.)
     * - Color: for color values (backgroundColor, borderColor, etc.)
     * - std::pair<float, float>: for paired values (position, size)
     */
    using PropertyValue = std::variant<float, Color, std::pair<float, float>>;

    /**
     * @brief Linear interpolation for float values
     * @param start Starting value
     * @param end Ending value
     * @param t Interpolation factor (0.0 to 1.0)
     * @return Interpolated value
     */
    inline float LerpFloat(float start, float end, float t) {
        return start + (end - start) * t;
    }

    /**
     * @brief Linear interpolation for Color values
     * @param start Starting color
     * @param end Ending color
     * @param t Interpolation factor (0.0 to 1.0)
     * @return Interpolated color
     */
    inline Color LerpColor(const Color& start, const Color& end, float t) {
        return Color(
            LerpFloat(start.r, end.r, t),
            LerpFloat(start.g, end.g, t),
            LerpFloat(start.b, end.b, t),
            LerpFloat(start.a, end.a, t)
        );
    }

    /**
     * @brief Linear interpolation for paired float values
     * @param start Starting pair
     * @param end Ending pair
     * @param t Interpolation factor (0.0 to 1.0)
     * @return Interpolated pair
     */
    inline std::pair<float, float> LerpPair(const std::pair<float, float>& start,
                                             const std::pair<float, float>& end,
                                             float t) {
        return {
            LerpFloat(start.first, end.first, t),
            LerpFloat(start.second, end.second, t)
        };
    }

    /**
     * @brief Generic interpolation for PropertyValue variant
     * @param start Starting value
     * @param end Ending value
     * @param t Interpolation factor (0.0 to 1.0)
     * @return Interpolated value
     */
    inline PropertyValue LerpPropertyValue(const PropertyValue& start,
                                           const PropertyValue& end,
                                           float t) {
        return std::visit([&](auto&& startVal, auto&& endVal) -> PropertyValue {
            using StartT = std::decay_t<decltype(startVal)>;
            using EndT = std::decay_t<decltype(endVal)>;

            if constexpr (std::is_same_v<StartT, EndT>) {
                if constexpr (std::is_same_v<StartT, float>) {
                    return LerpFloat(startVal, endVal, t);
                } else if constexpr (std::is_same_v<StartT, Color>) {
                    return LerpColor(startVal, endVal, t);
                } else if constexpr (std::is_same_v<StartT, std::pair<float, float>>) {
                    return LerpPair(startVal, endVal, t);
                }
            }

            // Type mismatch - return end value
            return end;
        }, start, end);
    }
}
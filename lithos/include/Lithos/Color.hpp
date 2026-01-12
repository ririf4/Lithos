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
     * @brief RGBA color representation with floating-point components
     *
     * Color values are stored as floats in the range [0.0, 1.0] for each channel.
     * This structure provides constructors for both float-based and integer-based (0-255) color creation.
     */
    struct Color {
        float r, g, b, a; ///< Red, Green, Blue, and Alpha channels (0.0-1.0)

        /**
         * @brief Default constructor - creates opaque black color
         */
        constexpr Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

        /**
         * @brief Constructs a color from floating-point RGBA values
         * @param r Red component (0.0-1.0)
         * @param g Green component (0.0-1.0)
         * @param b Blue component (0.0-1.0)
         * @param a Alpha component (0.0-1.0), default is 1.0 (fully opaque)
         */
        constexpr Color(const float r, const float g, const float b, const float a = 1.0f)
            : r(r), g(g), b(b), a(a) {}

        /**
         * @brief Creates an opaque color from 8-bit RGB values
         * @param r Red component (0-255)
         * @param g Green component (0-255)
         * @param b Blue component (0-255)
         * @return Color object with alpha set to 1.0
         */
        static constexpr Color LRGB(const int r, const int g, const int b) {
            return Color(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
        }

        /**
         * @brief Creates a color from 8-bit RGBA values
         * @param r Red component (0-255)
         * @param g Green component (0-255)
         * @param b Blue component (0-255)
         * @param a Alpha component (0-255)
         * @return Color object with all components normalized to 0.0-1.0 range
         */
        static constexpr Color LRGBA(const int r, const int g, const int b, const int a) {
            return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
        }
    };

    // ========== Predefined Colors ==========

    /// @defgroup BasicColors Basic Colors
    /// @{
    inline constexpr auto White           = Color(1.0f, 1.0f, 1.0f, 1.0f);   ///< Pure white
    inline constexpr auto Black           = Color(0.0f, 0.0f, 0.0f, 1.0f);   ///< Pure black
    inline constexpr auto Red             = Color(1.0f, 0.0f, 0.0f, 1.0f);   ///< Pure red
    inline constexpr auto Green           = Color(0.0f, 1.0f, 0.0f, 1.0f);   ///< Pure green
    inline constexpr auto Blue            = Color(0.0f, 0.0f, 1.0f, 1.0f);   ///< Pure blue
    inline constexpr auto Yellow          = Color(1.0f, 1.0f, 0.0f, 1.0f);   ///< Pure yellow
    inline constexpr auto Cyan            = Color(0.0f, 1.0f, 1.0f, 1.0f);   ///< Pure cyan
    inline constexpr auto Magenta         = Color(1.0f, 0.0f, 1.0f, 1.0f);   ///< Pure magenta
    inline constexpr auto Orange          = Color(1.0f, 0.647f, 0.0f, 1.0f); ///< Orange
    inline constexpr auto Pink            = Color(1.0f, 0.412f, 0.706f, 1.0f); ///< Hot pink
    /// @}

    /// @defgroup GrayscaleColors Grayscale Colors
    /// @{
    inline constexpr auto Gray            = Color(0.502f, 0.502f, 0.502f, 1.0f); ///< Medium gray
    inline constexpr auto LightGray       = Color(0.827f, 0.827f, 0.827f, 1.0f); ///< Light gray
    inline constexpr auto DarkGray        = Color(0.251f, 0.251f, 0.251f, 1.0f); ///< Dark gray
    /// @}

    /// @defgroup SpecialColors Special Colors
    /// @{
    inline constexpr auto Transparent     = Color(0.0f, 0.0f, 0.0f, 0.0f);   ///< Fully transparent color
    /// @}

    /// @defgroup VibrantColors Vibrant Colors
    /// @{
    inline constexpr auto SkyBlue         = Color(0.0f, 0.812f, 1.0f, 1.0f);     ///< Sky blue
    inline constexpr auto DeepSkyBlue     = Color(0.0f, 0.588f, 1.0f, 1.0f);     ///< Deep sky blue
    inline constexpr auto SpringGreen     = Color(0.0f, 1.0f, 0.498f, 1.0f);     ///< Spring green
    inline constexpr auto ElectricPurple  = Color(0.749f, 0.0f, 1.0f, 1.0f);     ///< Electric purple
    /// @}

    /// @defgroup MetallicColors Metallic Colors
    /// @{
    inline constexpr auto Gold            = Color(0.831f, 0.686f, 0.216f, 1.0f); ///< Gold
    inline constexpr auto Silver          = Color(0.745f, 0.761f, 0.796f, 1.0f); ///< Silver
    /// @}
}
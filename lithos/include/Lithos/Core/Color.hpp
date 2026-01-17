#pragma once

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    /**
     * @brief RGBA color representation with floating-point components
     */
    struct Color {
        float r, g, b, a;

        constexpr Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

        constexpr Color(const float r, const float g, const float b, const float a = 1.0f)
            : r(r), g(g), b(b), a(a) {}

        static constexpr Color LRGB(const int r, const int g, const int b) {
            return Color(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
        }

        static constexpr Color LRGBA(const int r, const int g, const int b, const int a) {
            return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
        }
    };

    namespace Colors {
        /// @defgroup BasicColors Basic Colors
        /// @{
        inline constexpr auto White           = Color(1.0f, 1.0f, 1.0f, 1.0f);
        inline constexpr auto Black           = Color(0.0f, 0.0f, 0.0f, 1.0f);
        inline constexpr auto Red             = Color(1.0f, 0.0f, 0.0f, 1.0f);
        inline constexpr auto Green           = Color(0.0f, 1.0f, 0.0f, 1.0f);
        inline constexpr auto Blue            = Color(0.0f, 0.0f, 1.0f, 1.0f);
        inline constexpr auto Yellow          = Color(1.0f, 1.0f, 0.0f, 1.0f);
        inline constexpr auto Cyan            = Color(0.0f, 1.0f, 1.0f, 1.0f);
        inline constexpr auto Magenta         = Color(1.0f, 0.0f, 1.0f, 1.0f);
        inline constexpr auto Orange          = Color(1.0f, 0.647f, 0.0f, 1.0f);
        inline constexpr auto Pink            = Color(1.0f, 0.412f, 0.706f, 1.0f);
        /// @}

        /// @defgroup GrayscaleColors Grayscale Colors
        /// @{
        inline constexpr auto Gray            = Color(0.502f, 0.502f, 0.502f, 1.0f);
        inline constexpr auto LightGray       = Color(0.827f, 0.827f, 0.827f, 1.0f);
        inline constexpr auto DarkGray        = Color(0.251f, 0.251f, 0.251f, 1.0f);
        /// @}

        /// @defgroup SpecialColors Special Colors
        /// @{
        inline constexpr auto Transparent     = Color(0.0f, 0.0f, 0.0f, 0.0f);
        /// @}

        /// @defgroup VibrantColors Vibrant Colors
        /// @{
        inline constexpr auto SkyBlue         = Color(0.0f, 0.812f, 1.0f, 1.0f);
        inline constexpr auto DeepSkyBlue     = Color(0.0f, 0.588f, 1.0f, 1.0f);
        inline constexpr auto SpringGreen     = Color(0.0f, 1.0f, 0.498f, 1.0f);
        inline constexpr auto ElectricPurple  = Color(0.749f, 0.0f, 1.0f, 1.0f);
        /// @}

        /// @defgroup MetallicColors Metallic Colors
        /// @{
        inline constexpr auto Gold            = Color(0.831f, 0.686f, 0.216f, 1.0f);
        inline constexpr auto Silver          = Color(0.745f, 0.761f, 0.796f, 1.0f);
        /// @}
    }
}
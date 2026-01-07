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

namespace Lithos {
    struct Color {
        float r, g, b, a;

        constexpr Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
        constexpr Color(const float r, const float g, const float b, const float a = 1.0f)
            : r(r), g(g), b(b), a(a) {}

        static constexpr Color LRGB(const int r, const int g, const int b) {
            return Color(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
        }

        static constexpr Color LRGBA(const int r, const int g, const int b,const int a) {
            return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
        }
    };

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

    inline constexpr auto Gray            = Color(0.502f, 0.502f, 0.502f, 1.0f);
    inline constexpr auto LightGray       = Color(0.827f, 0.827f, 0.827f, 1.0f);
    inline constexpr auto DarkGray        = Color(0.251f, 0.251f, 0.251f, 1.0f);

    inline constexpr auto Transparent     = Color(0.0f, 0.0f, 0.0f, 0.0f);

    inline constexpr auto SkyBlue         = Color(0.0f, 0.812f, 1.0f, 1.0f);
    inline constexpr auto DeepSkyBlue     = Color(0.0f, 0.588f, 1.0f, 1.0f);
    inline constexpr auto SpringGreen     = Color(0.0f, 1.0f, 0.498f, 1.0f);
    inline constexpr auto ElectricPurple  = Color(0.749f, 0.0f, 1.0f, 1.0f);

    inline constexpr auto Gold            = Color(0.831f, 0.686f, 0.216f, 1.0f);
    inline constexpr auto Silver          = Color(0.745f, 0.761f, 0.796f, 1.0f);
}
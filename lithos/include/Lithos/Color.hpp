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

        Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
        Color(const float r, const float g, const float b, const float a = 1.0f)
            : r(r), g(g), b(b), a(a) {}

        static Color RGB(const int r, const int g, const int b) {
            return Color { r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
        }

        static Color RGBA(const int r, const int g, const int b, const int a) {
            return Color { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
        }

        static inline const Color White           = { 1.0f, 1.0f, 1.0f, 1.0f };
        static inline const Color Black           = { 0.0f, 0.0f, 0.0f, 1.0f };
        static inline const Color Red             = { 1.0f, 0.0f, 0.0f, 1.0f };
        static inline const Color Green           = { 0.0f, 1.0f, 0.0f, 1.0f };
        static inline const Color Blue            = { 0.0f, 0.0f, 1.0f, 1.0f };
        static inline const Color Yellow          = { 1.0f, 1.0f, 0.0f, 1.0f };
        static inline const Color Cyan            = { 0.0f, 1.0f, 1.0f, 1.0f };
        static inline const Color Magenta         = { 1.0f, 0.0f, 1.0f, 1.0f };
        static inline const Color Orange          = { 1.0f, 0.647f, 0.0f, 1.0f };
        static inline const Color Pink            = { 1.0f, 0.412f, 0.706f, 1.0f };

        static inline const Color Gray            = { 0.502f, 0.502f, 0.502f, 1.0f };
        static inline const Color LightGray       = { 0.827f, 0.827f, 0.827f, 1.0f };
        static inline const Color DarkGray        = { 0.251f, 0.251f, 0.251f, 1.0f };

        static inline const Color Transparent     = { 0.0f, 0.0f, 0.0f, 0.0f };

        static inline const Color SkyBlue         = { 0.0f, 0.812f, 1.0f, 1.0f };
        static inline const Color DeepSkyBlue     = { 0.0f, 0.588f, 1.0f, 1.0f };
        static inline const Color SpringGreen     = { 0.0f, 1.0f, 0.498f, 1.0f };
        static inline const Color ElectricPurple  = { 0.749f, 0.0f, 1.0f, 1.0f };

        static inline const Color Gold            = { 0.831f, 0.686f, 0.216f, 1.0f };
        static inline const Color Silver          = { 0.745f, 0.761f, 0.796f, 1.0f };
    };
}
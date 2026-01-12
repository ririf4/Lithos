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
#include <functional>
#include <cmath>
#include <numbers>

namespace Lithos {
    /**
     * @brief Easing function signature: t (0.0 to 1.0) -> interpolated value (0.0 to 1.0)
     */
    using EasingFunction = std::function<float(float)>;

    /**
     * @brief Collection of easing functions for smooth animations
     *
     * All easing functions take a normalized time value t (0.0 to 1.0) and return
     * an interpolated value (typically 0.0 to 1.0, but can overshoot for some easings).
     *
     * Inspired by CSS easing functions and Robert Penner's easing equations.
     */
    namespace Easing {
        /**
         * @brief Linear interpolation (no easing)
         * @param t Normalized time (0.0 to 1.0)
         * @return Linear interpolation value
         */
        inline float Linear(float t) {
            return t;
        }

        /**
         * @brief CSS default easing (similar to ease)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value with smooth start and end
         */
        inline float Ease(float t) {
            return t < 0.5f
                ? 2.0f * t * t
                : -1.0f + (4.0f - 2.0f * t) * t;
        }

        /**
         * @brief Cubic ease-in (accelerating from zero velocity)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value
         */
        inline float EaseIn(float t) {
            return t * t * t;
        }

        /**
         * @brief Cubic ease-out (decelerating to zero velocity)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value
         */
        inline float EaseOut(float t) {
            float f = t - 1.0f;
            return f * f * f + 1.0f;
        }

        /**
         * @brief Cubic ease-in-out (accelerating then decelerating)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value
         */
        inline float EaseInOut(float t) {
            return t < 0.5f
                ? 4.0f * t * t * t
                : 1.0f + (--t) * (2.0f * (--t)) * (2.0f * t);
        }

        /**
         * @brief Quadratic ease-in
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value
         */
        inline float EaseInQuad(float t) {
            return t * t;
        }

        /**
         * @brief Quadratic ease-out
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value
         */
        inline float EaseOutQuad(float t) {
            return t * (2.0f - t);
        }

        /**
         * @brief Quadratic ease-in-out
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value
         */
        inline float EaseInOutQuad(float t) {
            return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
        }

        /**
         * @brief Exponential ease-in (very slow start, then rapid acceleration)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value
         */
        inline float EaseInExpo(float t) {
            return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * (t - 1.0f));
        }

        /**
         * @brief Exponential ease-out (rapid deceleration)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value
         */
        inline float EaseOutExpo(float t) {
            return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
        }

        /**
         * @brief Elastic ease-in (bouncy spring effect at start)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value (can overshoot)
         */
        inline float EaseInElastic(float t) {
            if (t == 0.0f || t == 1.0f) return t;
            const float p = 0.3f;
            return -std::pow(2.0f, 10.0f * (t - 1.0f)) *
                   std::sin((t - 1.1f) * (2.0f * std::numbers::pi_v<float>) / p);
        }

        /**
         * @brief Elastic ease-out (bouncy spring effect at end)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value (can overshoot)
         */
        inline float EaseOutElastic(float t) {
            if (t == 0.0f || t == 1.0f) return t;
            const float p = 0.3f;
            return std::pow(2.0f, -10.0f * t) *
                   std::sin((t - 0.1f) * (2.0f * std::numbers::pi_v<float>) / p) + 1.0f;
        }

        /**
         * @brief Back ease-in (slight backward motion before moving forward)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value (can be slightly negative)
         */
        inline float EaseInBack(float t) {
            const float c1 = 1.70158f;
            const float c3 = c1 + 1.0f;
            return c3 * t * t * t - c1 * t * t;
        }

        /**
         * @brief Back ease-out (overshoot at end)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value (can overshoot > 1.0)
         */
        inline float EaseOutBack(float t) {
            const float c1 = 1.70158f;
            const float c3 = c1 + 1.0f;
            return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
        }

        /**
         * @brief Back ease-in-out (slight backward then overshoot)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value
         */
        inline float EaseInOutBack(float t) {
            const float c1 = 1.70158f;
            const float c2 = c1 * 1.525f;

            return t < 0.5f
                ? (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
                : (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
        }

        /**
         * @brief Bounce ease-out (bouncing ball effect)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value
         */
        inline float EaseOutBounce(float t) {
            const float n1 = 7.5625f;
            const float d1 = 2.75f;

            if (t < 1.0f / d1) {
                return n1 * t * t;
            } else if (t < 2.0f / d1) {
                t -= 1.5f / d1;
                return n1 * t * t + 0.75f;
            } else if (t < 2.5f / d1) {
                t -= 2.25f / d1;
                return n1 * t * t + 0.9375f;
            } else {
                t -= 2.625f / d1;
                return n1 * t * t + 0.984375f;
            }
        }

        /**
         * @brief Bounce ease-in (reverse bounce effect)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value
         */
        inline float EaseInBounce(float t) {
            return 1.0f - EaseOutBounce(1.0f - t);
        }

        /**
         * @brief Bounce ease-in-out (bounce at both ends)
         * @param t Normalized time (0.0 to 1.0)
         * @return Eased value
         */
        inline float EaseInOutBounce(float t) {
            return t < 0.5f
                ? (1.0f - EaseOutBounce(1.0f - 2.0f * t)) / 2.0f
                : (1.0f + EaseOutBounce(2.0f * t - 1.0f)) / 2.0f;
        }
    }
}

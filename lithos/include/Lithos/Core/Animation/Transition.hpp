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
#include <chrono>
#include <utility>
#include <vector>
#include <unordered_map>
#include "AnimatableProperty.hpp"
#include "Easing.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class Element;
    /**
     * @brief Transition configuration for a single property
     *
     * Defines how a property should transition when its value changes.
     * Similar to CSS transition property.
     */
    struct LITHOS_API TransitionConfig {
        AnimatableProperty property;
        float duration = 0.3f;              ///< Transition duration in seconds
        EasingFunction easing = Easing::Ease; ///< Easing function
        float delay = 0.0f;                 ///< Delay before transition starts (seconds)

        explicit TransitionConfig(const AnimatableProperty prop)
            : property(prop) {}

        /**
         * @brief Sets the transition duration
         * @param dur Duration in seconds
         * @return Reference to this config for chaining
         */
        TransitionConfig& SetDuration(const float dur) {
            duration = dur;
            return *this;
        }

        /**
         * @brief Sets the easing function
         * @param ease Easing function
         * @return Reference to this config for chaining
         */
        TransitionConfig& SetEasing(EasingFunction ease) {
            easing = std::move(ease);
            return *this;
        }

        /**
         * @brief Sets the transition delay
         * @param del Delay in seconds
         * @return Reference to this config for chaining
         */
        TransitionConfig& SetDelay(float del) {
            delay = del;
            return *this;
        }
    };

    /**
     * @brief Active transition instance
     *
     * Represents a transition currently running on a property.
     */
    struct LITHOS_API ActiveTransition {
        AnimatableProperty property;
        PropertyValue startValue;
        PropertyValue targetValue;
        float duration;
        float delay;
        EasingFunction easing;
        std::chrono::steady_clock::time_point startTime;
        bool isRunning = false;
        bool delayComplete = false;

        ActiveTransition(
            const AnimatableProperty prop,
            PropertyValue start,
            PropertyValue target,
            const float dur,
            const float del,
            EasingFunction ease,
            const std::chrono::steady_clock::time_point time
            )
            : property(prop),
              startValue(std::move(start)),
              targetValue(std::move(target)),
              duration(dur),
              delay(del),
              easing(std::move(ease)),
              startTime(time),
              isRunning(true),
              delayComplete(del == 0.0f) {}
    };

    /**
     * @brief Transition manager attached to each element
     *
     * Manages CSS-like transitions for property changes.
     * When a property with a configured transition changes, the manager
     * automatically creates a smooth animation to the new value.
     */
    class LITHOS_API TransitionManager {
    public:
        TransitionManager() = default;
        ~TransitionManager() = default;

        TransitionManager(const TransitionManager&) = delete;
        TransitionManager& operator=(const TransitionManager&) = delete;
        TransitionManager(TransitionManager&&) = default;
        TransitionManager& operator=(TransitionManager&&) = default;

        /**
         * @brief Adds a transition configuration for a property
         * @param config Transition configuration
         */
        void AddTransition(const TransitionConfig& config);

        /**
         * @brief Removes transition configuration for a property
         * @param property Property to remove transition from
         */
        void RemoveTransition(AnimatableProperty property);

        /**
         * @brief Removes all transition configurations
         */
        void ClearTransitions();

        /**
         * @brief Called when a property value changes
         *
         * If a transition is configured for the property, starts a new transition.
         * If a transition is already active, it will be replaced starting from the
         * current interpolated value to ensure smooth transitions.
         *
         * @param element element whose property is changing
         * @param property Property that changed
         * @param newValue New target value
         */
        void OnPropertyChange(Element* element, AnimatableProperty property, const PropertyValue& newValue);

        /**
         * @brief Updates all active transitions
         *
         * Called each frame to interpolate property values and apply them to the element.
         * This ensures the style always reflects the current animation state.
         *
         * @param element element to update
         * @param currentTime Current time point
         * @return true if there are still active transitions
         */
        bool Update(Element* element, std::chrono::steady_clock::time_point currentTime);

        /**
         * @brief Checks if any transitions are currently active
         * @return true if at least one transition is running
         */
        bool HasActiveTransitions() const { return !activeTransitions.empty(); }

        /**
         * @brief Checks if a specific property has an active transition
         * @param property Property to check
         * @return true if the property is currently transitioning
         */
        bool HasActiveTransition(AnimatableProperty property) const;

    private:
        std::unordered_map<AnimatableProperty, TransitionConfig> configs;
        std::unordered_map<AnimatableProperty, ActiveTransition> activeTransitions;

        /**
         * @brief Gets the current value of a property
         *
         * If a transition is active, returns the current interpolated value.
         * Otherwise, returns the value from the element's style.
         *
         * @param element Element to get value from
         * @param property Property to get
         * @return Current property value
         */
        PropertyValue GetCurrentValue(Element* element, AnimatableProperty property);

        /**
         * @brief Applies a value to a property on a element
         * @param element element to apply value to
         * @param property Property to set
         * @param value Value to apply
         * @param skipTransition If true, bypasses transition system
         */
        void ApplyValue(Element* element, AnimatableProperty property, const PropertyValue& value, bool skipTransition = true);
    };
}
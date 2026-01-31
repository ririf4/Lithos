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

#define NOMINMAX

#include "Lithos/Core/Animation/Transition.hpp"
#include "Lithos/Core/Element.hpp"
#include <algorithm>

namespace Lithos {
    void TransitionManager::AddTransition(const TransitionConfig& config) {
        configs.insert_or_assign(config.property, config);
    }

    void TransitionManager::RemoveTransition(AnimatableProperty property) {
        configs.erase(property);
        activeTransitions.erase(property);
    }

    void TransitionManager::ClearTransitions() {
        configs.clear();
        activeTransitions.clear();
    }

    void TransitionManager::OnPropertyChange(Element* element, AnimatableProperty property, const PropertyValue& newValue) {
        // Check if this property has a transition configured
        auto configIt = configs.find(property);
        if (configIt == configs.end()) {
            return; // No transition configured
        }

        const TransitionConfig& config = configIt->second;

        // Get current value - either from running transition or from style
        PropertyValue currentValue = GetCurrentValue(element, property);

        // Create or update the active transition
        auto now = std::chrono::steady_clock::now();

        ActiveTransition transition(
            property,
            currentValue,
            newValue,
            config.duration,
            config.delay,
            config.easing,
            now
        );

        // Insert or replace existing transition for this property
        activeTransitions.insert_or_assign(property, transition);
    }

    bool TransitionManager::Update(Element* element, std::chrono::steady_clock::time_point currentTime) {
        if (activeTransitions.empty()) {
            return false;
        }

        std::vector<AnimatableProperty> completedTransitions;

        for (auto& [property, transition] : activeTransitions) {
            // Calculate elapsed time since transition started
            float elapsed = std::chrono::duration<float>(
                currentTime - transition.startTime
            ).count();

            // Handle delay
            if (!transition.delayComplete) {
                if (elapsed < transition.delay) {
                    // Still in delay - apply start value
                    ApplyValue(element, property, transition.startValue, true);
                    continue;
                }
                transition.delayComplete = true;
            }

            // Subtract delay from elapsed time for progress calculation
            float animElapsed = elapsed - transition.delay;

            // Check if transition is complete
            if (animElapsed >= transition.duration) {
                // Apply final value
                ApplyValue(element, property, transition.targetValue, true);
                completedTransitions.push_back(property);
            } else {
                // Calculate progress (0.0 to 1.0)
                float t = animElapsed / transition.duration;

                // Apply easing function
                float easedT = transition.easing ? transition.easing(t) : t;

                // Interpolate value
                PropertyValue interpolatedValue = LerpPropertyValue(
                    transition.startValue,
                    transition.targetValue,
                    easedT
                );

                // Apply interpolated value
                ApplyValue(element, property, interpolatedValue, true);
            }
        }

        // Remove completed transitions
        for (auto property : completedTransitions) {
            activeTransitions.erase(property);
        }

        return !activeTransitions.empty();
    }

    bool TransitionManager::HasActiveTransition(AnimatableProperty property) const {
        return activeTransitions.find(property) != activeTransitions.end();
    }

    PropertyValue TransitionManager::GetCurrentValue(Element* element, AnimatableProperty property) {
        // Check if there's an active transition for this property
        auto it = activeTransitions.find(property);
        if (it != activeTransitions.end()) {
            const auto& transition = it->second;

            // Return the current interpolated value
            const float elapsed = std::chrono::duration<float>(
                std::chrono::steady_clock::now() - transition.startTime
            ).count();

            if (!transition.delayComplete && elapsed < transition.delay) {
                return transition.startValue;
            }

            const float animElapsed = elapsed - transition.delay;
            const float t = std::min(std::max(animElapsed / transition.duration, 0.0f), 1.0f);
            const float easedT = transition.easing ? transition.easing(t) : t;

            return LerpPropertyValue(
                transition.startValue,
                transition.targetValue,
                easedT
            );
        }

        // No active transition - get value from element style
        switch (property) {
            case AnimatableProperty::Left:
                return element->style.left;
            case AnimatableProperty::Top:
                return element->style.top;
            case AnimatableProperty::Right:
                return element->style.right;
            case AnimatableProperty::Bottom:
                return element->style.bottom;
            case AnimatableProperty::Position:
                return std::make_pair(element->style.left, element->style.top);

            case AnimatableProperty::Width:
                return element->style.width;
            case AnimatableProperty::Height:
                return element->style.height;
            case AnimatableProperty::Size:
                return std::make_pair(element->style.width, element->style.height);

            case AnimatableProperty::Opacity:
                return element->style.opacity;

            case AnimatableProperty::BackgroundColor:
                return element->style.backgroundColor;
            case AnimatableProperty::BorderColor:
                return element->style.borderColor;
            case AnimatableProperty::BorderWidth:
                return element->style.borderWidth;
            case AnimatableProperty::BorderRadius:
                return element->style.borderRadius;
            case AnimatableProperty::TextColor:
                return element->style.textColor;

            case AnimatableProperty::ShadowOffsetX:
                return element->style.shadowOffsetX;
            case AnimatableProperty::ShadowOffsetY:
                return element->style.shadowOffsetY;
            case AnimatableProperty::ShadowBlur:
                return element->style.shadowBlur;
            case AnimatableProperty::ShadowColor:
                return element->style.shadowColor;

            case AnimatableProperty::Padding:
                return element->style.padding;
            case AnimatableProperty::PaddingTop:
                return element->style.paddingTop;
            case AnimatableProperty::PaddingRight:
                return element->style.paddingRight;
            case AnimatableProperty::PaddingBottom:
                return element->style.paddingBottom;
            case AnimatableProperty::PaddingLeft:
                return element->style.paddingLeft;

            case AnimatableProperty::Margin:
                return element->style.margin;
            case AnimatableProperty::MarginTop:
                return element->style.marginTop;
            case AnimatableProperty::MarginRight:
                return element->style.marginRight;
            case AnimatableProperty::MarginBottom:
                return element->style.marginBottom;
            case AnimatableProperty::MarginLeft:
                return element->style.marginLeft;

            default:
                return 0.0f;
        }
    }

    void TransitionManager::ApplyValue(Element* element, AnimatableProperty property, const PropertyValue& value, bool skipTransition) {
        // Determine if this property affects layout
        bool needsLayout = false;

        switch (property) {
            case AnimatableProperty::Left:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.left = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Top:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.top = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Right:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.right = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Bottom:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.bottom = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Position:
                if (const auto* v = std::get_if<std::pair<float, float>>(&value)) {
                    element->style.left = v->first;
                    element->style.top = v->second;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Width:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.width = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Height:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.height = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Size:
                if (const auto* v = std::get_if<std::pair<float, float>>(&value)) {
                    element->style.width = v->first;
                    element->style.height = v->second;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Opacity:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.opacity = *v;
                }
                break;

            case AnimatableProperty::BackgroundColor:
                if (const Color* v = std::get_if<Color>(&value)) {
                    element->style.backgroundColor = *v;
                }
                break;

            case AnimatableProperty::BorderColor:
                if (const Color* v = std::get_if<Color>(&value)) {
                    element->style.borderColor = *v;
                }
                break;

            case AnimatableProperty::BorderWidth:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.borderWidth = *v;
                }
                break;

            case AnimatableProperty::BorderRadius:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.borderRadius = *v;
                }
                break;

            case AnimatableProperty::TextColor:
                if (const Color* v = std::get_if<Color>(&value)) {
                    element->style.textColor = *v;
                }
                break;

            case AnimatableProperty::ShadowOffsetX:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.shadowOffsetX = *v;
                }
                break;

            case AnimatableProperty::ShadowOffsetY:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.shadowOffsetY = *v;
                }
                break;

            case AnimatableProperty::ShadowBlur:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.shadowBlur = *v;
                }
                break;

            case AnimatableProperty::ShadowColor:
                if (const Color* v = std::get_if<Color>(&value)) {
                    element->style.shadowColor = *v;
                }
                break;

            case AnimatableProperty::Padding:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.padding = *v;
                    element->style.paddingTop = *v;
                    element->style.paddingRight = *v;
                    element->style.paddingBottom = *v;
                    element->style.paddingLeft = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::PaddingTop:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.paddingTop = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::PaddingRight:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.paddingRight = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::PaddingBottom:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.paddingBottom = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::PaddingLeft:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.paddingLeft = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Margin:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.marginTop = *v;
                    element->style.marginRight = *v;
                    element->style.marginBottom = *v;
                    element->style.marginLeft = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::MarginTop:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.marginTop = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::MarginRight:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.marginRight = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::MarginBottom:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.marginBottom = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::MarginLeft:
                if (const float* v = std::get_if<float>(&value)) {
                    element->style.marginLeft = *v;
                    needsLayout = true;
                }
                break;
        }

        //TODO
        // // Trigger appropriate update
        // if (needsLayout) {
        //     element->RequestLayout();
        // } else {
        //     // MarkDirty() automatically routes to RequestRepaint() when differential rendering is enabled
        //     element->MarkDirty();
        // }
    }
}
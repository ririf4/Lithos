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

#include "Lithos/Core/Animation/Transition.hpp"
#include "Lithos/Core/Node.hpp"
#include <algorithm>

namespace Lithos {
    void TransitionManager::AddTransition(const TransitionConfig& config) {
        configs[config.property] = config;
    }

    void TransitionManager::RemoveTransition(AnimatableProperty property) {
        configs.erase(property);

        // Remove any active transitions for this property
        activeTransitions.erase(
            std::remove_if(activeTransitions.begin(), activeTransitions.end(),
                [property](const ActiveTransition& t) {
                    return t.property == property;
                }),
            activeTransitions.end()
        );
    }

    void TransitionManager::ClearTransitions() {
        configs.clear();
        activeTransitions.clear();
    }

    void TransitionManager::OnPropertyChange(Node* node,
                                            AnimatableProperty property,
                                            const PropertyValue& newValue) {
        // Check if this property has a transition configured
        auto configIt = configs.find(property);
        if (configIt == configs.end()) {
            return; // No transition configured
        }

        const TransitionConfig& config = configIt->second;

        // Get current value (either from running transition or from node)
        PropertyValue currentValue = GetCurrentValue(node, property);

        // Remove any existing transition for this property
        activeTransitions.erase(
            std::remove_if(activeTransitions.begin(), activeTransitions.end(),
                [property](const ActiveTransition& t) {
                    return t.property == property;
                }),
            activeTransitions.end()
        );

        // Create new active transition
        activeTransitions.emplace_back(
            property,
            currentValue,
            newValue,
            config.duration,
            config.delay,
            config.easing,
            std::chrono::steady_clock::now()
        );
    }

    bool TransitionManager::Update(Node* node, std::chrono::steady_clock::time_point currentTime) {
        bool hasActive = false;

        auto it = activeTransitions.begin();
        while (it != activeTransitions.end()) {
            auto& transition = *it;

            // Calculate elapsed time since transition started
            auto elapsed = std::chrono::duration<float>(
                currentTime - transition.startTime
            ).count();

            // Handle delay
            if (!transition.delayComplete) {
                if (elapsed < transition.delay) {
                    hasActive = true;
                    ++it;
                    continue;
                }
                transition.delayComplete = true;
                elapsed -= transition.delay;
            } else {
                elapsed -= transition.delay;
            }

            // Check if transition is complete
            if (elapsed >= transition.duration) {
                // Apply final value and remove transition
                ApplyValue(node, transition.property, transition.targetValue, true);
                it = activeTransitions.erase(it);
            } else {
                // Calculate progress (0.0 to 1.0)
                float t = elapsed / transition.duration;

                // Apply easing function
                float easedT = transition.easing ? transition.easing(t) : t;

                // Interpolate value
                PropertyValue currentValue = LerpPropertyValue(
                    transition.startValue,
                    transition.targetValue,
                    easedT
                );

                // Apply interpolated value
                ApplyValue(node, transition.property, currentValue, true);

                hasActive = true;
                ++it;
            }
        }

        return hasActive;
    }

    bool TransitionManager::HasActiveTransition(AnimatableProperty property) const {
        return std::any_of(activeTransitions.begin(), activeTransitions.end(),
            [property](const ActiveTransition& t) {
                return t.property == property;
            });
    }

    PropertyValue TransitionManager::GetCurrentValue(Node* node, AnimatableProperty property) {
        // Check if there's an active transition for this property
        for (const auto& transition : activeTransitions) {
            if (transition.property == property) {
                // Return the current interpolated value
                auto elapsed = std::chrono::duration<float>(
                    std::chrono::steady_clock::now() - transition.startTime
                ).count();

                if (!transition.delayComplete || elapsed < transition.delay) {
                    return transition.startValue;
                }

                elapsed -= transition.delay;
                float t = std::min(elapsed / transition.duration, 1.0f);
                float easedT = transition.easing ? transition.easing(t) : t;

                return LerpPropertyValue(
                    transition.startValue,
                    transition.targetValue,
                    easedT
                );
            }
        }

        // No active transition - get value from node
        switch (property) {
            case AnimatableProperty::Left:
                return node->GetX();
            case AnimatableProperty::Top:
                return node->GetY();
            case AnimatableProperty::Position:
                return std::make_pair(node->GetX(), node->GetY());

            case AnimatableProperty::Width:
                return node->GetWidth();
            case AnimatableProperty::Height:
                return node->GetHeight();
            case AnimatableProperty::Size:
                return std::make_pair(node->GetWidth(), node->GetHeight());

            case AnimatableProperty::Opacity:
                return node->style.opacity;

            case AnimatableProperty::BackgroundColor:
                return node->style.backgroundColor;
            case AnimatableProperty::BorderColor:
                return node->style.borderColor;
            case AnimatableProperty::BorderWidth:
                return node->style.borderWidth;
            case AnimatableProperty::BorderRadius:
                return node->style.borderRadius;
            case AnimatableProperty::TextColor:
                return node->style.textColor;

            case AnimatableProperty::ShadowOffsetX:
                return node->style.shadowOffsetX;
            case AnimatableProperty::ShadowOffsetY:
                return node->style.shadowOffsetY;
            case AnimatableProperty::ShadowBlur:
                return node->style.shadowBlur;
            case AnimatableProperty::ShadowColor:
                return node->style.shadowColor;

            case AnimatableProperty::Padding:
                return node->style.padding;
            case AnimatableProperty::PaddingTop:
                return node->style.paddingTop;
            case AnimatableProperty::PaddingRight:
                return node->style.paddingRight;
            case AnimatableProperty::PaddingBottom:
                return node->style.paddingBottom;
            case AnimatableProperty::PaddingLeft:
                return node->style.paddingLeft;

            case AnimatableProperty::Margin:
                return node->style.margin;
            case AnimatableProperty::MarginTop:
                return node->style.marginTop;
            case AnimatableProperty::MarginRight:
                return node->style.marginRight;
            case AnimatableProperty::MarginBottom:
                return node->style.marginBottom;
            case AnimatableProperty::MarginLeft:
                return node->style.marginLeft;

            default:
                return 0.0f;
        }
    }

    void TransitionManager::ApplyValue(Node* node,
                                       AnimatableProperty property,
                                       const PropertyValue& value,
                                       bool skipTransition) {
        // Determine if this property affects layout
        bool needsLayout = false;

        switch (property) {
            case AnimatableProperty::Left:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.left = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Top:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.top = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Position:
                if (const auto* v = std::get_if<std::pair<float, float>>(&value)) {
                    node->style.left = v->first;
                    node->style.top = v->second;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Width:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.width = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Height:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.height = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Size:
                if (const auto* v = std::get_if<std::pair<float, float>>(&value)) {
                    node->style.width = v->first;
                    node->style.height = v->second;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Opacity:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.opacity = *v;
                }
                break;

            case AnimatableProperty::BackgroundColor:
                if (const Color* v = std::get_if<Color>(&value)) {
                    node->style.backgroundColor = *v;
                }
                break;

            case AnimatableProperty::BorderColor:
                if (const Color* v = std::get_if<Color>(&value)) {
                    node->style.borderColor = *v;
                }
                break;

            case AnimatableProperty::BorderWidth:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.borderWidth = *v;
                }
                break;

            case AnimatableProperty::BorderRadius:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.borderRadius = *v;
                }
                break;

            case AnimatableProperty::TextColor:
                if (const Color* v = std::get_if<Color>(&value)) {
                    node->style.textColor = *v;
                }
                break;

            case AnimatableProperty::ShadowOffsetX:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.shadowOffsetX = *v;
                }
                break;

            case AnimatableProperty::ShadowOffsetY:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.shadowOffsetY = *v;
                }
                break;

            case AnimatableProperty::ShadowBlur:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.shadowBlur = *v;
                }
                break;

            case AnimatableProperty::ShadowColor:
                if (const Color* v = std::get_if<Color>(&value)) {
                    node->style.shadowColor = *v;
                }
                break;

            case AnimatableProperty::Padding:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.padding = *v;
                    node->style.paddingTop = *v;
                    node->style.paddingRight = *v;
                    node->style.paddingBottom = *v;
                    node->style.paddingLeft = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::PaddingTop:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.paddingTop = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::PaddingRight:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.paddingRight = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::PaddingBottom:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.paddingBottom = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::PaddingLeft:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.paddingLeft = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::Margin:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.margin = *v;
                    node->style.marginTop = *v;
                    node->style.marginRight = *v;
                    node->style.marginBottom = *v;
                    node->style.marginLeft = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::MarginTop:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.marginTop = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::MarginRight:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.marginRight = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::MarginBottom:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.marginBottom = *v;
                    needsLayout = true;
                }
                break;

            case AnimatableProperty::MarginLeft:
                if (const float* v = std::get_if<float>(&value)) {
                    node->style.marginLeft = *v;
                    needsLayout = true;
                }
                break;
        }

        // Trigger appropriate update
        if (needsLayout) {
            node->RequestLayout();
        } else {
            node->MarkDirty();
        }
    }
}

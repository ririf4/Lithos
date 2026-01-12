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

#include "Lithos/Layout/Container.hpp"

namespace Lithos {
    Container::Container() = default;

    Container& Container::SetDisplay(const Display display) {
        style.display = display;
        RequestLayout();
        return *this;
    }

    Container& Container::SetPositionType(const Position pos) {
        style.position = pos;
        RequestLayout();
        return *this;
    }

    Container& Container::SetFlexDirection(const FlexDirection direction) {
        style.flexDirection = direction;
        RequestLayout();
        return *this;
    }

    Container& Container::SetJustifyContent(const JustifyContent justify) {
        style.justifyContent = justify;
        RequestLayout();
        return *this;
    }

    Container& Container::SetAlignItems(const AlignItems align) {
        style.alignItems = align;
        RequestLayout();
        return *this;
    }

    Container& Container::SetGap(const float gap) {
        style.gap = gap;
        RequestLayout();
        return *this;
    }

    Container& Container::AddContainer() {
        auto container = std::make_unique<Container>();
        Container& ref = *container;
        AddChild(std::move(container));
        RequestLayout();
        return ref;
    }

    void Container::Layout() {
        Node::Layout();

        if (style.display == Display::Flex) {
            float totalChildrenMainSize = 0;
            auto visibleChildrenCount = 0;

            for (const auto& child : children) {
                if (!child->IsVisible()) continue;
                visibleChildrenCount++;

                if (style.flexDirection == FlexDirection::Row) {
                    totalChildrenMainSize += child->GetWidth();
                } else {
                    totalChildrenMainSize += child->GetHeight();
                }
            }

            if (visibleChildrenCount > 1) {
                totalChildrenMainSize += style.gap * (visibleChildrenCount - 1);
            }

            float currentPos = 0;
            float extraGap = 0;

            float containerMainSize = (style.flexDirection == FlexDirection::Row) ? bounds.width : bounds.height;
            float paddingMainStart = (style.flexDirection == FlexDirection::Row) ? style.paddingLeft : style.paddingTop;
            float paddingMainEnd = (style.flexDirection == FlexDirection::Row) ? style.paddingRight : style.paddingBottom;

            float availableSpace = containerMainSize - totalChildrenMainSize - paddingMainStart - paddingMainEnd;

            switch (style.justifyContent) {
                case JustifyContent::End:
                    currentPos = availableSpace;
                    break;
                case JustifyContent::Center:
                    currentPos = availableSpace / 2.0f;
                    break;
                case JustifyContent::SpaceBetween:
                    if (visibleChildrenCount > 1) extraGap = availableSpace / (visibleChildrenCount - 1);
                    break;
                case JustifyContent::SpaceAround:
                    if (visibleChildrenCount > 0) {
                        extraGap = availableSpace / visibleChildrenCount;
                        currentPos = extraGap / 2.0f;
                    }
                    break;
                case JustifyContent::Start:
                default:
                    currentPos = 0;
                    break;
            }

            for (const auto& child : children) {
                if (!child->IsVisible()) continue;

                float crossOffset = 0;

                float containerCrossSize = (style.flexDirection == FlexDirection::Row) ? bounds.height : bounds.width;
                float paddingCrossStart = (style.flexDirection == FlexDirection::Row) ? style.paddingTop : style.paddingLeft;
                float paddingCrossEnd = (style.flexDirection == FlexDirection::Row) ? style.paddingBottom : style.paddingRight;

                float childCrossSize = (style.flexDirection == FlexDirection::Row) ? child->GetHeight() : child->GetWidth();
                float availableCrossSpace = containerCrossSize - childCrossSize - paddingCrossStart - paddingCrossEnd;

                switch (style.alignItems) {
                    case AlignItems::End:
                        crossOffset = availableCrossSpace;
                        break;
                    case AlignItems::Center:
                        crossOffset = availableCrossSpace / 2.0f;
                        break;
                    case AlignItems::Stretch:
                        if (style.flexDirection == FlexDirection::Row) {
                            child->SetHeight(containerCrossSize - paddingCrossStart - paddingCrossEnd);
                        } else {
                            child->SetWidth(containerCrossSize - paddingCrossStart - paddingCrossEnd);
                        }
                        crossOffset = 0;
                        break;
                    case AlignItems::Start:
                    default:
                        crossOffset = 0;
                        break;
                }

                if (style.flexDirection == FlexDirection::Row) {
                    child->SetPosition(paddingMainStart + currentPos, paddingCrossStart + crossOffset);
                    currentPos += child->GetWidth() + style.gap + extraGap;
                } else {
                    child->SetPosition(paddingCrossStart + crossOffset, paddingMainStart + currentPos);
                    currentPos += child->GetHeight() + style.gap + extraGap;
                }

                child->Layout();
            }
        }
    }

    void Container::Draw(ID2D1DeviceContext* rt) {
        Node::Draw(rt);
    }
}
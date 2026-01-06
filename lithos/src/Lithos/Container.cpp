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

#include "Lithos/Container.hpp"

namespace Lithos {
    Container::Container() = default;

    Container& Container::SetDisplay(const Display display) {
        style.display = display;
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
        return ref;
    }

    void Container::Layout() {
        Node::Layout();

        //TODO: Improve
        if (style.display == Display::Flex) {
            float currentPos = 0;

            for (const auto& child : children) {
                if (style.flexDirection == FlexDirection::Row) {
                    child->SetPosition(
                        bounds.x + style.paddingLeft + currentPos,
                        bounds.y + style.paddingTop
                    );
                    currentPos += child->GetWidth() + style.gap;
                } else {
                    child->SetPosition(
                        bounds.x + style.paddingLeft,
                        bounds.y + style.paddingTop + currentPos
                    );
                    currentPos += child->GetHeight() + style.gap;
                }

                child->Layout();
            }
        }
    }

    void Container::Draw(ID2D1RenderTarget* rt) {
        Node::Draw(rt);
    }
}
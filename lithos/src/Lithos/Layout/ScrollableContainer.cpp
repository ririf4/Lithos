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

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "Lithos/Layout/ScrollableContainer.hpp"
#include <algorithm>
#include <iostream>

namespace Lithos {
    ScrollableContainer::ScrollableContainer()
        : scrollX(0),
          scrollY(0),
          contentWidth(0),
          contentHeight(0),
          scrollbarWidth(8),
          scrollbarColor(Color(0.5f, 0.5f, 0.5f, 0.8f)),
          scrollbarTrackColor(Color(0.9f, 0.9f, 0.9f, 0.5f)),
          showScrollbar(true),
          wheelScrollSpeed(20.0f),
          isDraggingScrollbar(false),
          scrollbarDragStartY(0),
          scrollDragStartY(0) {}

    ScrollableContainer& ScrollableContainer::SetScrollX(float x) {
        scrollX = x;
        ClampScroll();
        MarkDirty();
        return *this;
    }

    ScrollableContainer& ScrollableContainer::SetScrollY(float y) {
        scrollY = y;
        ClampScroll();
        MarkDirty();
        return *this;
    }

    ScrollableContainer& ScrollableContainer::ScrollBy(float dx, float dy) {
        scrollX += dx;
        scrollY += dy;
        ClampScroll();
        MarkDirty();
        return *this;
    }

    ScrollableContainer& ScrollableContainer::SetScrollbarWidth(float width) {
        scrollbarWidth = width;
        MarkDirty();
        return *this;
    }

    ScrollableContainer& ScrollableContainer::SetScrollbarColor(Color color) {
        scrollbarColor = color;
        MarkDirty();
        return *this;
    }

    ScrollableContainer& ScrollableContainer::SetScrollbarTrackColor(Color color) {
        scrollbarTrackColor = color;
        MarkDirty();
        return *this;
    }

    ScrollableContainer& ScrollableContainer::SetShowScrollbar(bool show) {
        showScrollbar = show;
        MarkDirty();
        return *this;
    }

    ScrollableContainer& ScrollableContainer::SetWheelScrollSpeed(float speed) {
        wheelScrollSpeed = speed;
        return *this;
    }

    void ScrollableContainer::CalculateContentSize() {
        contentWidth = 0;
        contentHeight = 0;

        for (const auto& child : children) {
            if (!child->IsVisible()) continue;

            float childRight = child->GetX() + child->GetWidth();
            float childBottom = child->GetY() + child->GetHeight();

            contentWidth = std::max(contentWidth, childRight - bounds.x);
            contentHeight = std::max(contentHeight, childBottom - bounds.y);
        }
    }

    void ScrollableContainer::ClampScroll() {
        float maxScrollX = std::max(0.0f, contentWidth - bounds.width);
        float maxScrollY = std::max(0.0f, contentHeight - bounds.height);

        scrollX = std::clamp(scrollX, 0.0f, maxScrollX);
        scrollY = std::clamp(scrollY, 0.0f, maxScrollY);
    }

    void ScrollableContainer::Layout() {
        if (isLayouting) return;
        isLayouting = true;

        if (style.width > 0) { bounds.width = style.width; }
        if (style.height > 0) { bounds.height = style.height; }

        if (style.position == Position::Absolute) {
            bounds.x = style.left;
            bounds.y = style.top;
        }
        else {
            if (parent) {
                const Rect& parentBounds = parent->GetBounds();
                bounds.x = parentBounds.x + style.left;
                bounds.y = parentBounds.y + style.top;
            }
            else {
                bounds.x = style.left;
                bounds.y = style.top;
            }
        }

        if (style.display == Display::Flex) {
            float totalChildrenMainSize = 0;
            auto visibleChildrenCount = 0;

            for (const auto& child : children) {
                if (!child->IsVisible()) continue;
                visibleChildrenCount++;

                if (style.flexDirection == FlexDirection::Row) { totalChildrenMainSize += child->GetWidth(); }
                else { totalChildrenMainSize += child->GetHeight(); }
            }

            if (visibleChildrenCount > 1) { totalChildrenMainSize += style.gap * (visibleChildrenCount - 1); }

            float currentPos = 0;
            float extraGap = 0;

            float containerMainSize = (style.flexDirection == FlexDirection::Row)
                                          ? bounds.width
                                          : bounds.height;
            float paddingMainStart = (style.flexDirection == FlexDirection::Row)
                                         ? style.paddingLeft
                                         : style.paddingTop;
            float paddingMainEnd = (style.flexDirection == FlexDirection::Row)
                                       ? style.paddingRight
                                       : style.paddingBottom;

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

                float containerCrossSize = (style.flexDirection == FlexDirection::Row)
                                               ? bounds.height
                                               : bounds.width;
                float paddingCrossStart = (style.flexDirection == FlexDirection::Row)
                                              ? style.paddingTop
                                              : style.paddingLeft;
                float paddingCrossEnd = (style.flexDirection == FlexDirection::Row)
                                            ? style.paddingBottom
                                            : style.paddingRight;

                float childCrossSize = (style.flexDirection == FlexDirection::Row)
                                           ? child->GetHeight()
                                           : child->GetWidth();
                float availableCrossSpace = containerCrossSize - childCrossSize - paddingCrossStart - paddingCrossEnd;

                switch (style.alignItems) {
                    case AlignItems::End:
                        crossOffset = availableCrossSpace;
                        break;
                    case AlignItems::Center:
                        crossOffset = availableCrossSpace / 2.0f;
                        break;
                    case AlignItems::Stretch:
                        if (style.flexDirection == FlexDirection::Row) { child->SetHeight(containerCrossSize - paddingCrossStart - paddingCrossEnd); }
                        else { child->SetWidth(containerCrossSize - paddingCrossStart - paddingCrossEnd); }
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
                }
                else {
                    child->SetPosition(paddingCrossStart + crossOffset, paddingMainStart + currentPos);
                    currentPos += child->GetHeight() + style.gap + extraGap;
                }
            }
        }

        for (const auto& child : children) { child->Layout(); }

        // Calculate content size
        CalculateContentSize();

        // Clamp scroll to valid range
        ClampScroll();

        isDirty = false;
        isLayouting = false;
    }

    void ScrollableContainer::Draw(ID2D1DeviceContext* rt) {
        if (!visible) return;

        // Draw background/border (from Node)
        Node::Draw(rt);

        // Save the current state
        rt->PushAxisAlignedClip(
            D2D1::RectF(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height),
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
        );

        // Save current transform
        D2D1_MATRIX_3X2_F oldTransform;
        rt->GetTransform(&oldTransform);

        // Apply scroll offset
        D2D1_MATRIX_3X2_F scrollTransform = D2D1::Matrix3x2F::Translation(-scrollX, -scrollY);
        D2D1_MATRIX_3X2_F newTransform = scrollTransform * oldTransform;
        rt->SetTransform(newTransform);

        // Draw children with scroll offset
        for (const auto& child : children) { if (child->IsVisible()) { child->Draw(rt); } }

        // Restore transform
        rt->SetTransform(oldTransform);

        // Pop clip
        rt->PopAxisAlignedClip();

        // Draw scrollbar
        if (showScrollbar && contentHeight > bounds.height) { DrawScrollbar(rt); }
    }

    void ScrollableContainer::DrawScrollbar(ID2D1DeviceContext* rt) {
        // Scrollbar track
        float trackX = bounds.x + bounds.width - scrollbarWidth;
        float trackY = bounds.y;
        float trackWidth = scrollbarWidth;
        float trackHeight = bounds.height;

        ID2D1SolidColorBrush* trackBrush = nullptr;
        rt->CreateSolidColorBrush(
            D2D1::ColorF(
                scrollbarTrackColor.r,
                scrollbarTrackColor.g,
                scrollbarTrackColor.b,
                scrollbarTrackColor.a
            ),
            &trackBrush
        );

        if (trackBrush) {
            rt->FillRectangle(
                D2D1::RectF(trackX, trackY, trackX + trackWidth, trackY + trackHeight),
                trackBrush
            );
            trackBrush->Release();
        }

        // Scrollbar thumb
        float thumbHeight = GetScrollbarHeight();
        float thumbY = GetScrollbarY();

        ID2D1SolidColorBrush* thumbBrush = nullptr;
        rt->CreateSolidColorBrush(
            D2D1::ColorF(
                scrollbarColor.r,
                scrollbarColor.g,
                scrollbarColor.b,
                scrollbarColor.a
            ),
            &thumbBrush
        );

        if (thumbBrush) {
            D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
                D2D1::RectF(trackX, thumbY, trackX + trackWidth, thumbY + thumbHeight),
                scrollbarWidth / 2.0f,
                scrollbarWidth / 2.0f
            );
            rt->FillRoundedRectangle(roundedRect, thumbBrush);
            thumbBrush->Release();
        }
    }

    float ScrollableContainer::GetScrollbarHeight() const {
        if (contentHeight <= 0) return bounds.height;
        float ratio = bounds.height / contentHeight;
        return std::max(20.0f, bounds.height * ratio);
    }

    float ScrollableContainer::GetScrollbarY() const {
        if (contentHeight <= bounds.height) return bounds.y;
        float maxScroll = contentHeight - bounds.height;
        float scrollRatio = scrollY / maxScroll;
        float maxThumbY = bounds.height - GetScrollbarHeight();
        return bounds.y + (maxThumbY * scrollRatio);
    }

    bool ScrollableContainer::IsScrollbarHit(float x, float y) const {
        if (!showScrollbar || contentHeight <= bounds.height) return false;

        float trackX = bounds.x + bounds.width - scrollbarWidth;
        float thumbY = GetScrollbarY();
        float thumbHeight = GetScrollbarHeight();

        return x >= trackX && x <= trackX + scrollbarWidth &&
            y >= thumbY && y <= thumbY + thumbHeight;
    }

    bool ScrollableContainer::OnEvent(const Event& event) {
        // Check if event is within bounds
        bool isInBounds = event.mouseX >= bounds.x && event.mouseX <= bounds.x + bounds.width &&
            event.mouseY >= bounds.y && event.mouseY <= bounds.y + bounds.height;

        // Handle scrollbar dragging
        if (event.type == EventType::MouseDown && event.button == MouseButton::Left) {
            if (IsScrollbarHit(static_cast<float>(event.mouseX), static_cast<float>(event.mouseY))) {
                isDraggingScrollbar = true;
                scrollbarDragStartY = static_cast<float>(event.mouseY);
                scrollDragStartY = scrollY;
                return true;
            }
        }

        if (event.type == EventType::MouseUp && event.button == MouseButton::Left) {
            if (isDraggingScrollbar) {
                isDraggingScrollbar = false;
                return true;
            }
        }

        if (event.type == EventType::MouseMove && isDraggingScrollbar) {
            float deltaY = static_cast<float>(event.mouseY) - scrollbarDragStartY;
            float maxScroll = contentHeight - bounds.height;
            float maxThumbY = bounds.height - GetScrollbarHeight();

            if (maxThumbY > 0) {
                float scrollDelta = (deltaY / maxThumbY) * maxScroll;
                SetScrollY(scrollDragStartY + scrollDelta);
            }
            return true;
        }

        // Handle mouse wheel scrolling (only if in bounds)
        if (event.type == EventType::MouseWheel && isInBounds) {
            float scrollAmount = -(event.wheelDelta / 120.0f) * wheelScrollSpeed;
            ScrollBy(0, scrollAmount);
            return true;
        }

        // Forward events to children with scroll offset
        if (isInBounds) {
            Event localEvent = event;
            localEvent.mouseX = static_cast<int>(event.mouseX - bounds.x + scrollX);
            localEvent.mouseY = static_cast<int>(event.mouseY - bounds.y + scrollY);

            for (auto it = children.rbegin(); it != children.rend(); ++it) { if ((*it)->OnEvent(localEvent)) { return true; } }
        }

        return false;
    }
}

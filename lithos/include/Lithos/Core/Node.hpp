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
#include <d2d1_1.h>
#include <memory>
#include <vector>
#include "Event.hpp"
#include "Rect.hpp"
#include "Style.hpp"
#include "Animation/Transition.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    /**
     * @brief Rendering priority levels for nodes
     *
     * Higher priority nodes are rendered first and are less likely to be skipped
     * when frame time budget is exceeded.
     */
    enum class RenderPriority : int {
        Critical = 100,  ///< Always rendered (UI chrome, focus indicators)
        High = 50,       ///< Important interactive elements (buttons, inputs)
        Normal = 0,      ///< Default priority (most content)
        Low = -50,       ///< Background decorations
        Deferred = -100  ///< Can be skipped if frame budget exceeded
    };

    /**
     * @brief Base class for all UI elements in the scene graph
     *
     * Node is the fundamental building block of the Lithos UI framework. It provides:
     * - Hierarchical tree structure (parent-child relationships)
     * - Layout and rendering capabilities
     * - Event handling and propagation
     * - Style properties for visual appearance
     * - Fluent API for method chaining
     *
     * All UI components (Container, Button, TextNode, etc.) inherit from Node.
     *
     * @note Nodes use unique_ptr for ownership and cannot be copied (only moved).
     */
    class LITHOS_API Node {
    public:
        Node();
        virtual ~Node();

        Node(const Node&) = delete;              ///< Copy constructor deleted
        Node& operator=(const Node&) = delete;   ///< Copy assignment deleted
        Node(Node&&) = default;                  ///< Move constructor
        Node& operator=(Node&&) = default;       ///< Move assignment

        // ========== Tree Structure ==========

        /**
         * @brief Adds a child node to this node
         * @param child Unique pointer to the child node (transfers ownership)
         */
        void AddChild(std::unique_ptr<Node> child);

        /**
         * @brief Creates and adds a child node of specified type
         * @tparam T Type of node to create (must inherit from Node)
         * @tparam Args Constructor argument types
         * @param args Arguments to forward to the node constructor
         * @return Reference to the newly created child node
         *
         * @example
         * auto& text = root.AddChild<TextNode>("Hello, World!");
         * auto& button = root.AddChild<Button>("Click Me");
         */
        template<typename T, typename... Args>
        T& AddChild(Args&&... args) {
            static_assert(std::is_base_of<Node, T>::value, "T must inherit from Node");
            auto child = std::make_unique<T>(std::forward<Args>(args)...);
            T* childPtr = child.get();
            AddChild(std::move(child));
            return *childPtr;
        }

        /**
         * @brief Gets the parent node
         * @return Pointer to parent node, or nullptr if this is the root
         */
        Node* GetParent() { return parent; }

        /**
         * @brief Gets the parent node (const version)
         * @return Const pointer to parent node, or nullptr if this is the root
         */
        const Node* GetParent() const { return parent; }

        /**
         * @brief Gets all children of this node
         * @return Const reference to vector of child nodes
         */
        const std::vector<std::unique_ptr<Node>>& GetChildren() const { return children; }

        // ========== Styling Methods (Fluent API) ==========

        /** @brief Sets position (left, top) - returns *this for chaining */
        Node& SetPosition(float x, float y);
        /** @brief Sets right offset for right-edge positioning - returns *this for chaining */
        Node& SetRight(float right);
        /** @brief Sets bottom offset for bottom-edge positioning - returns *this for chaining */
        Node& SetBottom(float bottom);
        /** @brief Sets size (width, height) - returns *this for chaining */
        Node& SetSize(float width, float height);
        /** @brief Sets width - returns *this for chaining */
        Node& SetWidth(float width);
        /** @brief Sets height - returns *this for chaining */
        Node& SetHeight(float height);
        /** @brief Sets uniform padding - returns *this for chaining */
        Node& SetPadding(float padding);
        /** @brief Sets uniform margin - returns *this for chaining */
        Node& SetMargin(float margin);
        /** @brief Sets background color - returns *this for chaining */
        Node& SetBackgroundColor(Color color);
        /** @brief Sets border color - returns *this for chaining */
        Node& SetBorderColor(Color color);
        /** @brief Sets border width - returns *this for chaining */
        Node& SetBorderWidth(float width);
        /** @brief Sets border radius for rounded corners - returns *this for chaining */
        Node& SetBorderRadius(float radius);
        /** @brief Sets visibility - returns *this for chaining */
        Node& SetVisible(bool visible);
        /** @brief Sets opacity (0.0-1.0) - returns *this for chaining */
        Node& SetOpacity(float opacity);
        /** @brief Configures drop shadow - returns *this for chaining */
        Node& SetShadow(float offsetX, float offsetY, float blur, Color color);
        /** @brief Enables/disables shadow - returns *this for chaining */
        Node& SetShadowEnabled(bool enabled);

    protected:
        // ========== Internal Implementation Methods ==========
        // These methods contain the actual implementation and are used by derived classes
        // to override public methods while maintaining type safety.

        /** @brief Internal implementation for SetPosition */
        Node& SetPositionInternal(float x, float y);
        /** @brief Internal implementation for SetRight */
        Node& SetRightInternal(float right);
        /** @brief Internal implementation for SetBottom */
        Node& SetBottomInternal(float bottom);
        /** @brief Internal implementation for SetSize */
        Node& SetSizeInternal(float width, float height);
        /** @brief Internal implementation for SetWidth */
        Node& SetWidthInternal(float width);
        /** @brief Internal implementation for SetHeight */
        Node& SetHeightInternal(float height);
        /** @brief Internal implementation for SetBackgroundColor */
        Node& SetBackgroundColorInternal(Color color);
        /** @brief Internal implementation for SetBorderColor */
        Node& SetBorderColorInternal(Color color);
        /** @brief Internal implementation for SetBorderWidth */
        Node& SetBorderWidthInternal(float width);
        /** @brief Internal implementation for SetBorderRadius */
        Node& SetBorderRadiusInternal(float radius);
        /** @brief Internal implementation for SetOpacity */
        Node& SetOpacityInternal(float opacity);

    public:

        // ========== Getters ==========

        /** @brief Gets X position */
        float GetX() const { return bounds.x; }
        /** @brief Gets Y position */
        float GetY() const { return bounds.y; }
        /** @brief Gets width */
        float GetWidth() const { return bounds.width; }
        /** @brief Gets height */
        float GetHeight() const { return bounds.height; }
        /** @brief Checks if node is visible */
        bool IsVisible() const { return visible; }
        /** @brief Gets bounding rectangle */
        const Rect& GetBounds() const { return bounds; }

        // ========== Layout and Rendering ==========

        /**
         * @brief Performs layout calculation
         *
         * Calculates and updates the bounds of this node and its children.
         * Called automatically when layout changes occur (via RequestLayout).
         * Override in derived classes for custom layout logic.
         */
        virtual void Layout();

        /**
         * @brief Renders this node and its children
         * @param rt Direct2D device context for rendering
         *
         * Draws background, borders, shadows, and recursively renders all children.
         * Override in derived classes for custom rendering.
         */
        virtual void Draw(ID2D1DeviceContext* rt);

        // ========== Event Handling ==========

        /**
         * @brief Handles events dispatched to this node
         * @param event The event to handle
         * @return true if the event was handled (stops propagation)
         *
         * Default implementation forwards events to children in reverse order
         * (topmost child first). Override for custom event handling.
         */
        virtual bool OnEvent(const Event& event);

        /**
         * @brief Tests if a point is inside this node's bounds
         * @param x X coordinate to test
         * @param y Y coordinate to test
         * @return true if the point is inside and the node is visible
         */
        bool HitTest(float x, float y) const;

        // ========== Animation and Transitions ==========

        /**
         * @brief Enables smooth transition for a property
         *
         * When the property changes, it will smoothly animate to the new value
         * instead of changing instantly. Similar to CSS transition property.
         *
         * @param property Property to animate
         * @param duration Transition duration in seconds (default: 0.3)
         * @param easing Easing function (default: Ease)
         * @return Reference to this node for chaining
         *
         * @example
         * node.Transition(AnimatableProperty::Opacity, 0.5f, Easing::EaseOut)
         *     .SetOpacity(0.0f);  // Will fade out smoothly over 0.5 seconds
         */
        Node& Transition(AnimatableProperty property, float duration = 0.3f, EasingFunction easing = Easing::Ease);

        /**
         * @brief Enables transitions for all animatable properties
         *
         * All property changes will be smoothly animated.
         *
         * @param duration Transition duration in seconds (default: 0.3)
         * @param easing Easing function (default: Ease)
         * @return Reference to this node for chaining
         */
        Node& TransitionAll(float duration = 0.3f, EasingFunction easing = Easing::Ease);

        /**
         * @brief Removes transition from a property
         *
         * After calling this, changes to the property will be instant again.
         *
         * @param property Property to remove transition from
         * @return Reference to this node for chaining
         */
        Node& RemoveTransition(AnimatableProperty property);

        /**
         * @brief Gets the transition manager for advanced configuration
         * @return Reference to the transition manager
         */
        TransitionManager& GetTransitionManager() { return transitionManager; }

        /**
         * @brief Gets the transition manager (const version)
         * @return Const reference to the transition manager
         */
        const TransitionManager& GetTransitionManager() const { return transitionManager; }

        // ========== Rendering Priority ==========

        /**
         * @brief Sets rendering priority for this node
         * @param priority Priority value (higher = rendered first, less likely to be skipped)
         * @return Reference to this node for chaining
         */
        Node& SetRenderPriority(int priority);

        /**
         * @brief Gets rendering priority
         * @return Current rendering priority
         */
        int GetRenderPriority() const { return renderPriority; }

    protected:
        Node* parent;                                ///< Parent node (nullptr for root)
        std::vector<std::unique_ptr<Node>> children; ///< Child nodes
        Style style;                                 ///< Style properties
        Rect bounds;                                 ///< Computed bounds
        bool visible;                                ///< Visibility flag
        bool isDirty;                                ///< Needs redraw flag

        /**
         * @brief Layout recursion guard flag
         *
         * When Container::Layout() sets child positions, those SetPosition() calls
         * would normally trigger RequestLayout(), causing infinite recursion.
         * This flag prevents layout during layout to break the cycle.
         *
         * @note Alternative design: Split SetPosition into user-facing and internal
         *       APIs, but this adds complexity and breaks existing code.
         */
        bool isLayouting;                            ///< Layout in progress guard

        int renderPriority;                          ///< Rendering priority (default: 0)
        TransitionManager transitionManager;         ///< Manages property transitions

        void* windowPtr;                             ///< Pointer to owning Window (void* to avoid circular dependency)

        // Performance optimization: cached brushes
        mutable ID2D1SolidColorBrush* cachedBackgroundBrush;  ///< Cached background brush
        mutable Color cachedBackgroundColor;                   ///< Cached background color
        mutable ID2D1SolidColorBrush* cachedBorderBrush;      ///< Cached border brush
        mutable Color cachedBorderColor;                       ///< Cached border color

        /**
         * @brief Marks this node and ancestors as needing redraw (legacy method)
         *
         * This method is maintained for backward compatibility. It routes to
         * RequestRepaint() when differential rendering is enabled, or falls back
         * to the old behavior otherwise.
         */
        void MarkDirty();

        /**
         * @brief Requests repaint of this node's bounds
         *
         * This is the new rendering system entry point. It calculates the dirty
         * region (including shadow/transparency expansion) and sends a repaint
         * request to the Window.
         *
         * @param priority Render priority (default: -1 uses node's renderPriority)
         */
        void RequestRepaint(int priority = -1);

        /**
         * @brief Requests repaint of a specific rectangle
         *
         * Useful for partial invalidation (e.g., scroll updates).
         *
         * @param localRect Rectangle in local coordinates
         * @param priority Render priority (default: -1 uses node's renderPriority)
         */
        void RequestRepaintRect(const Rect& localRect, int priority = -1);

        /**
         * @brief Triggers layout recalculation from root
         *
         * Walks up to the root node and calls Layout().
         * Called automatically when style properties change.
         */
        void RequestLayout();

        /**
         * @brief Requests keyboard focus for this node
         *
         * Notifies the window to set this node as the focused node.
         * Only works if the node belongs to a window.
         */
        void RequestFocus();

        /**
         * @brief Called when this node loses focus
         *
         * Override this method in derived classes to handle focus loss.
         * Default implementation does nothing.
         */
        virtual void OnLostFocus();

        /**
         * @brief Gets the window pointer
         * @return Pointer to Window (may be nullptr if not attached to window)
         */
        void* GetWindow() const { return windowPtr; }

        /**
         * @brief Sets the window pointer (called internally by Window)
         * @param window Pointer to the owning Window
         */
        void SetWindow(void* window);

        // Make Window and TransitionManager friends so they can access internals
        friend class Window;
        friend class TransitionManager;
        /**
         * @brief Safely releases a COM interface pointer
         * @tparam T Type of the interface
         * @param ptr Reference to the pointer to release
         */
        template<typename T>
        void SafeRelease(T*& ptr) const {
            if (ptr) {
                ptr->Release();
                ptr = nullptr;
            }
        }

        /**
         * @brief Gets or creates a cached brush for the given color
         * @param dc Direct2D device context
         * @param color Desired color
         * @param cachedBrush Reference to cached brush pointer
         * @param cachedColor Reference to cached color
         * @return Brush for the specified color (cached or newly created)
         */
        ID2D1SolidColorBrush* GetOrCreateBrush(
            ID2D1DeviceContext* dc,
            const Color& color,
            ID2D1SolidColorBrush*& cachedBrush,
            Color& cachedColor
        ) const;
    };
}
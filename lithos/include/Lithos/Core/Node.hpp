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

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
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
        virtual ~Node() = default;

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

    protected:
        Node* parent;                                ///< Parent node (nullptr for root)
        std::vector<std::unique_ptr<Node>> children; ///< Child nodes
        Style style;                                 ///< Style properties
        Rect bounds;                                 ///< Computed bounds
        bool visible;                                ///< Visibility flag
        bool isDirty;                                ///< Needs redraw flag
        bool isLayouting;                            ///< Layout in progress guard

        /**
         * @brief Marks this node and ancestors as needing redraw
         */
        void MarkDirty();

        /**
         * @brief Triggers layout recalculation from root
         *
         * Walks up to the root node and calls Layout().
         * Called automatically when style properties change.
         */
        void RequestLayout();
    };
}
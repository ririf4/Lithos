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
#include "Lithos/PCH.hpp"

#include "Node.hpp"
#include "Lithos/Layout/Container.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    /**
     * @brief Main application window with Direct2D rendering
     *
     * Window manages the application window and rendering pipeline:
     * - Windows API window creation and message loop
     * - Direct2D 1.1 device context for hardware-accelerated rendering
     * - Direct3D 11 swap chain for presentation
     * - Root node for the UI scene graph
     * - Event dispatching from Windows messages to nodes
     * - Automatic resize handling and DPI awareness
     *
     * Window uses the Pimpl idiom to hide implementation details and
     * maintain clean separation between public API and internal resources.
     *
     * @note Window takes ownership of the root node and all children.
     *
     * Example usage:
     * @code
     * Window window(800, 600, "My Application");
     * auto& container = window.AddContainer();
     * container.SetSize(400, 300);
     * window.Show();
     * window.Run();  // Blocks until window is closed
     * @endcode
     */
    class LITHOS_API Window {
        public:
            /**
             * @brief Rendering configuration options
             */
            struct RenderConfig {
                bool enableDifferentialRendering = true;  ///< Enable differential rendering optimization
                bool enableRegionMerging = true;          ///< Merge adjacent dirty regions
                float maxMergeGap = 10.0f;                ///< Maximum gap between regions to merge (pixels)
                float areaRatioThreshold = 1.5f;          ///< Maximum area ratio for merging (merged_area / combined_area)
                int frameTimeBudgetMs = 16;               ///< Frame time budget in milliseconds (~60 FPS)
            };

            /**
             * @brief Rendering statistics for debugging and profiling
             */
            struct RenderStats {
                int dirtyRegionCount = 0;          ///< Number of dirty regions before merging
                int mergedRegionCount = 0;         ///< Number of regions after merging
                int skippedLowPriorityCount = 0;   ///< Number of low-priority regions skipped this frame
                float lastFrameTimeMs = 0.0f;      ///< Last frame rendering time in milliseconds
            };

            /**
             * @brief Constructs a window with specified dimensions and title
             * @param width Window width in pixels
             * @param height Window height in pixels
             * @param title Window title (UTF-8 encoded)
             */
            Window(int width, int height, const std::string& title);

            ~Window();

            /**
             * @brief Shows the window (makes it visible)
             */
            void Show() const;

            /**
             * @brief Runs the window message loop (blocking)
             *
             * This method blocks until the window is closed by the user.
             * Events are processed and the window is redrawn as needed.
             */
            void Run();

            /**
             * @brief Gets the root node of the scene graph
             * @return Reference to the root Node
             */
            Node& GetRoot();

            /**
             * @brief Creates and adds a Container to the root node
             * @return Reference to the newly created Container
             */
            Container& AddContainer();

            /**
             * @brief Sets the window cursor
             * @param cursorType Windows cursor resource (e.g., IDC_ARROW)
             */
            void SetCursor(LPCWSTR cursorType);
            void SetFocusedNode(Node* node);
            Node* GetFocusedNode();

            /**
             * @brief Sets rendering configuration
             * @param config New rendering configuration
             */
            void SetRenderConfig(const RenderConfig& config);

            /**
             * @brief Gets current rendering configuration
             * @return Current rendering configuration
             */
            RenderConfig GetRenderConfig() const;

            /**
             * @brief Gets rendering statistics from last frame
             * @return Rendering statistics
             */
            RenderStats GetRenderStats() const;

        private:
            struct Impl;                    ///< Forward-declared implementation
            std::unique_ptr<Impl> pimpl;    ///< Pointer to implementation (Pimpl idiom)

            /**
             * @brief Internal method to add a dirty region (called by Node::RequestRepaint)
             * @param rect Dirty rectangle in window coordinates
             * @param priority Render priority
             */
            void AddDirtyRegion(const Rect& rect, int priority);

            friend class Node;  ///< Allow Node to call AddDirtyRegion
    };
}
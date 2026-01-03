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

// Panel.hpp
#pragma once
#include <memory>
#include <vector>
#include <include/core/SkCanvas.h>

#include "Element.hpp"
#include "Event.hpp"
#include "Layer.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class LITHOS_API Panel {
        public:
            Panel(float x, float y, float width, float height);
            ~Panel();

            Panel(const Panel&) = delete;
            Panel& operator=(const Panel&) = delete;
            Panel(Panel&&) = default;
            Panel& operator=(Panel&&) = default;

            void SetPosition(float x, float y);
            void SetSize(float width, float height);

            float GetX() const { return x; }
            float GetY() const { return y; }
            float GetWidth() const { return width; }
            float GetHeight() const { return height; }

            size_t AddLayer(std::unique_ptr<Layer> layer);
            size_t GetLayerCount() const { return layers.size(); }

            Layer* GetLayer(size_t index);
            const Layer* GetLayer(size_t index) const;
            Layer* GetLayerByName(const std::string& name);
            const Layer* GetLayerByName(const std::string& name) const;

            void AddElement(std::unique_ptr<Element> element, size_t layerIndex = 0);
            void AddElement(std::unique_ptr<Element> element, const std::string& name);

            void Draw(SkCanvas* canvas) const;
            bool HandleEvent(const Event& event);

            void OnResize(const std::function<void(Panel*, int, int)>& callback);
            void OnClickDown(const std::function<void(Panel*, MouseButton)>& callback);
            void OnClickUp(const std::function<void(Panel*, MouseButton)>& callback);
            void OnMouseMove(const std::function<void(Panel*, int, int)>& callback);
            void OnKeyDown(const std::function<void(Panel*, int)>& callback);
            void OnKeyUp(const std::function<void(Panel*, int)>& callback);

        private:
            float x, y, width, height;
            std::vector<std::unique_ptr<Layer>> layers;
            std::vector<std::vector<std::unique_ptr<Element>>> layerElements;

            std::function<void(Panel*, int, int)> resizeCallback;
            std::function<void(Panel*, MouseButton)> clickDownCallback;
            std::function<void(Panel*, MouseButton)> clickUpCallback;
            std::function<void(Panel*, int, int)> mouseMoveCallback;
            std::function<void(Panel*, int)> keyDownCallback;
            std::function<void(Panel*, int)> keyUpCallback;
    };
}
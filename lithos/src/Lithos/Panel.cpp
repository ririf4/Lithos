// Panel.cpp
#include "Lithos/Panel.hpp"
#include <algorithm>
#include <iostream>

namespace Lithos {
    Panel::Panel(const float x, const float y, const float width, const float height)
        : x(x),
          y(y),
          width(width),
          height(height) {}

    Panel::~Panel() = default;

    void Panel::SetPosition(const float x, const float y) {
        this->x = x;
        this->y = y;
    }

    void Panel::SetSize(const float width, const float height) {
        this->width = width;
        this->height = height;
    }

    size_t Panel::AddLayer(std::unique_ptr<Layer> layer) {
        layers.push_back(std::move(layer));
        layerElements.emplace_back();
        return layers.size() - 1;
    }

    Layer* Panel::GetLayer(const size_t index) {
        return index < layers.size()
                   ? layers[index].get()
                   : nullptr;
    }

    const Layer* Panel::GetLayer(const size_t index) const {
        return index < layers.size()
                   ? layers[index].get()
                   : nullptr;
    }

    Layer* Panel::GetLayerByName(const std::string& name) {
        const auto it = std::ranges::find_if(
            layers,
            [&name](const auto& layer) { return layer->GetName() == name; }
        );
        return it != layers.end()
                   ? it->get()
                   : nullptr;
    }

    const Layer* Panel::GetLayerByName(const std::string& name) const {
        const auto it = std::ranges::find_if(
            layers,
            [&name](const auto& layer) { return layer->GetName() == name; }
        );
        return it != layers.end()
                   ? it->get()
                   : nullptr;
    }

    void Panel::AddElement(std::unique_ptr<Element> element, const size_t layerIndex) {
        if (layerIndex >= layers.size()) { while (layers.size() <= layerIndex) { AddLayer(std::make_unique<Layer>()); } }

        layerElements[layerIndex].push_back(std::move(element));
    }

    void Panel::AddElement(std::unique_ptr<Element> element, const std::string& name) {
        const auto it = std::ranges::find_if(
            layers,
            [&name](const std::unique_ptr<Layer>& layer) { return layer->GetName() == name; }
        );

        if (it == layers.end()) {
            std::cerr << "[Lithos Warning] Layer not found: \"" << name << "\"." << std::endl;
            return;
        }

        AddElement(std::move(element), std::distance(layers.begin(), it));
    }

    void Panel::Draw(SkCanvas* canvas) const {
        canvas->save();

        canvas->clipRect(SkRect::MakeXYWH(x, y, width, height));

        canvas->translate(x, y);

        for (size_t i = 0; i < layers.size(); ++i) {
            const auto& layer = layers[i];
            const auto& elements = layerElements[i];

            canvas->save();

            if (layer->GetOpacity() < 1.0f) {
                SkPaint layerPaint;
                layerPaint.setAlpha(static_cast<uint8_t>(layer->GetOpacity() * 255));
                layerPaint.setBlendMode(layer->GetBlendMode());
                canvas->saveLayer(nullptr, &layerPaint);
            }

            for (const auto& element : elements) { if (element->IsVisible()) { element->Draw(canvas); } }

            if (layer->GetOpacity() < 1.0f) { canvas->restore(); }

            canvas->restore();
        }

        canvas->restore();
    }

    void Panel::OnResize(const std::function<void(Panel*, int, int)>& callback) { resizeCallback = callback; }

    void Panel::OnClickDown(const std::function<void(Panel*, MouseButton)>& callback) { clickDownCallback = callback; }

    void Panel::OnClickUp(const std::function<void(Panel*, MouseButton)>& callback) { clickUpCallback = callback; }

    void Panel::OnMouseMove(const std::function<void(Panel*, int, int)>& callback) { mouseMoveCallback = callback; }

    void Panel::OnKeyDown(const std::function<void(Panel*, int)>& callback) { keyDownCallback = callback; }

    void Panel::OnKeyUp(const std::function<void(Panel*, int)>& callback) { keyUpCallback = callback; }

    bool Panel::HandleEvent(const Event& event) {
        // WindowResizeイベント
        if (event.type == EventType::WindowResize) {
            if (resizeCallback) {
                resizeCallback(this, event.windowWidth, event.windowHeight);
            }
            // 全Elementに伝播（常に）
            for (int i = static_cast<int>(layers.size()) - 1; i >= 0; --i) {
                auto& elements = layerElements[i];
                for (const auto& element : elements) {
                    element->OnEvent(event);
                }
            }
            return false;  // 他のPanelにも伝播
        }

        // マウスイベントのバウンダリチェック
        if (event.type == EventType::MouseDown || event.type == EventType::MouseUp ||
            event.type == EventType::MouseMove) {
            if (event.mouseX < x || event.mouseX > x + width ||
                event.mouseY < y || event.mouseY > y + height) {
                return false;
                }
            }

        // Panelレベルのコールバック（監視のみ、消費しない）
        if (keyDownCallback && event.type == EventType::KeyDown) {
            keyDownCallback(this, event.key);
        }
        if (keyUpCallback && event.type == EventType::KeyUp) {
            keyUpCallback(this, event.key);
        }
        if (clickDownCallback && event.type == EventType::MouseDown) {
            clickDownCallback(this, event.button);
        }
        if (clickUpCallback && event.type == EventType::MouseUp) {
            clickUpCallback(this, event.button);
        }
        if (mouseMoveCallback && event.type == EventType::MouseMove) {
            mouseMoveCallback(this, event.mouseX, event.mouseY);
        }

        // Elementへ伝播（ローカル座標）
        Event localEvent = event;
        localEvent.mouseX -= static_cast<int>(x);
        localEvent.mouseY -= static_cast<int>(y);

        for (int i = static_cast<int>(layers.size()) - 1; i >= 0; --i) {
            auto& elements = layerElements[i];
            for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
                if ((*it)->OnEvent(localEvent)) {
                    return true;  // Elementが消費したら終了
                }
            }
        }

        return false;
    }
}

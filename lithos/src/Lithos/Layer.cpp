// Layer.cpp
#include "Lithos/Layer.hpp"
#include <algorithm>
#include "include/core/SkPaint.h"

namespace Lithos {
    Layer::Layer()
        : transparent(false)
        , opacity(1.0f)
        , blendMode(BlendMode::Normal) {
    }

    Layer::~Layer() = default;

    void Layer::SetTransparent(const bool value) {
        transparent = value;
    }

    bool Layer::IsTransparent() const {
        return transparent;
    }

    void Layer::SetOpacity(const float value) {
        opacity = std::clamp(value, 0.0f, 1.0f);
    }

    float Layer::GetOpacity() const {
        return opacity;
    }

    void Layer::SetBlendMode(const BlendMode mode) {
        blendMode = mode;
    }

    BlendMode Layer::GetBlendMode() const {
        return blendMode;
    }

    void Layer::AddElement(std::unique_ptr<Element> element) {
        elements.push_back(std::move(element));
    }

    const std::vector<std::unique_ptr<Element>>& Layer::GetElements() const {
        return elements;
    }

    bool Layer::HandleEvent(const Event& event) {
        if (!transparent && opacity <= 0.0f) {
            return false;
        }

        for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
            if ((*it)->OnEvent(event)) {
                return true;
            }
        }

        return false;
    }

    void Layer::Draw(SkCanvas* canvas) const {
        if (!transparent && opacity <= 0.0f) {
            return;
        }

        canvas->save();

        if (opacity < 1.0f) {
            SkPaint layerPaint;
            layerPaint.setAlpha(static_cast<uint8_t>(opacity * 255));
            // TODO: BlendMode適用
        }

        // 各Element描画
        for (const auto& element : elements) {
            element->Draw(canvas);
        }

        canvas->restore();
    }
}
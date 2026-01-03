// Layer.cpp
#include "Lithos/Layer.hpp"
#include <algorithm>
#include "include/core/SkPaint.h"

namespace Lithos {
    Layer::Layer()
        : name(""),
          opacity(1.0f),
          blendMode(SkBlendMode::kSrcOver) {}

    Layer::Layer(const std::string& name)
        : name(name),
          opacity(1.0f),
          blendMode(SkBlendMode::kSrcOver) {}

    Layer::~Layer() = default;

    void Layer::SetName(const std::string& name) { this->name = name; }

    const std::string& Layer::GetName() const { return name; }

    void Layer::SetOpacity(const float value) { opacity = std::clamp(value, 0.0f, 1.0f); }

    float Layer::GetOpacity() const { return opacity; }

    void Layer::SetBlendMode(const SkBlendMode mode) { blendMode = mode; }

    SkBlendMode Layer::GetBlendMode() const { return blendMode; }

    void Layer::Draw(SkCanvas* canvas) const {
        if (opacity <= 0.0f) { return; }

        canvas->save();

        if (opacity < 1.0f) {
            SkPaint layerPaint;
            layerPaint.setAlpha(static_cast<uint8_t>(opacity * 255));
            layerPaint.setBlendMode(blendMode);
            canvas->saveLayer(nullptr, &layerPaint);
        }

        canvas->restore();
    }
}

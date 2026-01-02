// example/main.cpp
#include "Lithos/Window.hpp"
#include "Lithos/Layer.hpp"
#include "Lithos/Elements/RectElement.hpp"

int main() {
    const Lithos::Window window(800, 600, "Lithos Test");

    // Layer作成
    auto layer = std::make_unique<Lithos::Layer>();

    // 背景
    auto bg = std::make_unique<Lithos::RectElement>(0, 0, 800, 600, SK_ColorDKGRAY);
    layer->AddElement(std::move(bg));

    // 赤い矩形
    auto rect1 = std::make_unique<Lithos::RectElement>(50, 50, 200, 100, SK_ColorRED);
    rect1->SetCornerRadius(10.0f);
    layer->AddElement(std::move(rect1));

    // 青い矩形
    auto rect2 = std::make_unique<Lithos::RectElement>(300, 150, 150, 150, SK_ColorBLUE);
    rect2->SetCornerRadius(20.0f);
    layer->AddElement(std::move(rect2));

    window.AddLayer(std::move(layer));

    window.Show();
    window.RunMessageLoop();

    return 0;
}
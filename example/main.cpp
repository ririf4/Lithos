#include "Lithos/Window.hpp"
#include "Lithos/Panel.hpp"
#include "Lithos/Layer.hpp"
#include "Lithos/Elements/RectElement.hpp"

int main() {
    Lithos::Window window(800, 600, "Lithos Test");

    // Panel作成
    auto panel = std::make_unique<Lithos::Panel>(0, 0, 800, 600);

    // Layer作成
    size_t bgLayer = panel->AddLayer(std::make_unique<Lithos::Layer>("background"));
    size_t uiLayer = panel->AddLayer(std::make_unique<Lithos::Layer>("ui"));

    // 背景
    auto bg = std::make_unique<Lithos::RectElement>(0, 0, 800, 600, SK_ColorDKGRAY);
    bg->OnResize([](Lithos::Element* elem, int w, int h) {
        elem->SetSize(w, h);
    });
    panel->AddElement(std::move(bg), bgLayer);

    // 赤い矩形
    auto rect1 = std::make_unique<Lithos::RectElement>(50, 50, 200, 100, SK_ColorRED);
    rect1->SetCornerRadius(10.0f);
    panel->AddElement(std::move(rect1), uiLayer);

    // 青い矩形
    auto rect2 = std::make_unique<Lithos::RectElement>(300, 150, 150, 150, SK_ColorBLUE);
    rect2->SetCornerRadius(20.0f);
    panel->AddElement(std::move(rect2), uiLayer);

    window.AddPanel(std::move(panel));

    window.Show();
    window.RunMessageLoop();

    return 0;
}
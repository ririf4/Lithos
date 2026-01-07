#include "Lithos/Color.hpp"
#include "Lithos/Container.hpp"
#include "Lithos/Window.hpp"

int main() {
    Lithos::Window window(800, 600, "Lithos Shadow Test");

    auto& root = window.GetRoot();
    root.SetBackgroundColor(Lithos::LightGray);

    // カード1：軽いシャドウ
    auto& card1 = window.AddContainer();
    card1.SetPositionType(Lithos::Position::Absolute);
    card1.SetPosition(50, 50)
        .SetSize(200, 150)
        .SetBackgroundColor(Lithos::White)
        .SetBorderRadius(15)
        .SetShadow(0, 4, 8, Lithos::Color(0, 0, 0, 0.15f));

    // カード2：中くらいのシャドウ
    auto& card2 = window.AddContainer();
    card2.SetPositionType(Lithos::Position::Absolute);
    card2.SetPosition(300, 50)
        .SetSize(200, 150)
        .SetBackgroundColor(Lithos::White)
        .SetBorderRadius(15)
        .SetShadow(0, 8, 16, Lithos::Color(0, 0, 0, 0.25f));

    // カード3：強いシャドウ
    auto& card3 = window.AddContainer();
    card3.SetPositionType(Lithos::Position::Absolute);
    card3.SetPosition(550, 50)
        .SetSize(200, 150)
        .SetBackgroundColor(Lithos::White)
        .SetBorderRadius(15)
        .SetShadow(0, 12, 24, Lithos::Color(0, 0, 0, 0.35f));

    // ボタン風：軽いシャドウ
    auto& button1 = window.AddContainer();
    button1.SetPositionType(Lithos::Position::Absolute);
    button1.SetPosition(100, 250)
        .SetSize(150, 50)
        .SetBackgroundColor(Lithos::Blue)
        .SetBorderRadius(8)
        .SetShadow(0, 2, 4, Lithos::Color(0, 0, 0, 0.2f));

    // ボタン風：中程度のシャドウ（ホバー状態風）
    auto& button2 = window.AddContainer();
    button2.SetPositionType(Lithos::Position::Absolute);
    button2.SetPosition(300, 250)
        .SetSize(150, 50)
        .SetBackgroundColor(Lithos::Blue)
        .SetBorderRadius(8)
        .SetShadow(0, 4, 8, Lithos::Color(0, 0, 0, 0.3f));

    // ボタン風：強いシャドウ（クリック風）
    auto& button3 = window.AddContainer();
    button3.SetPositionType(Lithos::Position::Absolute);
    button3.SetPosition(500, 250)
        .SetSize(150, 50)
        .SetBackgroundColor(Lithos::Blue)
        .SetBorderRadius(8)
        .SetShadow(0, 1, 2, Lithos::Color(0, 0, 0, 0.2f));

    // 円形：影付き
    auto& circle = window.AddContainer();
    circle.SetPositionType(Lithos::Position::Absolute);
    circle.SetPosition(150, 350)
        .SetSize(100, 100)
        .SetBackgroundColor(Lithos::Red)
        .SetBorderRadius(50)
        .SetShadow(0, 8, 16, Lithos::Color(0, 0, 0, 0.3f));

    // Flexコンテナ：影付きカード
    auto& flexCard = window.AddContainer();
    flexCard.SetPositionType(Lithos::Position::Absolute);
    flexCard.SetDisplay(Lithos::Display::Flex)
            .SetFlexDirection(Lithos::FlexDirection::Column)
            .SetGap(15)
            .SetPosition(300, 350)
            .SetSize(450, 200)
            .SetBackgroundColor(Lithos::White)
            .SetPadding(20)
            .SetBorderRadius(12)
            .SetShadow(0, 10, 20, Lithos::Color(0, 0, 0, 0.2f));

    // Flexカード内の子要素
    auto& flexChild1 = flexCard.AddContainer();
    flexChild1.SetSize(410, 40)
              .SetBackgroundColor(Lithos::SpringGreen)
              .SetBorderRadius(6)
              .SetShadow(0, 2, 4, Lithos::Color(0, 0, 0, 0.1f));

    auto& flexChild2 = flexCard.AddContainer();
    flexChild2.SetSize(410, 40)
              .SetBackgroundColor(Lithos::Gold)
              .SetBorderRadius(6)
              .SetShadow(0, 2, 4, Lithos::Color(0, 0, 0, 0.1f));

    auto& flexChild3 = flexCard.AddContainer();
    flexChild3.SetSize(410, 40)
              .SetBackgroundColor(Lithos::SkyBlue)
              .SetBorderRadius(6)
              .SetShadow(0, 2, 4, Lithos::Color(0, 0, 0, 0.1f));

    window.Show();
    window.Run();
    return 0;
}
#include <iostream>

#include "Lithos/Button.hpp"
#include "Lithos/Color.hpp"
#include "Lithos/Container.hpp"
#include "Lithos/ScrollableContainer.hpp"
#include "Lithos/TextNode.hpp"
#include "Lithos/Window.hpp"

int main() {
    Lithos::Window window(900, 700, "Lithos Button Test");

    auto& root = window.GetRoot();
    root.SetBackgroundColor(Lithos::Color(0.95f, 0.95f, 0.97f));

    // ========== タイトル ==========
    auto titleText = std::make_unique<Lithos::TextNode>("Button Component Test");
    titleText->SetFontSize(32)
              .SetFontWeight(Lithos::FontWeight::Bold)
              .SetTextColor(Lithos::Color(0.1f, 0.1f, 0.2f))
              .SetPosition(50, 30);
    root.AddChild(std::move(titleText));

    // ========== 基本的なボタン ==========
    auto sectionTitle1 = std::make_unique<Lithos::TextNode>("Basic Buttons");
    sectionTitle1->SetFontSize(20)
                  .SetFontWeight(Lithos::FontWeight::Medium)
                  .SetTextColor(Lithos::Color(0.2f, 0.2f, 0.3f))
                  .SetPosition(50, 100);
    root.AddChild(std::move(sectionTitle1));

    // ボタン1: デフォルトスタイル
    auto button1 = std::make_unique<Lithos::Button>("Click Me!");
    button1->SetOnClick([](Lithos::Button* btn) {
               std::cout << "Button 1 clicked!" << std::endl;
               btn->SetLabel("Clicked!");
           })
           .SetSize(150, 50)
           .SetPosition(50, 140);
    root.AddChild(std::move(button1));

    // ボタン2: カスタムカラー
    auto button2 = std::make_unique<Lithos::Button>("Custom Colors");
    button2->SetNormalColor(Lithos::Color(0.2f, 0.7f, 0.3f))  // Green
           .SetHoverColor(Lithos::Color(0.3f, 0.8f, 0.4f))
           .SetPressedColor(Lithos::Color(0.15f, 0.6f, 0.25f))
           .SetOnClick([](Lithos::Button* btn) {
               std::cout << "Button 2 clicked!" << std::endl;
           })
           .SetSize(150, 50)
           .SetPosition(220, 140);
    root.AddChild(std::move(button2));

    // ボタン3: 角丸大きめ
    auto button3 = std::make_unique<Lithos::Button>("Rounded");
    button3->SetNormalColor(Lithos::Color(0.8f, 0.3f, 0.3f))  // Red
           .SetHoverColor(Lithos::Color(0.9f, 0.4f, 0.4f))
           .SetPressedColor(Lithos::Color(0.7f, 0.2f, 0.2f))
           .SetOnClick([](Lithos::Button* btn) {
               std::cout << "Button 3 clicked!" << std::endl;
           })
           .SetBorderRadius(25)
           .SetSize(150, 50)
           .SetPosition(390, 140);
    root.AddChild(std::move(button3));

    // ========== サイズバリエーション ==========
    auto sectionTitle2 = std::make_unique<Lithos::TextNode>("Different Sizes");
    sectionTitle2->SetFontSize(20)
                  .SetFontWeight(Lithos::FontWeight::Medium)
                  .SetTextColor(Lithos::Color(0.2f, 0.2f, 0.3f))
                  .SetPosition(50, 220);
    root.AddChild(std::move(sectionTitle2));

    // 小さいボタン
    auto buttonSmall = std::make_unique<Lithos::Button>("Small");
    buttonSmall->SetFontSize(12)
               .SetOnClick([](Lithos::Button* btn) {
                   std::cout << "Small button clicked!" << std::endl;
               })
               .SetSize(100, 30)
               .SetPosition(50, 260);
    root.AddChild(std::move(buttonSmall));

    // 中サイズボタン
    auto buttonMedium = std::make_unique<Lithos::Button>("Medium");
    buttonMedium->SetFontSize(16)
                .SetOnClick([](Lithos::Button* btn) {
                    std::cout << "Medium button clicked!" << std::endl;
                })
                .SetSize(130, 40)
                .SetPosition(170, 255);
    root.AddChild(std::move(buttonMedium));

    // 大きいボタン
    auto buttonLarge = std::make_unique<Lithos::Button>("Large Button");
    buttonLarge->SetFontSize(20)
               .SetOnClick([](Lithos::Button* btn) {
                   std::cout << "Large button clicked!" << std::endl;
               })
               .SetSize(180, 60)
               .SetPosition(320, 250);
    root.AddChild(std::move(buttonLarge));

    // ========== 無効化されたボタン ==========
    auto sectionTitle3 = std::make_unique<Lithos::TextNode>("Disabled Button");
    sectionTitle3->SetFontSize(20)
                  .SetFontWeight(Lithos::FontWeight::Medium)
                  .SetTextColor(Lithos::Color(0.2f, 0.2f, 0.3f))
                  .SetPosition(50, 340);
    root.AddChild(std::move(sectionTitle3));

    auto buttonDisabled = std::make_unique<Lithos::Button>("Disabled Button");
    buttonDisabled->SetEnabled(false)
                  .SetOnClick([](Lithos::Button* btn) {
                      std::cout << "This shouldn't be called!" << std::endl;
                  })
                  .SetSize(150, 50)
                  .SetPosition(50, 380);
    root.AddChild(std::move(buttonDisabled));

    // ========== インタラクティブな例 ==========
    auto sectionTitle4 = std::make_unique<Lithos::TextNode>("Interactive Example");
    sectionTitle4->SetFontSize(20)
                  .SetFontWeight(Lithos::FontWeight::Medium)
                  .SetTextColor(Lithos::Color(0.2f, 0.2f, 0.3f))
                  .SetPosition(50, 460);
    root.AddChild(std::move(sectionTitle4));

    // カウンターテキスト
    auto counterText = std::make_unique<Lithos::TextNode>("Count: 0");
    counterText->SetFontSize(18)
               .SetTextColor(Lithos::Black)
               .SetPosition(50, 500);
    auto* counterTextPtr = counterText.get();
    root.AddChild(std::move(counterText));

    // カウントアップボタン
    int* counter = new int(0);
    auto buttonIncrement = std::make_unique<Lithos::Button>("Increment");
    buttonIncrement->SetOnClick([counter, counterTextPtr](Lithos::Button* btn) {
                       (*counter)++;
                       counterTextPtr->SetText("Count: " + std::to_string(*counter));
                       std::cout << "Count: " << *counter << std::endl;
                   })
                   .SetSize(120, 40)
                   .SetPosition(50, 530);
    root.AddChild(std::move(buttonIncrement));

    // リセットボタン
    auto buttonReset = std::make_unique<Lithos::Button>("Reset");
    buttonReset->SetNormalColor(Lithos::Color(0.7f, 0.3f, 0.3f))
               .SetHoverColor(Lithos::Color(0.8f, 0.4f, 0.4f))
               .SetPressedColor(Lithos::Color(0.6f, 0.2f, 0.2f))
               .SetOnClick([counter, counterTextPtr](Lithos::Button* btn) {
                   *counter = 0;
                   counterTextPtr->SetText("Count: 0");
                   std::cout << "Reset!" << std::endl;
               })
               .SetSize(120, 40)
               .SetPosition(190, 530);
    root.AddChild(std::move(buttonReset));

    // ========== スクロール可能なボタンリスト ==========
    auto sectionTitle5 = std::make_unique<Lithos::TextNode>("Scrollable Button List");
    sectionTitle5->SetFontSize(20)
                  .SetFontWeight(Lithos::FontWeight::Medium)
                  .SetTextColor(Lithos::Color(0.2f, 0.2f, 0.3f))
                  .SetPosition(550, 100);
    root.AddChild(std::move(sectionTitle5));

    auto scrollContainer = std::make_unique<Lithos::ScrollableContainer>();
    scrollContainer->SetDisplay(Lithos::Display::Flex)
                   .SetFlexDirection(Lithos::FlexDirection::Column)
                   .SetGap(10)
                   .SetPadding(10)
                   .SetBackgroundColor(Lithos::White)
                   .SetBorderColor(Lithos::Color(0.8f, 0.8f, 0.8f))
                   .SetBorderWidth(1)
                   .SetBorderRadius(8)
                   .SetSize(300, 450)
                   .SetPosition(550, 140);

    for (int i = 0; i < 20; i++) {
        auto btn = std::make_unique<Lithos::Button>("Button #" + std::to_string(i + 1));

        // 色をバリエーション
        Lithos::Color btnColor = Lithos::Color(
            0.3f + (i % 3) * 0.2f,
            0.3f + ((i + 1) % 3) * 0.2f,
            0.3f + ((i + 2) % 3) * 0.2f
        );

        int btnIndex = i + 1;
        btn->SetNormalColor(btnColor)
           .SetOnClick([btnIndex](Lithos::Button* b) {
               std::cout << "Clicked button #" << btnIndex << std::endl;
           })
           .SetSize(280, 40);

        scrollContainer->AddChild(std::move(btn));
    }

    root.AddChild(std::move(scrollContainer));

    window.Show();
    window.Run();

    delete counter;
    return 0;
}
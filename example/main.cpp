#include "Lithos/Color.hpp"
#include "Lithos/Container.hpp"
#include "Lithos/TextNode.hpp"
#include "Lithos/Window.hpp"

int main() {
    Lithos::Window window(900, 700, "Lithos TextNode Test");

    auto& root = window.GetRoot();
    root.SetBackgroundColor(Lithos::Color(0.95f, 0.95f, 0.97f));

    // ========== 見出しテスト ==========
    auto titleText = std::make_unique<Lithos::TextNode>("TextNode Feature Test");
    titleText->SetFontSize(32)
              .SetFontWeight(Lithos::FontWeight::Bold)
              .SetTextColor(Lithos::Color(0.1f, 0.1f, 0.2f))
              .SetPosition(50, 30);
    root.AddChild(std::move(titleText));

    // ========== フォントサイズのバリエーション ==========
    auto sectionTitle1 = std::make_unique<Lithos::TextNode>("Font Sizes");
    sectionTitle1->SetFontSize(20)
                  .SetFontWeight(Lithos::FontWeight::Medium)
                  .SetTextColor(Lithos::Color(0.2f, 0.2f, 0.3f))
                  .SetPosition(50, 90);
    root.AddChild(std::move(sectionTitle1));

    auto text12 = std::make_unique<Lithos::TextNode>("12px - Small text");
    text12->SetFontSize(12).SetTextColor(Lithos::Black).SetPosition(50, 120);
    root.AddChild(std::move(text12));

    auto text16 = std::make_unique<Lithos::TextNode>("16px - Normal text");
    text16->SetFontSize(16).SetTextColor(Lithos::Black).SetPosition(50, 140);
    root.AddChild(std::move(text16));

    auto text24 = std::make_unique<Lithos::TextNode>("24px - Large text");
    text24->SetFontSize(24).SetTextColor(Lithos::Black).SetPosition(50, 165);
    root.AddChild(std::move(text24));

    auto text36 = std::make_unique<Lithos::TextNode>("36px - Extra large text");
    text36->SetFontSize(36).SetTextColor(Lithos::Black).SetPosition(50, 200);
    root.AddChild(std::move(text36));

    // ========== フォントウェイトのバリエーション ==========
    auto sectionTitle2 = std::make_unique<Lithos::TextNode>("Font Weights");
    sectionTitle2->SetFontSize(20)
                  .SetFontWeight(Lithos::FontWeight::Medium)
                  .SetTextColor(Lithos::Color(0.2f, 0.2f, 0.3f))
                  .SetPosition(400, 90);
    root.AddChild(std::move(sectionTitle2));

    auto thinText = std::make_unique<Lithos::TextNode>("Thin Weight");
    thinText->SetFontSize(18)
             .SetFontWeight(Lithos::FontWeight::Thin)
             .SetTextColor(Lithos::Black)
             .SetPosition(400, 120);
    root.AddChild(std::move(thinText));

    auto normalText = std::make_unique<Lithos::TextNode>("Normal Weight");
    normalText->SetFontSize(18)
               .SetFontWeight(Lithos::FontWeight::Normal)
               .SetTextColor(Lithos::Black)
               .SetPosition(400, 145);
    root.AddChild(std::move(normalText));

    auto boldText = std::make_unique<Lithos::TextNode>("Bold Weight");
    boldText->SetFontSize(18)
             .SetFontWeight(Lithos::FontWeight::Bold)
             .SetTextColor(Lithos::Black)
             .SetPosition(400, 170);
    root.AddChild(std::move(boldText));

    auto blackText = std::make_unique<Lithos::TextNode>("Black Weight");
    blackText->SetFontSize(18)
              .SetFontWeight(Lithos::FontWeight::Black)
              .SetTextColor(Lithos::Black)
              .SetPosition(400, 195);
    root.AddChild(std::move(blackText));

    // ========== テキストアライメント ==========
    auto sectionTitle3 = std::make_unique<Lithos::TextNode>("Text Alignment");
    sectionTitle3->SetFontSize(20)
                  .SetFontWeight(Lithos::FontWeight::Medium)
                  .SetTextColor(Lithos::Color(0.2f, 0.2f, 0.3f))
                  .SetPosition(50, 260);
    root.AddChild(std::move(sectionTitle3));

    auto& alignContainer = window.AddContainer();
    alignContainer
       .SetFlexDirection(Lithos::FlexDirection::Column)
       .SetDisplay(Lithos::Display::Flex)
       .SetPositionType(Lithos::Position::Absolute)
       .SetPosition(50, 290)
       .SetSize(800, 100)
       .SetBackgroundColor(Lithos::Color(1.0f, 1.0f, 1.0f, 0.5f))
       .SetBorderColor(Lithos::Color(0.8f, 0.8f, 0.8f))
       .SetBorderWidth(1)
       .SetBorderRadius(8)
       .SetPadding(10);

    auto leftText = std::make_unique<Lithos::TextNode>("Left aligned text");
    leftText->SetFontSize(16)
             .SetTextAlign(Lithos::TextAlign::Left)
             .SetTextColor(Lithos::Black)
             .SetWidth(780);
    alignContainer.AddChild(std::move(leftText));

    auto centerText = std::make_unique<Lithos::TextNode>("Center aligned text");
    centerText->SetFontSize(16)
               .SetTextAlign(Lithos::TextAlign::Center)
               .SetTextColor(Lithos::Black)
               .SetWidth(780);
    alignContainer.AddChild(std::move(centerText));

    auto rightText = std::make_unique<Lithos::TextNode>("Right aligned text");
    rightText->SetFontSize(16)
              .SetTextAlign(Lithos::TextAlign::Right)
              .SetTextColor(Lithos::Black)
              .SetWidth(780);
    alignContainer.AddChild(std::move(rightText));

    // ========== カラーバリエーション ==========
    auto sectionTitle4 = std::make_unique<Lithos::TextNode>("Text Colors");
    sectionTitle4->SetFontSize(20)
                  .SetFontWeight(Lithos::FontWeight::Medium)
                  .SetTextColor(Lithos::Color(0.2f, 0.2f, 0.3f))
                  .SetPosition(50, 410);
    root.AddChild(std::move(sectionTitle4));

    auto redText = std::make_unique<Lithos::TextNode>("Red text");
    redText->SetFontSize(18).SetTextColor(Lithos::Red).SetPosition(50, 445);
    root.AddChild(std::move(redText));

    auto blueText = std::make_unique<Lithos::TextNode>("Blue text");
    blueText->SetFontSize(18).SetTextColor(Lithos::Blue).SetPosition(180, 445);
    root.AddChild(std::move(blueText));

    auto greenText = std::make_unique<Lithos::TextNode>("Green text");
    greenText->SetFontSize(18).SetTextColor(Lithos::SpringGreen).SetPosition(310, 445);
    root.AddChild(std::move(greenText));

    auto goldText = std::make_unique<Lithos::TextNode>("Gold text");
    goldText->SetFontSize(18).SetTextColor(Lithos::Gold).SetPosition(460, 445);
    root.AddChild(std::move(goldText));

    // ========== カード内のテキスト ==========
    auto& card = window.AddContainer();
    card.SetPositionType(Lithos::Position::Absolute)
        .SetPosition(50, 490)
        .SetSize(800, 150)
        .SetBackgroundColor(Lithos::White)
        .SetBorderRadius(12)
        .SetShadow(0, 4, 12, Lithos::Color(0, 0, 0, 0.1f))
        .SetPadding(20);

    auto cardTitle = std::make_unique<Lithos::TextNode>("Card with Text");
    cardTitle->SetFontSize(24)
              .SetFontWeight(Lithos::FontWeight::Bold)
              .SetTextColor(Lithos::Color(0.1f, 0.1f, 0.2f))
              .SetPosition(70, 510);
    root.AddChild(std::move(cardTitle));

    auto cardBody = std::make_unique<Lithos::TextNode>(
        "This is a card containing text with shadows and rounded corners."
    );
    cardBody->SetFontSize(16)
             .SetTextColor(Lithos::Color(0.3f, 0.3f, 0.4f))
             .SetPosition(70, 545);
    root.AddChild(std::move(cardBody));

    auto cardSubtext = std::make_unique<Lithos::TextNode>("Demonstrating TextNode integration");
    cardSubtext->SetFontSize(14)
                .SetFontStyle(Lithos::FontStyle::Italic)
                .SetTextColor(Lithos::Color(0.5f, 0.5f, 0.6f))
                .SetPosition(70, 575);
    root.AddChild(std::move(cardSubtext));

    window.Show();
    window.Run();
    return 0;
}

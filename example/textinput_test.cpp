/*
    TextInput Test
    Tests basic TextInput functionality
*/

#include <iostream>

#include "Lithos/Components/TextInput.hpp"
#include "Lithos/Components/TextNode.hpp"
#include "Lithos/Core/Color.hpp"
#include "Lithos/Core/Window.hpp"

int main() {
    std::cout << "Starting TextInput test..." << std::endl;

    Lithos::Window window(800, 600, "TextInput Test");

    auto& root = window.GetRoot();
    root.SetBackgroundColor(Lithos::Color(0.95f, 0.95f, 0.97f));

    std::cout << "Window created" << std::endl;

    // タイトル
    auto titleText = std::make_unique<Lithos::TextNode>("TextInput Test");
    titleText->SetFontSize(24)
              .SetTextColor(Lithos::Black)
              .SetPosition(50, 30);
    root.AddChild(std::move(titleText));

    std::cout << "Title added" << std::endl;

    // 通常のテキストインプット
    auto normalLabel = std::make_unique<Lithos::TextNode>("Normal Input:");
    normalLabel->SetFontSize(16)
                .SetTextColor(Lithos::Black)
                .SetPosition(50, 80);
    root.AddChild(std::move(normalLabel));

    auto normalInput = std::make_unique<Lithos::TextInput>("Enter text here...");
    normalInput->SetOnChange([](Lithos::TextInput* input, const std::string& text) {
                    std::cout << "Normal input changed: " << text << std::endl;
                })
                .SetSize(300, 40)
                .SetPosition(50, 110);
    root.AddChild(std::move(normalInput));

    std::cout << "Normal input added" << std::endl;

    // パスワード入力
    auto passwordLabel = std::make_unique<Lithos::TextNode>("Password Input:");
    passwordLabel->SetFontSize(16)
                  .SetTextColor(Lithos::Black)
                  .SetPosition(50, 170);
    root.AddChild(std::move(passwordLabel));

    auto passwordInput = std::make_unique<Lithos::TextInput>("Enter password...");
    passwordInput->SetPasswordMode(true)
                  .SetOnChange([](Lithos::TextInput* input, const std::string& text) {
                      std::cout << "Password changed (length: " << text.length() << ")" << std::endl;
                  })
                  .SetSize(300, 40)
                  .SetPosition(50, 200);
    root.AddChild(std::move(passwordInput));

    std::cout << "Password input added" << std::endl;

    // 最大文字数制限付き入力
    auto maxLengthLabel = std::make_unique<Lithos::TextNode>("Max Length (10):");
    maxLengthLabel->SetFontSize(16)
                   .SetTextColor(Lithos::Black)
                   .SetPosition(50, 260);
    root.AddChild(std::move(maxLengthLabel));

    auto maxLengthInput = std::make_unique<Lithos::TextInput>("Max 10 chars");
    maxLengthInput->SetMaxLength(10)
                   .SetOnChange([](Lithos::TextInput* input, const std::string& text) {
                       std::cout << "Max length input: " << text << " (" << text.length() << " chars)" << std::endl;
                   })
                   .SetSize(300, 40)
                   .SetPosition(50, 290);
    root.AddChild(std::move(maxLengthInput));

    std::cout << "Max length input added" << std::endl;

    // カスタムスタイルの入力
    auto styledLabel = std::make_unique<Lithos::TextNode>("Styled Input:");
    styledLabel->SetFontSize(16)
                .SetTextColor(Lithos::Black)
                .SetPosition(50, 350);
    root.AddChild(std::move(styledLabel));

    auto styledInput = std::make_unique<Lithos::TextInput>("Custom styled...");
    styledInput->SetFontSize(18)
                .SetFontFamily("Arial")
                .SetTextColor(Lithos::Color(0.2f, 0.4f, 0.8f))
                .SetPlaceholderColor(Lithos::Color(0.5f, 0.7f, 1.0f))
                .SetFocusedBorderColor(Lithos::Color(0.0f, 0.6f, 0.3f))
                .SetHoverBorderColor(Lithos::Color(0.3f, 0.7f, 0.5f))
                .SetOnChange([](Lithos::TextInput* input, const std::string& text) {
                    std::cout << "Styled input: " << text << std::endl;
                })
                .SetSize(300, 40)
                .SetPosition(50, 380)
                .SetBorderRadius(8);
    root.AddChild(std::move(styledInput));

    std::cout << "Styled input added" << std::endl;

    // 複数行入力
    auto multilineLabel = std::make_unique<Lithos::TextNode>("Multiline Input:");
    multilineLabel->SetFontSize(16)
                   .SetTextColor(Lithos::Black)
                   .SetPosition(50, 440);
    root.AddChild(std::move(multilineLabel));

    auto multilineInput = std::make_unique<Lithos::TextInput>("Enter multiple lines...");
    multilineInput->SetMultiLine(true)
                   .SetOnChange([](Lithos::TextInput* input, const std::string& text) {
                       std::cout << "Multiline input changed" << std::endl;
                   })
                   .SetSize(300, 100)
                   .SetPosition(50, 470);
    root.AddChild(std::move(multilineInput));

    std::cout << "Multiline input added" << std::endl;

    // 無効化された入力
    auto disabledLabel = std::make_unique<Lithos::TextNode>("Disabled Input:");
    disabledLabel->SetFontSize(16)
                  .SetTextColor(Lithos::Black)
                  .SetPosition(400, 80);
    root.AddChild(std::move(disabledLabel));

    auto disabledInput = std::make_unique<Lithos::TextInput>("Cannot edit");
    disabledInput->SetEnabled(false)
                  .SetText("This is disabled")
                  .SetSize(300, 40)
                  .SetPosition(400, 110);
    root.AddChild(std::move(disabledInput));

    std::cout << "Disabled input added" << std::endl;

    std::cout << "About to show window..." << std::endl;
    window.Show();

    std::cout << "About to run event loop..." << std::endl;
    window.Run();

    std::cout << "Event loop ended" << std::endl;
    return 0;
}

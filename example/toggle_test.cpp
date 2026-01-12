/*
    Minimal ToggleButton Test
    Tests basic functionality to isolate issues
*/

#include <iostream>

#include "Lithos/Components/ToggleButton.hpp"
#include "Lithos/Components/TextNode.hpp"
#include "Lithos/Core/Color.hpp"
#include "Lithos/Core/Window.hpp"

int main() {
    std::cout << "Starting ToggleButton minimal test..." << std::endl;

    Lithos::Window window(800, 600, "Minimal ToggleButton Test");

    auto& root = window.GetRoot();
    root.SetBackgroundColor(Lithos::Color(0.95f, 0.95f, 0.97f));

    std::cout << "Window created" << std::endl;

    // タイトル
    auto titleText = std::make_unique<Lithos::TextNode>("Minimal Test");
    titleText->SetFontSize(24)
              .SetTextColor(Lithos::Black)
              .SetPosition(50, 30);
    root.AddChild(std::move(titleText));

    std::cout << "Title added" << std::endl;

    // 基本チェックボックス1つだけ
    auto checkboxLabel = std::make_unique<Lithos::TextNode>("Test Checkbox");
    checkboxLabel->SetFontSize(16)
                  .SetTextColor(Lithos::Black)
                  .SetPosition(90, 103);
    root.AddChild(std::move(checkboxLabel));

    std::cout << "Label added" << std::endl;

    auto checkbox = std::make_unique<Lithos::ToggleButton>(Lithos::ToggleStyle::Checkbox);
    std::cout << "ToggleButton created" << std::endl;

    checkbox->SetOnChange([](Lithos::ToggleButton* toggle, bool checked) {
        std::cout << "Checkbox clicked: " << (checked ? "checked" : "unchecked") << std::endl;
    });
    std::cout << "OnChange set" << std::endl;

    checkbox->SetPosition(50, 100);
    std::cout << "Position set" << std::endl;

    root.AddChild(std::move(checkbox));
    std::cout << "Checkbox added to root" << std::endl;

    // スイッチ1つだけ
    auto switchLabel = std::make_unique<Lithos::TextNode>("Test Switch");
    switchLabel->SetFontSize(16)
                .SetTextColor(Lithos::Black)
                .SetPosition(120, 153);
    root.AddChild(std::move(switchLabel));

    std::cout << "Switch label added" << std::endl;

    auto toggleSwitch = std::make_unique<Lithos::ToggleButton>(Lithos::ToggleStyle::Switch);
    std::cout << "Switch created" << std::endl;

    toggleSwitch->SetOnChange([](Lithos::ToggleButton* toggle, bool checked) {
        std::cout << "Switch toggled: " << (checked ? "ON" : "OFF") << std::endl;
    });
    std::cout << "Switch OnChange set" << std::endl;

    toggleSwitch->SetPosition(50, 150);
    std::cout << "Switch position set" << std::endl;

    root.AddChild(std::move(toggleSwitch));
    std::cout << "Switch added to root" << std::endl;

    std::cout << "About to show window..." << std::endl;
    window.Show();

    std::cout << "About to run event loop..." << std::endl;
    window.Run();

    std::cout << "Event loop ended" << std::endl;
    return 0;
}
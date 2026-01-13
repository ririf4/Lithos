/*
    Lithos Animation & Transition Demo

    This example demonstrates the CSS-like transition system:
    - Smooth opacity fade-ins and fade-outs
    - Color transitions with different easing functions
    - Size animations with bounce effects
    - Combined property transitions
*/

#include <Lithos/Core/Window.hpp>
#include <Lithos/Layout/Container.hpp>
#include <Lithos/Components/Button.hpp>
#include <Lithos/Components/TextNode.hpp>
#include <Lithos/Core/Animation/Easing.hpp>
#include <Lithos/Core/Animation/AnimatableProperty.hpp>

using namespace Lithos;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create window
    Window window(800, 600, "Lithos Transition Demo");
    auto& root = window.GetRoot();
    root.SetBackgroundColor(Color::LRGB(245, 245, 250));

    // Title
    auto title = std::make_unique<TextNode>("Lithos Transition System Demo");
    title->SetFontSize(32)
         .SetFontWeight(FontWeight::Bold)
         .SetTextColor(Color::LRGB(50, 50, 70))
         .SetPosition(50, 30);
    root.AddChild(std::move(title));

    // Description
    auto desc = std::make_unique<TextNode>("Click buttons to see smooth CSS-like transitions!");
    desc->SetFontSize(16)
        .SetTextColor(Color::LRGB(100, 100, 120))
        .SetPosition(50, 75);
    root.AddChild(std::move(desc));

    // ========== Demo 1: Fade In/Out ==========
    auto fadeTitle = std::make_unique<TextNode>("1. Opacity Fade");
    fadeTitle->SetFontSize(20)
             .SetFontWeight(FontWeight::SemiBold)
             .SetTextColor(Color::LRGB(70, 70, 90))
             .SetPosition(50, 120);
    root.AddChild(std::move(fadeTitle));

    // Fade target box
    auto fadeBox = std::make_unique<Container>();
    fadeBox->SetBackgroundColor(Color::LRGB(100, 150, 255))
           .SetBorderRadius(10)
           .Transition(AnimatableProperty::Opacity, 0.5f, Easing::EaseInOut)
           .SetPosition(50, 150)
           .SetSize(100, 100);
    Container* fadeBoxPtr = fadeBox.get();
    root.AddChild(std::move(fadeBox));

    // Fade In button
    auto fadeInBtn = std::make_unique<Button>("Fade In");
    fadeInBtn->SetNormalColor(Color::LRGB(70, 180, 100))
             .SetHoverColor(Color::LRGB(90, 200, 120))
             .SetPosition(170, 150)
             .SetSize(120, 40);
    fadeInBtn->SetOnClick([fadeBoxPtr](Button* btn) {
        fadeBoxPtr->SetOpacity(1.0f);
    });
    root.AddChild(std::move(fadeInBtn));

    // Fade Out button
    auto fadeOutBtn = std::make_unique<Button>("Fade Out");
    fadeOutBtn->SetNormalColor(Color::LRGB(255, 100, 100))
              .SetHoverColor(Color::LRGB(255, 120, 120))
              .SetPosition(170, 200)
              .SetSize(120, 40);
    fadeOutBtn->SetOnClick([fadeBoxPtr](Button* btn) {
        fadeBoxPtr->SetOpacity(0.0f);
    });
    root.AddChild(std::move(fadeOutBtn));

    // ========== Demo 2: Color Transitions ==========
    auto colorTitle = std::make_unique<TextNode>("2. Color Transition");
    colorTitle->SetFontSize(20)
              .SetFontWeight(FontWeight::SemiBold)
              .SetTextColor(Color::LRGB(70, 70, 90))
              .SetPosition(50, 280);
    root.AddChild(std::move(colorTitle));

    // Color target box
    auto colorBox = std::make_unique<Container>();
    colorBox->SetBackgroundColor(Color::LRGB(255, 100, 100))
            .SetBorderRadius(10)
            .Transition(AnimatableProperty::BackgroundColor, 0.8f, Easing::EaseInOut)
            .SetPosition(50, 310)
            .SetSize(100, 100);
    Container* colorBoxPtr = colorBox.get();
    root.AddChild(std::move(colorBox));

    // Color buttons
    auto redBtn = std::make_unique<Button>("Red");
    redBtn->SetNormalColor(Color::LRGB(255, 100, 100))
          .SetPosition(170, 310)
          .SetSize(80, 35);
    redBtn->SetOnClick([colorBoxPtr](Button* btn) {
        colorBoxPtr->SetBackgroundColor(Color::LRGB(255, 100, 100));
    });
    root.AddChild(std::move(redBtn));

    auto greenBtn = std::make_unique<Button>("Green");
    greenBtn->SetNormalColor(Color::LRGB(100, 255, 100))
            .SetPosition(260, 310)
            .SetSize(80, 35);
    greenBtn->SetOnClick([colorBoxPtr](Button* btn) {
        colorBoxPtr->SetBackgroundColor(Color::LRGB(100, 255, 100));
    });
    root.AddChild(std::move(greenBtn));

    auto blueBtn = std::make_unique<Button>("Blue");
    blueBtn->SetNormalColor(Color::LRGB(100, 150, 255))
           .SetPosition(350, 310)
           .SetSize(80, 35);
    blueBtn->SetOnClick([colorBoxPtr](Button* btn) {
        colorBoxPtr->SetBackgroundColor(Color::LRGB(100, 150, 255));
    });
    root.AddChild(std::move(blueBtn));

    auto purpleBtn = std::make_unique<Button>("Purple");
    purpleBtn->SetNormalColor(Color::LRGB(200, 100, 255))
             .SetPosition(170, 355)
             .SetSize(80, 35);
    purpleBtn->SetOnClick([colorBoxPtr](Button* btn) {
        colorBoxPtr->SetBackgroundColor(Color::LRGB(200, 100, 255));
    });
    root.AddChild(std::move(purpleBtn));

    // ========== Demo 3: Size with Bounce ==========
    auto sizeTitle = std::make_unique<TextNode>("3. Size with Bounce Effect");
    sizeTitle->SetFontSize(20)
             .SetFontWeight(FontWeight::SemiBold)
             .SetTextColor(Color::LRGB(70, 70, 90))
             .SetPosition(430, 120);
    root.AddChild(std::move(sizeTitle));

    // Size target box
    auto sizeBox = std::make_unique<Container>();
    sizeBox->SetBackgroundColor(Color::LRGB(255, 180, 100))
           .SetBorderRadius(15)
           .Transition(AnimatableProperty::Size, 0.6f, Easing::EaseOutBounce)
           .SetPosition(430, 150)
           .SetSize(80, 80);
    Container* sizeBoxPtr = sizeBox.get();
    root.AddChild(std::move(sizeBox));

    // Size buttons
    auto smallBtn = std::make_unique<Button>("Small");
    smallBtn->SetNormalColor(Color::LRGB(150, 150, 200))
            .SetPosition(530, 150)
            .SetSize(100, 35);
    smallBtn->SetOnClick([sizeBoxPtr](Button* btn) {
        sizeBoxPtr->SetSize(80, 80);
    });
    root.AddChild(std::move(smallBtn));

    auto mediumBtn = std::make_unique<Button>("Medium");
    mediumBtn->SetNormalColor(Color::LRGB(150, 150, 200))
             .SetPosition(530, 195)
             .SetSize(100, 35);
    mediumBtn->SetOnClick([sizeBoxPtr](Button* btn) {
        sizeBoxPtr->SetSize(120, 120);
    });
    root.AddChild(std::move(mediumBtn));

    auto largeBtn = std::make_unique<Button>("Large");
    largeBtn->SetNormalColor(Color::LRGB(150, 150, 200))
            .SetPosition(530, 240)
            .SetSize(100, 35);
    largeBtn->SetOnClick([sizeBoxPtr](Button* btn) {
        sizeBoxPtr->SetSize(160, 160);
    });
    root.AddChild(std::move(largeBtn));

    // ========== Demo 4: Multiple Properties ==========
    auto multiTitle = std::make_unique<TextNode>("4. Multiple Properties");
    multiTitle->SetFontSize(20)
              .SetFontWeight(FontWeight::SemiBold)
              .SetTextColor(Color::LRGB(70, 70, 90))
              .SetPosition(430, 310);
    root.AddChild(std::move(multiTitle));

    // Multi-property target box
    auto multiBox = std::make_unique<Container>();
    multiBox->SetBackgroundColor(Color::LRGB(180, 100, 200))
            .SetBorderRadius(10)
            .SetOpacity(1.0f)
            .TransitionAll(0.7f, Easing::EaseInOutBack)
            .SetPosition(430, 340)
            .SetSize(90, 90);
    Container* multiBoxPtr = multiBox.get();
    root.AddChild(std::move(multiBox));

    // Transform button
    auto transformBtn = std::make_unique<Button>("Transform!");
    transformBtn->SetNormalColor(Color::LRGB(255, 150, 80))
                .SetHoverColor(Color::LRGB(255, 170, 100))
                .SetPosition(540, 350)
                .SetSize(140, 50);

    bool isTransformed = false;
    transformBtn->SetOnClick([multiBoxPtr, &isTransformed](Button* btn) {
        if (!isTransformed) {
            multiBoxPtr->SetSize(140, 140);
            multiBoxPtr->SetBackgroundColor(Color::LRGB(100, 200, 255));
            multiBoxPtr->SetBorderRadius(70); // Make it circular
            multiBoxPtr->SetOpacity(0.7f);
        } else {
            multiBoxPtr->SetSize(90, 90);
            multiBoxPtr->SetBackgroundColor(Color::LRGB(180, 100, 200));
            multiBoxPtr->SetBorderRadius(10);
            multiBoxPtr->SetOpacity(1.0f);
        }
        isTransformed = !isTransformed;
    });
    root.AddChild(std::move(transformBtn));

    // Info text at bottom
    auto infoText = std::make_unique<TextNode>(
        "All transitions are powered by the Lithos Animation System - CSS-like, but for C++!"
    );
    infoText->SetFontSize(14)
            .SetTextColor(Color::LRGB(120, 120, 140))
            .SetPosition(50, 520);
    root.AddChild(std::move(infoText));

    // Show window and run
    window.Show();
    window.Run();

    return 0;
}
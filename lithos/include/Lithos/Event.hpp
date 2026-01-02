// Event.hpp
#pragma once

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    enum class EventType {
        // Mouse events
        MouseDown,
        MouseUp,
        MouseMove,

        // Window events
        WindowResize,

        // Keyboard events
        KeyDown,
        KeyUp
    };

    enum class MouseButton {
        Left,
        Right,
        Middle,
        None
    };

    struct LITHOS_API Event {
        EventType type;

        int mouseX = 0;
        int mouseY = 0;
        MouseButton button = MouseButton::None;

        int windowWidth = 0;
        int windowHeight = 0;

        int key = 0;
    };
}
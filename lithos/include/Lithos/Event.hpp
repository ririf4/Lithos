/*
    Copyright 2026 RiriFa

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 */

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
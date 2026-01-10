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

#pragma once

#include <Windows.h>

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    enum class EventType {
        MouseDown,
        MouseUp,
        MouseMove,
        MouseWheel,

        KeyDown,
        KeyUp,
        WindowResize
    };

    enum class MouseButton {
        Left,
        Right,
        Middle,
        None
    };

    enum class CursorType {
        Arrow,      // IDC_ARROW
        Hand,       // IDC_HAND
        IBeam,      // IDC_IBEAM
        Wait,       // IDC_WAIT
        Cross,      // IDC_CROSS
        SizeAll,    // IDC_SIZEALL
        No          // IDC_NO
    };

    inline LPCWSTR GetWindowsCursor(CursorType type) {
        switch (type) {
            case CursorType::Hand:    return IDC_HAND;
            case CursorType::IBeam:   return IDC_IBEAM;
            case CursorType::Wait:    return IDC_WAIT;
            case CursorType::Cross:   return IDC_CROSS;
            case CursorType::SizeAll: return IDC_SIZEALL;
            case CursorType::No:      return IDC_NO;
            case CursorType::Arrow:
            default:                  return IDC_ARROW;
        }
    }

    struct Event {
        EventType type;

        int mouseX = 0;
        int mouseY = 0;
        MouseButton button = MouseButton::None;
        int wheelDelta = 0;

        int windowWidth = 0;
        int windowHeight = 0;

        int key = 0;
    };
}
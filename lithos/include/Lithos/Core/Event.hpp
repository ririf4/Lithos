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
    /**
     * @brief Types of events that can be dispatched to UI nodes
     */
    enum class EventType {
        MouseDown,      ///< Mouse button pressed
        MouseUp,        ///< Mouse button released
        MouseMove,      ///< Mouse cursor moved
        MouseWheel,     ///< Mouse wheel scrolled
        Char,           ///< Character input

        KeyDown,        ///< Keyboard key pressed
        KeyUp,          ///< Keyboard key released
        WindowResize    ///< Window size changed
    };

    /**
     * @brief Mouse button identifiers
     */
    enum class MouseButton {
        Left,    ///< Left mouse button
        Right,   ///< Right mouse button
        Middle,  ///< Middle mouse button (wheel button)
        None     ///< No button (for mouse move events)
    };

    /**
     * @brief Cursor types for visual feedback
     *
     * Maps to Windows standard cursor types (IDC_*)
     */
    enum class CursorType {
        Arrow,      ///< Standard arrow cursor (IDC_ARROW)
        Hand,       ///< Hand pointer cursor (IDC_HAND)
        IBeam,      ///< Text selection cursor (IDC_IBEAM)
        Wait,       ///< Wait/busy cursor (IDC_WAIT)
        Cross,      ///< Crosshair cursor (IDC_CROSS)
        SizeAll,    ///< Move cursor (IDC_SIZEALL)
        No          ///< Prohibited/no action cursor (IDC_NO)
    };

    /**
     * @brief Converts CursorType enum to Windows cursor resource identifier
     * @param type The cursor type to convert
     * @return Windows cursor resource identifier (LPCWSTR)
     */
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

    /**
     * @brief Event data structure containing all event information
     *
     * This structure is passed to Node::OnEvent() and contains all relevant
     * information about the event being dispatched.
     */
    struct Event {
        EventType type;  ///< Type of the event

        // Mouse event data
        int mouseX = 0;          ///< Mouse X coordinate (window-relative)
        int mouseY = 0;          ///< Mouse Y coordinate (window-relative)
        MouseButton button = MouseButton::None;  ///< Mouse button involved
        int wheelDelta = 0;      ///< Wheel scroll delta (120 units per notch)

        // Window event data
        int windowWidth = 0;     ///< Window width (for WindowResize events)
        int windowHeight = 0;    ///< Window height (for WindowResize events)

        // Keyboard event data
        int key = 0;             ///< Virtual key code (for KeyDown/KeyUp events)
        wchar_t character = 0;
    };
}
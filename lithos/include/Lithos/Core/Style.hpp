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
#include "Lithos/Core/Color.hpp"

enum class CursorType {
    Arrow,      ///< Standard arrow cursor (IDC_ARROW)
    Hand,       ///< Hand pointer cursor (IDC_HAND)
    IBeam,      ///< Text selection cursor (IDC_IBEAM)
    Wait,       ///< Wait/busy cursor (IDC_WAIT)
    Cross,      ///< Crosshair cursor (IDC_CROSS)
    SizeAll,    ///< Move cursor (IDC_SIZEALL)
    No          ///< Prohibited/no action cursor (IDC_NO)
};

struct Style {
    float left = 0, top = 0, right = 0, bottom = 0;
    float width = 0, height = 0;

    float opacity = 1.0f;
    Lithos::Color backgroundColor = Lithos::Colors::Transparent;
    Lithos::Color borderColor = Lithos::Colors::Transparent;
    float borderWidth = 0;
    float borderRadius = 0;

    float padding = 0, paddingTop = 0, paddingRight = 0, paddingBottom = 0, paddingLeft = 0;
    float margin = 0, marginTop = 0, marginRight = 0, marginBottom = 0, marginLeft = 0;

    bool shadowEnabled = false;
    float shadowOffsetX = 0, shadowOffsetY = 0;
    float shadowBlur = 0;
    Lithos::Color shadowColor = Lithos::Color(0, 0, 0, 0.5f);
    CursorType cursor = CursorType::Arrow;
};

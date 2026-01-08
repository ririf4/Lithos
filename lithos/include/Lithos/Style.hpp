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
#include <dwrite.h>
#include <string>
#include "Lithos/Color.hpp"

namespace Lithos {
    enum class Display {
        Block,
        Flex,
        Grid
    };

    enum class FlexDirection {
        Row,
        Column
    };

    enum class JustifyContent {
        Start,
        End,
        Center,
        SpaceBetween,
        SpaceAround
    };

    enum class AlignItems {
        Start,
        End,
        Center,
        Stretch
    };

    enum class Position {
        Relative,
        Absolute
    };

    enum class FontWeight {
        Thin = DWRITE_FONT_WEIGHT_THIN,
        Light = DWRITE_FONT_WEIGHT_LIGHT,
        Normal = DWRITE_FONT_WEIGHT_NORMAL,
        Medium = DWRITE_FONT_WEIGHT_MEDIUM,
        Bold = DWRITE_FONT_WEIGHT_BOLD,
        ExtraBold = DWRITE_FONT_WEIGHT_EXTRA_BOLD,
        Black = DWRITE_FONT_WEIGHT_BLACK
    };

    enum class FontStyle {
        Normal = DWRITE_FONT_STYLE_NORMAL,
        Italic = DWRITE_FONT_STYLE_ITALIC,
        Oblique = DWRITE_FONT_STYLE_OBLIQUE
    };

    enum class TextAlign {
        Left,
        Center,
        Right,
        Justify
    };

    struct Style {
        Display display = Display::Block;
        FlexDirection flexDirection = FlexDirection::Row;
        JustifyContent justifyContent = JustifyContent::Start;
        AlignItems alignItems = AlignItems::Stretch;
        Position position = Position::Relative;

        float width = 0;    // 0 = auto
        float height = 0;   // 0 = auto
        float minWidth = 0;
        float minHeight = 0;
        float maxWidth = 0;
        float maxHeight = 0;

        float padding = 0;
        float paddingTop = 0;
        float paddingRight = 0;
        float paddingBottom = 0;
        float paddingLeft = 0;

        float margin = 0;
        float marginTop = 0;
        float marginRight = 0;
        float marginBottom = 0;
        float marginLeft = 0;

        float gap = 0;

        float left = 0;
        float top = 0;
        float right = 0;
        float bottom = 0;

        Color backgroundColor = Transparent;
        Color borderColor = Transparent;
        float borderWidth = 0;
        float borderRadius = 0;
        float opacity = 1.0f;

        // Shadow
        bool shadowEnabled = false;
        float shadowOffsetX = 0;
        float shadowOffsetY = 0;
        float shadowBlur = 0;
        Color shadowColor = Color(0, 0, 0, 0.5f);

        Color textColor = Black;
        float fontSize = 16;
        std::string fontFamily = "Arial";
        FontWeight fontWeight = FontWeight::Normal;
        TextAlign textAlign = TextAlign::Left;
        FontStyle fontStyle = FontStyle::Normal;

        float flex = 0;
    };
}
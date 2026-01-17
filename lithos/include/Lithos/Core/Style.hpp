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
#include "Lithos/Core/Color.hpp"

namespace Lithos {
    /**
     * @brief Display type for layout calculation
     */
    enum class Display {
        Block,  ///< Block layout (default)
        Flex,   ///< Flexbox layout
        Grid    ///< Grid layout (not yet implemented)
    };

    /**
     * @brief Flex container direction
     */
    enum class FlexDirection {
        Row,    ///< Arrange children horizontally
        Column  ///< Arrange children vertically
    };

    /**
     * @brief Alignment of children along the main axis in flex layout
     */
    enum class JustifyContent {
        Start,         ///< Align to start of container
        End,           ///< Align to end of container
        Center,        ///< Center in container
        SpaceBetween,  ///< Distribute with space between items
        SpaceAround    ///< Distribute with space around items
    };

    /**
     * @brief Alignment of children along the cross axis in flex layout
     */
    enum class AlignItems {
        Start,   ///< Align to start of cross axis
        End,     ///< Align to end of cross axis
        Center,  ///< Center on cross axis
        Stretch  ///< Stretch to fill cross axis
    };

    /**
     * @brief Flex wrap behavior for multi-line layouts
     */
    enum class FlexWrap {
        NoWrap,       ///< Single line (default behavior)
        Wrap,         ///< Wrap to multiple lines
        WrapReverse   ///< Wrap to multiple lines in reverse order
    };

    /**
     * @brief Alignment of lines along the cross axis in multi-line flex layout
     */
    enum class AlignContent {
        Start,         ///< Pack lines to start of cross axis
        End,           ///< Pack lines to end of cross axis
        Center,        ///< Pack lines to center of cross axis
        SpaceBetween,  ///< Distribute lines with space between
        SpaceAround,   ///< Distribute lines with space around
        Stretch        ///< Stretch lines to fill container
    };

    /**
     * @brief Positioning type for nodes
     */
    enum class Position {
        Relative,  ///< Position relative to parent
        Absolute   ///< Position absolute (ignore normal flow)
    };

    /**
     * @brief Font weight values mapped to DirectWrite font weights
     */
    enum class FontWeight {
        Thin = DWRITE_FONT_WEIGHT_THIN,        ///< Thin (100)
        Light = DWRITE_FONT_WEIGHT_LIGHT,      ///< Light (300)
        Normal = DWRITE_FONT_WEIGHT_NORMAL,    ///< Normal (400)
        Medium = DWRITE_FONT_WEIGHT_MEDIUM,    ///< Medium (500)
        SemiBold = DWRITE_FONT_WEIGHT_SEMI_BOLD,
        Bold = DWRITE_FONT_WEIGHT_BOLD,        ///< Bold (700)
        ExtraBold = DWRITE_FONT_WEIGHT_EXTRA_BOLD,  ///< Extra Bold (800)
        Black = DWRITE_FONT_WEIGHT_BLACK       ///< Black (900)
    };

    /**
     * @brief Font style values mapped to DirectWrite font styles
     */
    enum class FontStyle {
        Normal = DWRITE_FONT_STYLE_NORMAL,   ///< Normal upright text
        Italic = DWRITE_FONT_STYLE_ITALIC,   ///< Italic text
        Oblique = DWRITE_FONT_STYLE_OBLIQUE  ///< Oblique (slanted) text
    };

    /**
     * @brief Text alignment options
     */
    enum class TextAlign {
        Left,     ///< Align text to the left
        Center,   ///< Center text
        Right,    ///< Align text to the right
        Justify   ///< Justify text
    };

    /**
     * @brief Comprehensive style properties for UI nodes
     *
     * Contains all styling properties including layout, spacing, colors, borders,
     * shadows, and typography. Used by Node and its derived classes.
     */
    struct Style {
        // ========== Layout Properties ==========
        Display display = Display::Block;                    ///< Display/layout type
        FlexDirection flexDirection = FlexDirection::Row;    ///< Flex direction (Row/Column)
        JustifyContent justifyContent = JustifyContent::Start; ///< Main axis alignment
        AlignItems alignItems = AlignItems::Stretch;         ///< Cross axis alignment
        Position position = Position::Relative;              ///< Positioning type

        // ========== Dimensions ==========
        float width = 0;     ///< Width (0 = auto)
        float height = 0;    ///< Height (0 = auto)
        float minWidth = 0;  ///< Minimum width constraint
        float minHeight = 0; ///< Minimum height constraint
        float maxWidth = 0;  ///< Maximum width constraint (0 = none)
        float maxHeight = 0; ///< Maximum height constraint (0 = none)

        // ========== Padding (inner spacing) ==========
        float padding = 0;        ///< Uniform padding (overridden by specific sides)
        float paddingTop = 0;     ///< Top padding
        float paddingRight = 0;   ///< Right padding
        float paddingBottom = 0;  ///< Bottom padding
        float paddingLeft = 0;    ///< Left padding

        // ========== Margin (outer spacing) ==========
        float margin = 0;        ///< Uniform margin (overridden by specific sides)
        float marginTop = 0;     ///< Top margin
        float marginRight = 0;   ///< Right margin
        float marginBottom = 0;  ///< Bottom margin
        float marginLeft = 0;    ///< Left margin

        // ========== Flexbox Properties ==========
        float gap = 0;               ///< Gap between flex children
        float flex = 0;              ///< Flex grow factor (not fully implemented)
        float flexShrink = 0;        ///< Flex shrink factor for item shrinking
        FlexWrap flexWrap = FlexWrap::NoWrap;           ///< Flex wrap behavior
        AlignContent alignContent = AlignContent::Start; ///< Multi-line cross-axis alignment

        // ========== Positioning Offsets ==========
        float left = 0;    ///< Left offset (for absolute/relative positioning)
        float top = 0;     ///< Top offset (for absolute/relative positioning)
        float right = 0;   ///< Right offset (for right-edge positioning)
        float bottom = 0;  ///< Bottom offset (for bottom-edge positioning)

        // ========== Visual Properties ==========
        Color backgroundColor = Colors::Transparent;  ///< Background fill color
        Color borderColor = Colors::Transparent;      ///< Border stroke color
        float borderWidth = 0;                        ///< Border thickness
        float borderRadius = 0;                       ///< Corner radius for rounded borders
        float opacity = 1.0f;                         ///< Opacity (0.0 = transparent, 1.0 = opaque)

        // ========== Shadow Properties ==========
        bool shadowEnabled = false;              ///< Enable/disable shadow rendering
        float shadowOffsetX = 0;                 ///< Shadow horizontal offset
        float shadowOffsetY = 0;                 ///< Shadow vertical offset
        float shadowBlur = 0;                    ///< Shadow blur radius
        Color shadowColor = Color(0, 0, 0, 0.5f); ///< Shadow color

        // ========== Typography Properties ==========
        Color textColor = Colors::Black;     ///< Text color
        float fontSize = 16;                 ///< Font size in points
        std::string fontFamily = "Arial";    ///< Font family name
        FontWeight fontWeight = FontWeight::Normal;  ///< Font weight
        TextAlign textAlign = TextAlign::Left;       ///< Text alignment
        FontStyle fontStyle = FontStyle::Normal;     ///< Font style (Normal/Italic/Oblique)
    };
}
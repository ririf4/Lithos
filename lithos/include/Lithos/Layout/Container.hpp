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
#include "Lithos/Core/Node.hpp"
#include "Lithos/Core/Style.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class LITHOS_API Container : public Node {
        public:
            Container();
            ~Container() override = default;

            Container& SetDisplay(Display display);
            Container& SetPositionType(Position pos);
            Container& SetFlexDirection(FlexDirection direction);
            Container& SetJustifyContent(JustifyContent justify);
            Container& SetAlignItems(AlignItems align);
            Container& SetGap(float gap);

            Container& AddContainer();

            void Layout() override;
            void Draw(ID2D1DeviceContext* rt) override;
    };
}
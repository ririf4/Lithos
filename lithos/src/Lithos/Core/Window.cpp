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

#include "Lithos/Core/Window.hpp"

namespace Lithos {
    namespace {
        std::wstring ToWString(const std::string& utf8) {
            if (utf8.empty()) return L"";

            const int size = MultiByteToWideChar(
                CP_UTF8,
                0,
                utf8.data(),
                static_cast<int>(utf8.size()),
                nullptr,
                0
            );

            std::wstring result(size, 0);
            MultiByteToWideChar(
                CP_UTF8,
                0,
                utf8.data(),
                static_cast<int>(utf8.size()),
                result.data(),
                size
            );

            return result;
        }
    }

    struct Window::Impl {
        HWND hwnd;

        // Direct2D 1.1
        ID2D1Factory1* pD2DFactory;
        ID2D1Device* pD2DDevice;
        ID2D1DeviceContext* pDeviceContext;

        // Direct3D 11
        ID3D11Device* pD3DDevice;
        ID3D11DeviceContext* pD3DContext;
        IDXGISwapChain1* pSwapChain;
        ID2D1Bitmap1* pTargetBitmap;
    };
}

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

// Window.cpp
#include "Lithos/Window.hpp"
#include "Lithos/Event.hpp"
#include "Lithos/Panel.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <iostream>
#include <vector>
#include <windows.h>
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"

namespace Lithos {
    namespace {
        std::wstring ToWString(const std::string& utf8) {
            if (utf8.empty()) return L"";
            const int size_needed = MultiByteToWideChar(
                CP_UTF8,
                0,
                utf8.data(),
                static_cast<int>(utf8.size()),
                nullptr,
                0
            );
            std::wstring wstrTo(size_needed, 0);
            MultiByteToWideChar(
                CP_UTF8,
                0,
                utf8.data(),
                static_cast<int>(utf8.size()),
                wstrTo.data(),
                size_needed
            );
            return wstrTo;
        }
    }

    struct Window::Impl {
        HWND hwnd = nullptr;
        sk_sp<SkSurface> surface;
        int width{}, height{};
        std::vector<std::unique_ptr<Panel>> panels;

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
            Impl* pImpl = nullptr;
            if (message == WM_NCCREATE) {
                const auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
                pImpl = static_cast<Impl*>(cs->lpCreateParams);
                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pImpl));
            }
            else { pImpl = reinterpret_cast<Impl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA)); }

            if (pImpl) {
                switch (message) {
                    case WM_PAINT: {
                        pImpl->OnPaint();
                        return 0;
                    }

                    case WM_LBUTTONDOWN: {
                        Event evt;
                        evt.type = EventType::MouseDown;
                        evt.button = MouseButton::Left;
                        evt.mouseX = LOWORD(lParam);
                        evt.mouseY = HIWORD(lParam);

                        for (const auto& panel : pImpl->panels) { if (panel->HandleEvent(evt)) break; }
                        return 0;
                    }

                    case WM_LBUTTONUP: {
                        Event evt;
                        evt.type = EventType::MouseUp;
                        evt.button = MouseButton::Left;
                        evt.mouseX = LOWORD(lParam);
                        evt.mouseY = HIWORD(lParam);

                        for (const auto& panel : pImpl->panels) { if (panel->HandleEvent(evt)) break; }
                        return 0;
                    }

                    case WM_RBUTTONDOWN: {
                        Event evt;
                        evt.type = EventType::MouseDown;
                        evt.button = MouseButton::Right;
                        evt.mouseX = LOWORD(lParam);
                        evt.mouseY = HIWORD(lParam);

                        for (const auto& panel : pImpl->panels) { if (panel->HandleEvent(evt)) break; }
                        return 0;
                    }

                    case WM_RBUTTONUP: {
                        Event evt;
                        evt.type = EventType::MouseUp;
                        evt.button = MouseButton::Right;
                        evt.mouseX = LOWORD(lParam);
                        evt.mouseY = HIWORD(lParam);

                        for (const auto& panel : pImpl->panels) { if (panel->HandleEvent(evt)) break; }
                        return 0;
                    }

                    case WM_MBUTTONDOWN: {
                        Event evt;
                        evt.type = EventType::MouseDown;
                        evt.button = MouseButton::Middle;
                        evt.mouseX = LOWORD(lParam);
                        evt.mouseY = HIWORD(lParam);

                        for (const auto& panel : pImpl->panels) { if (panel->HandleEvent(evt)) break; }
                        return 0;
                    }

                    case WM_MBUTTONUP: {
                        Event evt;
                        evt.type = EventType::MouseUp;
                        evt.button = MouseButton::Middle;
                        evt.mouseX = LOWORD(lParam);
                        evt.mouseY = HIWORD(lParam);

                        for (const auto& panel : pImpl->panels) { if (panel->HandleEvent(evt)) break; }
                        return 0;
                    }

                    case WM_SIZE: {
                        pImpl->width = LOWORD(lParam);
                        pImpl->height = HIWORD(lParam);

                        Event evt;
                        evt.type = EventType::WindowResize;
                        evt.windowWidth = pImpl->width;
                        evt.windowHeight = pImpl->height;

                        for (const auto& panel : pImpl->panels) { panel->HandleEvent(evt); }

                        const SkImageInfo info = SkImageInfo::MakeN32Premul(pImpl->width, pImpl->height);
                        pImpl->surface = SkSurfaces::Raster(info);

                        InvalidateRect(hWnd, nullptr, FALSE);
                        return 0;
                    }

                    case WM_DESTROY: {
                        PostQuitMessage(0);
                        return 0;
                    }
                }
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        void OnPaint() const {
            SkCanvas* canvas = surface->getCanvas();
            canvas->clear(SK_ColorWHITE);

            for (const auto& panel : panels) { panel->Draw(canvas); }

            PAINTSTRUCT ps;
            const HDC hdc = BeginPaint(hwnd, &ps);

            SkPixmap pixmap;
            if (surface->peekPixels(&pixmap)) {
                BITMAPINFO bmi = {};
                bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                bmi.bmiHeader.biWidth = pixmap.width();
                bmi.bmiHeader.biHeight = -pixmap.height();
                bmi.bmiHeader.biPlanes = 1;
                bmi.bmiHeader.biBitCount = 32;
                bmi.bmiHeader.biCompression = BI_RGB;

                StretchDIBits(
                    hdc,
                    0, 0,
                    pixmap.width(), pixmap.height(),
                    0, 0,
                    pixmap.width(), pixmap.height(),
                    pixmap.addr(),
                    &bmi,
                    DIB_RGB_COLORS,
                    SRCCOPY
                );
            }
            EndPaint(hwnd, &ps);
        }
    };

    Window::Window(const int width, const int height, const std::string& title)
        : pimpl(std::make_unique<Impl>()) {

        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        pimpl->width = width;
        pimpl->height = height;

        const std::wstring wTitle = ToWString(title);

        WNDCLASS wc = {};
        wc.lpfnWndProc = Impl::WndProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = L"LithosWindowClass";
        RegisterClass(&wc);

        pimpl->hwnd = CreateWindowEx(
            0,
            L"LithosWindowClass",
            wTitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            width,
            height,
            nullptr,
            nullptr,
            wc.hInstance,
            pimpl.get()
        );

        const SkImageInfo info = SkImageInfo::MakeN32Premul(width, height);
        pimpl->surface = SkSurfaces::Raster(info);
    }

    void Window::AddPanel(std::unique_ptr<Panel> panel) const { pimpl->panels.push_back(std::move(panel)); }

    void Window::Show() const { ShowWindow(pimpl->hwnd, SW_SHOW); }

    void Window::RunMessageLoop() const {
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    Window::~Window() = default;
}

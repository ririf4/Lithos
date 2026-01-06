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

#include "Lithos/Window.hpp"
#include <d2d1.h>
#include <windows.h>
#include "Lithos/Event.hpp"
#include "Lithos/Node.hpp"
#pragma comment(lib, "d2d1.lib")

namespace Lithos {
    namespace {
        std::wstring ToWString(const std::string& utf8) {
            if (utf8.empty()) return L"";

            const int size = MultiByteToWideChar(
                CP_UTF8, 0,
                utf8.data(), static_cast<int>(utf8.size()),
                nullptr, 0
            );

            std::wstring result(size, 0);
            MultiByteToWideChar(
                CP_UTF8, 0,
                utf8.data(), static_cast<int>(utf8.size()),
                result.data(), size
            );

            return result;
        }
    }

    struct Window::Impl {
        HWND hwnd;
        ID2D1Factory* pD2DFactory;
        ID2D1HwndRenderTarget* pRenderTarget;
        int width;
        int height;

        std::unique_ptr<Node> rootNode;

        Impl()
            : hwnd(nullptr),
              pD2DFactory(nullptr),
              pRenderTarget(nullptr),
              width(0),
              height(0),
              rootNode(std::make_unique<Node>()) {}

        ~Impl() {
            SafeRelease(pRenderTarget);
            SafeRelease(pD2DFactory);
        }

        template<typename T>
        void SafeRelease(T*& ptr) {
            if (ptr) {
                ptr->Release();
                ptr = nullptr;
            }
        }

        void CreateDeviceResources() {
            if (pRenderTarget) return;

            RECT rc;
            GetClientRect(hwnd, &rc);

            const D2D1_SIZE_U size = D2D1::SizeU(
                rc.right - rc.left,
                rc.bottom - rc.top
            );

            pD2DFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(hwnd, size),
                &pRenderTarget
            );
        }

        void OnResize(const int newWidth, const int newHeight) {
            width = newWidth;
            height = newHeight;

            if (pRenderTarget) {
                pRenderTarget->Resize(D2D1::SizeU(width, height));

                rootNode->SetSize(static_cast<float>(width), static_cast<float>(height));
                rootNode->Layout();

                InvalidateRect(hwnd, nullptr, FALSE);
            }

            Event evt;
            evt.type = EventType::WindowResize;
            evt.windowWidth = width;
            evt.windowHeight = height;
            rootNode->OnEvent(evt);
        }

        void OnPaint() {
            if (!pRenderTarget) return;

            pRenderTarget->BeginDraw();
            pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

            rootNode->Draw(pRenderTarget);

            const HRESULT hr = pRenderTarget->EndDraw();

            if (hr == D2DERR_RECREATE_TARGET) {
                SafeRelease(pRenderTarget);
                CreateDeviceResources();
            }
        }

        void OnMouseEvent(UINT msg, WPARAM wParam, LPARAM lParam) {
            Event evt;
            evt.mouseX = LOWORD(lParam);
            evt.mouseY = HIWORD(lParam);

            switch (msg) {
                case WM_LBUTTONDOWN:
                    evt.type = EventType::MouseDown;
                    evt.button = MouseButton::Left;
                    break;
                case WM_LBUTTONUP:
                    evt.type = EventType::MouseUp;
                    evt.button = MouseButton::Left;
                    break;
                case WM_RBUTTONDOWN:
                    evt.type = EventType::MouseDown;
                    evt.button = MouseButton::Right;
                    break;
                case WM_RBUTTONUP:
                    evt.type = EventType::MouseUp;
                    evt.button = MouseButton::Right;
                    break;
                case WM_MBUTTONDOWN:
                    evt.type = EventType::MouseDown;
                    evt.button = MouseButton::Middle;
                    break;
                case WM_MBUTTONUP:
                    evt.type = EventType::MouseUp;
                    evt.button = MouseButton::Middle;
                    break;
                case WM_MOUSEMOVE:
                    evt.type = EventType::MouseMove;
                    break;
                default:
                    return;
            }

            rootNode->OnEvent(evt);
            InvalidateRect(hwnd, nullptr, FALSE);
        }

        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            Impl* pImpl = nullptr;

            if (msg == WM_NCCREATE) {
                const auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
                pImpl = static_cast<Impl*>(cs->lpCreateParams);
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pImpl));
            } else {
                pImpl = reinterpret_cast<Impl*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            }

            if (!pImpl) {
                return DefWindowProc(hwnd, msg, wParam, lParam);
            }

            switch (msg) {
                case WM_PAINT:
                    pImpl->OnPaint();
                    ValidateRect(hwnd, nullptr);
                    return 0;

                case WM_SIZE:
                    pImpl->OnResize(LOWORD(lParam), HIWORD(lParam));
                    return 0;

                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                case WM_MOUSEMOVE:
                    pImpl->OnMouseEvent(msg, wParam, lParam);
                    return 0;

                case WM_DESTROY:
                    PostQuitMessage(0);
                    return 0;

                default:
                    return DefWindowProc(hwnd, msg, wParam, lParam);
            }
        }
    };

    Window::Window(const int width, const int height, const std::string& title)
        : pimpl(std::make_unique<Impl>()) {

        pimpl->width = width;
        pimpl->height = height;

        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &pimpl->pD2DFactory
        );

        const std::wstring wTitle = ToWString(title);
        const HINSTANCE hInstance = GetModuleHandle(nullptr);

        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = Impl::WndProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"LithosWindowClass";
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        RegisterClassEx(&wc);

        pimpl->hwnd = CreateWindowEx(
            0,
            L"LithosWindowClass",
            wTitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            width, height,
            nullptr,
            nullptr,
            hInstance,
            pimpl.get()
        );

        pimpl->CreateDeviceResources();

        pimpl->rootNode->SetSize(static_cast<float>(width), static_cast<float>(height));
        pimpl->rootNode->SetPosition(0, 0);
        pimpl->rootNode->Layout();
    }

    Window::~Window() = default;

    void Window::Show() const {
        ShowWindow(pimpl->hwnd, SW_SHOW);
        UpdateWindow(pimpl->hwnd);
    }

    void Window::Run() {
        MSG msg = {};
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    Node& Window::GetRoot() {
        return *pimpl->rootNode;
    }

    Container& Window::AddContainer() {
        auto container = std::make_unique<Container>();
        Container& ref = *container;
        pimpl->rootNode->AddChild(std::move(container));
        return ref;
    }

}
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

#include "Lithos/Core/Element.hpp"
#include "Lithos/Core/Event.hpp"

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

        int width, height;
        std::unique_ptr<Element> rootElement;

        Impl()
            : hwnd(nullptr),
              pD2DFactory(nullptr),
              pD2DDevice(nullptr),
              pDeviceContext(nullptr),
              pD3DDevice(nullptr),
              pD3DContext(nullptr),
              pSwapChain(nullptr),
              pTargetBitmap(nullptr),
              width(0),
              height(0),
              rootElement(std::make_unique<Element>()) {}

        ~Impl() {
            SafeRelease(pTargetBitmap);
            SafeRelease(pSwapChain);
            SafeRelease(pDeviceContext);
            SafeRelease(pD2DDevice);
            SafeRelease(pD3DContext);
            SafeRelease(pD3DDevice);
            SafeRelease(pD2DFactory);
        }

        template <typename T>
        void SafeRelease(T*& ptr) {
            if (ptr) {
                ptr->Release();
                ptr = nullptr;
            }
        }

        void CreateDeviceResources() {
            if (pDeviceContext) return;

            D3D_FEATURE_LEVEL featureLevels[] = {
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0
            };

            UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
            #ifdef _DEBUG
            creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
            #endif

            D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
                creationFlags,
                featureLevels,
                ARRAYSIZE(featureLevels),
                D3D11_SDK_VERSION,
                &pD3DDevice,
                nullptr,
                &pD3DContext
            );

            IDXGIDevice* dxgiDevice = nullptr;
            pD3DDevice->QueryInterface(&dxgiDevice);

            pD2DFactory->CreateDevice(dxgiDevice, &pD2DDevice);
            pD2DDevice->CreateDeviceContext(
                D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
                &pDeviceContext
            );

            IDXGIAdapter* dxgiAdapter = nullptr;
            dxgiDevice->GetAdapter(&dxgiAdapter);

            IDXGIFactory2* dxgiFactory = nullptr;
            dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
            swapChainDesc.Width = width;
            swapChainDesc.Height = height;
            swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = 2;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

            dxgiFactory->CreateSwapChainForHwnd(
                pD3DDevice,
                hwnd,
                &swapChainDesc,
                nullptr,
                nullptr,
                &pSwapChain
            );

            SafeRelease(dxgiFactory);
            SafeRelease(dxgiAdapter);
            SafeRelease(dxgiDevice);

            CreateBitmapFromSwapChain();
        }

        void CreateBitmapFromSwapChain() {
            SafeRelease(pTargetBitmap);

            IDXGISurface* dxgiBackBuffer = nullptr;
            pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

            const UINT dpi = GetDpiForWindow(hwnd);
            const auto dpiF = static_cast<float>(dpi);

            const D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
                D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
                dpiF,
                dpiF
            );

            pDeviceContext->CreateBitmapFromDxgiSurface(
                dxgiBackBuffer,
                &bitmapProperties,
                &pTargetBitmap
            );

            pDeviceContext->SetTarget(pTargetBitmap);

            SafeRelease(dxgiBackBuffer);
        }

        void OnPaint() const {
            if (!pDeviceContext) return;

            //TODO
        }

        void OnResize(const int newWidth, const int newHeight) {
            //TODO
        }

        void OnMouseEvent(UINT msg, WPARAM wParam, LPARAM lParam) {
            MouseEvent evt;
            evt.x = LOWORD(lParam);
            evt.y = HIWORD(lParam);

            switch (msg) {
                case WM_LBUTTONDOWN:
                    evt.type = MouseEventType::MouseDown;
                    evt.button = Left;
                    break;
                case WM_LBUTTONUP:
                    evt.type = MouseEventType::MouseUp;
                    evt.button = Left;
                    break;
                case WM_RBUTTONDOWN:
                    evt.type = MouseEventType::MouseDown;
                    evt.button = Right;
                    break;
                case WM_RBUTTONUP:
                    evt.type = MouseEventType::MouseUp;
                    evt.button = Right;
                    break;
                case WM_MBUTTONDOWN:
                    evt.type = MouseEventType::MouseDown;
                    evt.button = Middle;
                    break;
                case WM_MBUTTONUP:
                    evt.type = MouseEventType::MouseUp;
                    evt.button = Middle;
                    break;
                case WM_MOUSEMOVE:
                    evt.type = MouseEventType::MouseMove;
                    break;
                default:
                    return;
            }

            bool handled = rootElement->OnMouseEvent(evt);

            if (!handled && evt.type == MouseEventType::MouseDown) {
                //TODO
                // if (focusedNode) {
                //     focusedNode->OnLostFocus();
                //     focusedNode = nullptr;
                // }
            }

            InvalidateRect(hwnd, nullptr, FALSE);
        }

        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            Impl* pImpl = nullptr;

            if (msg == WM_NCCREATE) {
                const auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
                pImpl = static_cast<Impl*>(cs->lpCreateParams);
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pImpl));
            }
            else { pImpl = reinterpret_cast<Impl*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); }

            if (!pImpl) { return DefWindowProc(hwnd, msg, wParam, lParam); }

            switch (msg) {
                case WM_PAINT: {
                    pImpl->OnPaint();
                    ValidateRect(hwnd, nullptr);
                    return 0;
                }

                case WM_SIZE: {
                    pImpl->OnResize(LOWORD(lParam), HIWORD(lParam));
                    return 0;
                }

                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                case WM_MOUSEMOVE:
                    pImpl->OnMouseEvent(msg, wParam, lParam);
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
            __uuidof(ID2D1Factory1),
            reinterpret_cast<void**>(&pimpl->pD2DFactory)
        );

        const std::wstring wTitle = ToWString(title);
        HINSTANCE hInstance = GetModuleHandle(nullptr);

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
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            width,
            height,
            nullptr,
            nullptr,
            hInstance,
            pimpl.get()
        );

        pimpl->CreateDeviceResources();
    }

    Window::~Window() = default;
}

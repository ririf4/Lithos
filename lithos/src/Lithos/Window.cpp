#include "Lithos/Window.hpp"
#include "Lithos/Layer.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <vector>
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
        std::vector<std::unique_ptr<Layer>> layers;  // 追加

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
            Impl* pImpl = nullptr;
            if (message == WM_NCCREATE) {
                const auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
                pImpl = static_cast<Impl*>(cs->lpCreateParams);
                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pImpl));
            }
            else {
                pImpl = reinterpret_cast<Impl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            }

            if (pImpl) {
                switch (message) {
                case WM_PAINT: {
                    pImpl->OnPaint();
                    return 0;
                }
                case WM_DESTROY:
                    PostQuitMessage(0);
                    return 0;
                }
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        void OnPaint() const {
            SkCanvas* canvas = surface->getCanvas();
            canvas->clear(SK_ColorWHITE);

            for (const auto& layer : layers) {
                layer->Draw(canvas);
            }

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            SkPixmap pixmap;
            if (surface->peekPixels(&pixmap)) {
                BITMAPINFO bmi = {};
                bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                bmi.bmiHeader.biWidth = pixmap.width();
                bmi.bmiHeader.biHeight = -pixmap.height();
                bmi.bmiHeader.biPlanes = 1;
                bmi.bmiHeader.biBitCount = 32;
                bmi.bmiHeader.biCompression = BI_RGB;

                StretchDIBits(hdc, 0, 0, pixmap.width(), pixmap.height(),
                              0, 0, pixmap.width(), pixmap.height(),
                              pixmap.addr(), &bmi, DIB_RGB_COLORS, SRCCOPY);
            }
            EndPaint(hwnd, &ps);
        }
    };

    Window::Window(const int width, const int height, const std::string& title)
        : pimpl(std::make_unique<Impl>()) {
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

    void Window::AddLayer(std::unique_ptr<Layer> layer) const {
        pimpl->layers.push_back(std::move(layer));
    }

    void Window::Show() const { ShowWindow(pimpl->hwnd, SW_SHOW); }

    void Window::RunMessageLoop() const {
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    Window::~Window() = default;
} // namespace Lithos
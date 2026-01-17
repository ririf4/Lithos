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
#include "Lithos/PCH.hpp"
#include <chrono>
#include <thread>
#include <algorithm>
#include <cmath>
#include "Lithos/Layout/Container.hpp"
#include "Lithos/Core/Event.hpp"
#include "Lithos/Core/Node.hpp"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

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

        int width;
        int height;
        std::unique_ptr<Node> rootNode;
        Node* focusedNode;

        // Animation support
        std::chrono::steady_clock::time_point lastFrameTime;
        bool needsAnimation;

        // Rendering configuration and state
        Window::RenderConfig renderConfig;
        Window::RenderStats renderStats;

        // Dirty region management
        struct DirtyRegion {
            Rect rect;
            int priority;
        };

        struct DirtyRegionManager {
            std::vector<DirtyRegion> dirtyRegions;
            std::vector<DirtyRegion> deferredRegions;

            void AddDirtyRegion(const Rect& rect, int priority) {
                dirtyRegions.push_back({rect, priority});
            }

            void Clear() {
                dirtyRegions.clear();
            }

            void MergeRegions(const Window::RenderConfig& config);
            float CalculateGap(const Rect& a, const Rect& b) const;
        };

        DirtyRegionManager dirtyRegionManager;

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
              rootNode(std::make_unique<Node>()),
              focusedNode(nullptr),
              needsAnimation(false) {}

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

            UINT dpi = GetDpiForWindow(hwnd);
            float dpiF = static_cast<float>(dpi);

            D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
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

        void OnResize(const int newWidth, const int newHeight) {
            width = newWidth;
            height = newHeight;

            if (pSwapChain) {
                pDeviceContext->SetTarget(nullptr);
                SafeRelease(pTargetBitmap);

                pSwapChain->ResizeBuffers(
                    0,
                    width,
                    height,
                    DXGI_FORMAT_UNKNOWN,
                    0
                );

                CreateBitmapFromSwapChain();

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

        void RenderSubtree(Node* node, const Rect& clipRegion) {
            if (!node || !node->IsVisible()) return;

            // Early rejection: bounds check
            if (!node->GetBounds().Intersects(clipRegion)) {
                return;  // Skip entire subtree
            }

            node->Draw(pDeviceContext);

            for (const auto& child : node->GetChildren()) {
                RenderSubtree(child.get(), clipRegion);
            }
        }

        void OnPaint() {
            if (!pDeviceContext) return;

            auto frameStartTime = std::chrono::steady_clock::now();

            // Merge deferred regions from previous frame
            for (const auto& deferred : dirtyRegionManager.deferredRegions) {
                dirtyRegionManager.AddDirtyRegion(deferred.rect, deferred.priority);
            }
            dirtyRegionManager.deferredRegions.clear();

            // Statistics: count regions before merging
            renderStats.dirtyRegionCount = static_cast<int>(dirtyRegionManager.dirtyRegions.size());

            // Merge dirty regions if enabled
            if (renderConfig.enableRegionMerging && !dirtyRegionManager.dirtyRegions.empty()) {
                dirtyRegionManager.MergeRegions(renderConfig);
            }

            renderStats.mergedRegionCount = static_cast<int>(dirtyRegionManager.dirtyRegions.size());

            pDeviceContext->BeginDraw();

            if (dirtyRegionManager.dirtyRegions.empty() || !renderConfig.enableDifferentialRendering) {
                // Full redraw (fallback)
                pDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
                rootNode->Draw(pDeviceContext);
            } else {
                // Differential rendering
                // Sort by priority (descending: high priority first)
                std::sort(dirtyRegionManager.dirtyRegions.begin(),
                         dirtyRegionManager.dirtyRegions.end(),
                         [](const DirtyRegion& a, const DirtyRegion& b) {
                             return a.priority > b.priority;
                         });

                renderStats.skippedLowPriorityCount = 0;

                for (const auto& region : dirtyRegionManager.dirtyRegions) {
                    // Check frame time budget
                    auto elapsed = std::chrono::steady_clock::now() - frameStartTime;
                    auto elapsedMs = std::chrono::duration<float, std::milli>(elapsed).count();

                    if (elapsedMs > static_cast<float>(renderConfig.frameTimeBudgetMs) &&
                        region.priority < static_cast<int>(RenderPriority::High)) {
                        // Skip low-priority regions if budget exceeded
                        dirtyRegionManager.deferredRegions.push_back(region);
                        renderStats.skippedLowPriorityCount++;
                        continue;
                    }

                    // Clear region
                    pDeviceContext->PushAxisAlignedClip(
                        D2D1::RectF(region.rect.Left(), region.rect.Top(),
                                   region.rect.Right(), region.rect.Bottom()),
                        D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
                    );

                    pDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::White));

                    // Render subtree with clipping
                    RenderSubtree(rootNode.get(), region.rect);

                    pDeviceContext->PopAxisAlignedClip();
                }
            }

            const HRESULT hr = pDeviceContext->EndDraw();

            if (hr == D2DERR_RECREATE_TARGET) {
                SafeRelease(pTargetBitmap);
                SafeRelease(pSwapChain);
                SafeRelease(pDeviceContext);
                SafeRelease(pD2DDevice);
                CreateDeviceResources();
            }

            pSwapChain->Present(1, 0);

            // Clear dirty regions for next frame
            dirtyRegionManager.Clear();

            // Update frame time statistics
            auto frameEndTime = std::chrono::steady_clock::now();
            renderStats.lastFrameTimeMs = std::chrono::duration<float, std::milli>(
                frameEndTime - frameStartTime).count();
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

            bool handled = rootNode->OnEvent(evt);

            // If mouse down wasn't handled by any node, clear focus
            if (!handled && evt.type == EventType::MouseDown) {
                if (focusedNode) {
                    focusedNode->OnLostFocus();
                    focusedNode = nullptr;
                }
            }

            InvalidateRect(hwnd, nullptr, FALSE);
        }

        void OnKeyEvent(UINT msg, WPARAM wParam, LPARAM lParam) {
            Event evt;

            if (msg == WM_KEYDOWN || msg == WM_KEYUP) {
                evt.type = (msg == WM_KEYDOWN) ? EventType::KeyDown : EventType::KeyUp;
                evt.key = static_cast<int>(wParam);

                if (focusedNode) {
                    focusedNode->OnEvent(evt);
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            } else if (msg == WM_CHAR) {
                evt.type = EventType::Char;
                evt.character = static_cast<wchar_t>(wParam);

                if (focusedNode) {
                    focusedNode->OnEvent(evt);
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            }
        }

        void UpdateAnimations() {
            auto currentTime = std::chrono::steady_clock::now();

            // Update all node transitions recursively
            needsAnimation = UpdateNodeAnimations(rootNode.get(), currentTime);

            // Trigger redraw if animations are active
            if (needsAnimation) {
                InvalidateRect(hwnd, nullptr, FALSE);
                UpdateWindow(hwnd);  // Force immediate redraw
            }

            lastFrameTime = currentTime;
        }

        bool UpdateNodeAnimations(Node* node, std::chrono::steady_clock::time_point currentTime) {
            bool hasActive = node->GetTransitionManager().Update(node, currentTime);

            // Recursively update children
            for (const auto& child : node->GetChildren()) {
                if (UpdateNodeAnimations(child.get(), currentTime)) {
                    hasActive = true;
                }
            }

            return hasActive;
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

                case WM_MOUSEWHEEL: {
                    Event evt;
                    evt.type = EventType::MouseWheel;
                    evt.wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);

                    POINT pt;
                    pt.x = GET_X_LPARAM(lParam);
                    pt.y = GET_Y_LPARAM(lParam);
                    ScreenToClient(hwnd, &pt);

                    evt.mouseX = pt.x;
                    evt.mouseY = pt.y;

                    pImpl->rootNode->OnEvent(evt);
                    InvalidateRect(hwnd, nullptr, FALSE);
                    return 0;
                }

                case WM_KEYDOWN:
                case WM_KEYUP:
                case WM_CHAR:
                    pImpl->OnKeyEvent(msg, wParam, lParam);
                    return 0;

                case WM_SETCURSOR:
                    if (LOWORD(lParam) == HTCLIENT) {
                        return TRUE;
                    }
                    break;

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
            __uuidof(ID2D1Factory1),
            reinterpret_cast<void**>(&pimpl->pD2DFactory)
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

        // Set window pointer in root node
        pimpl->rootNode->SetWindow(this);

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
        pimpl->lastFrameTime = std::chrono::steady_clock::now();

        while (true) {
            // Process all pending messages without blocking
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    return;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            // Update animations (every frame)
            pimpl->UpdateAnimations();

            // Limit to ~60 FPS to avoid excessive CPU usage
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration<float>(
                now - pimpl->lastFrameTime
            ).count();

            if (elapsed < 1.0f / 60.0f && !pimpl->needsAnimation) {
                // Sleep briefly to avoid busy-waiting when no animations are active
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    }

    Node& Window::GetRoot() { return *pimpl->rootNode; }

    Container& Window::AddContainer() {
        auto container = std::make_unique<Container>();
        Container& ref = *container;
        pimpl->rootNode->AddChild(std::move(container));
        pimpl->rootNode->Layout();
        return ref;
    }

    void Window::SetCursor(LPCWSTR cursorType) {
        HCURSOR cursor = LoadCursor(nullptr, cursorType);
        ::SetCursor(cursor);
    }

    void Window::SetFocusedNode(Node* node) {
        // Call OnLostFocus on the previous focused node
        if (pimpl->focusedNode && pimpl->focusedNode != node) {
            pimpl->focusedNode->OnLostFocus();
        }
        pimpl->focusedNode = node;
    }

    Node* Window::GetFocusedNode() {
        return pimpl->focusedNode;
    }

    void Window::SetRenderConfig(const RenderConfig& config) {
        pimpl->renderConfig = config;
    }

    Window::RenderConfig Window::GetRenderConfig() const {
        return pimpl->renderConfig;
    }

    Window::RenderStats Window::GetRenderStats() const {
        return pimpl->renderStats;
    }

    void Window::AddDirtyRegion(const Rect& rect, int priority) {
        pimpl->dirtyRegionManager.AddDirtyRegion(rect, priority);
        // Trigger repaint
        InvalidateRect(pimpl->hwnd, nullptr, FALSE);
    }

    // ========== DirtyRegionManager Implementation ==========

    void Window::Impl::DirtyRegionManager::MergeRegions(const Window::RenderConfig& config) {
        if (dirtyRegions.size() < 2) return;

        bool changed = true;
        while (changed) {
            changed = false;

            for (size_t i = 0; i < dirtyRegions.size() && !changed; i++) {
                for (size_t j = i + 1; j < dirtyRegions.size(); j++) {
                    auto& regionA = dirtyRegions[i];
                    auto& regionB = dirtyRegions[j];

                    // Don't merge different priorities (affects rendering order)
                    if (regionA.priority != regionB.priority) {
                        continue;
                    }

                    // Check if regions overlap or are adjacent
                    const float gap = CalculateGap(regionA.rect, regionB.rect);

                    if (gap <= config.maxMergeGap) {
                        // Check area ratio
                        const Rect merged = regionA.rect.BoundingBox(regionB.rect);
                        const float mergedArea = merged.Area();
                        const float combinedArea = regionA.rect.Area() + regionB.rect.Area();

                        const float ratio = mergedArea / combinedArea;

                        if (ratio <= config.areaRatioThreshold) {
                            // Merge: expand regionA and remove regionB
                            regionA.rect = merged;
                            dirtyRegions.erase(dirtyRegions.begin() + j);
                            changed = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    float Window::Impl::DirtyRegionManager::CalculateGap(const Rect& a, const Rect& b) const {
        // Check if rectangles overlap or touch
        if (a.Intersects(b)) {
            return 0.0f;
        }

        // Calculate horizontal gap
        float hGap = 0.0f;
        if (a.Right() < b.Left()) {
            hGap = b.Left() - a.Right();
        } else if (b.Right() < a.Left()) {
            hGap = a.Left() - b.Right();
        }

        // Calculate vertical gap
        float vGap = 0.0f;
        if (a.Bottom() < b.Top()) {
            vGap = b.Top() - a.Bottom();
        } else if (b.Bottom() < a.Top()) {
            vGap = a.Top() - b.Bottom();
        }

        // Return minimum distance
        if (hGap > 0.0f && vGap > 0.0f) {
            // Diagonal distance
            return std::sqrt(hGap * hGap + vGap * vGap);
        } else {
            // Manhattan distance
            return (std::max)(hGap, vGap);
        }
    }
}

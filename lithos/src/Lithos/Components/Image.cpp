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

#include "Lithos/Components/Image.hpp"
#include "Lithos/PCH.hpp"
#include <wincodec.h>
#include <iostream>
#include <algorithm>

namespace Lithos {
    // Static WIC factory (singleton pattern like DirectWrite)
    static IWICImagingFactory* GetWICFactory() {
        static IWICImagingFactory* wicFactory = nullptr;
        if (!wicFactory) {
            HRESULT hr = CoCreateInstance(
                CLSID_WICImagingFactory,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_IWICImagingFactory,
                reinterpret_cast<void**>(&wicFactory)
            );

            if (FAILED(hr)) {
                std::cerr << "[Lithos Error] Failed to create WIC factory" << std::endl;
            }
        }
        return wicFactory;
    }

    // Helper: Convert UTF-8 std::string to std::wstring
    static std::wstring ToWString(const std::string& str) {
        if (str.empty()) return std::wstring();

        int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        std::wstring result(size, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size);

        return result;
    }

    Image::Image()
        : filePath(""),
          displayMode(ImageDisplayMode::Fill),
          imageOpacity(1.0f),
          wicConverter(nullptr),
          bitmap(nullptr),
          imageWidth(0),
          imageHeight(0) {
    }

    Image::Image(const std::string& filePath)
        : Image() {
        SetSource(filePath);
    }

    Image::~Image() {
        ReleaseResources();
    }

    Image& Image::SetSource(const std::string& filePath) {
        this->filePath = filePath;

        // Release old resources
        ReleaseResources();

        // Load new image
        if (!filePath.empty()) {
            LoadImage();
        }

        RequestLayout();
        return *this;
    }

    Image& Image::SetDisplayMode(ImageDisplayMode mode) {
        displayMode = mode;
        MarkDirty();
        return *this;
    }

    Image& Image::SetImageOpacity(float opacity) {
        imageOpacity = std::clamp(opacity, 0.0f, 1.0f);
        MarkDirty();
        return *this;
    }

    bool Image::LoadImage() {
        IWICImagingFactory* wicFactory = GetWICFactory();
        if (!wicFactory) {
            return false;
        }

        // Convert path to wide string
        std::wstring wPath = ToWString(filePath);

        // Create decoder
        IWICBitmapDecoder* decoder = nullptr;
        HRESULT hr = wicFactory->CreateDecoderFromFilename(
            wPath.c_str(),
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &decoder
        );

        if (FAILED(hr)) {
            std::cerr << "[Lithos Error] Failed to load image: " << filePath << std::endl;
            return false;
        }

        // Get first frame
        IWICBitmapFrameDecode* frame = nullptr;
        hr = decoder->GetFrame(0, &frame);

        if (FAILED(hr)) {
            std::cerr << "[Lithos Error] Failed to get image frame: " << filePath << std::endl;
            decoder->Release();
            return false;
        }

        // Create format converter (ensure BGRA format for Direct2D)
        hr = wicFactory->CreateFormatConverter(&wicConverter);

        if (FAILED(hr)) {
            std::cerr << "[Lithos Error] Failed to create format converter" << std::endl;
            frame->Release();
            decoder->Release();
            return false;
        }

        hr = wicConverter->Initialize(
            frame,
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0,
            WICBitmapPaletteTypeMedianCut
        );

        if (FAILED(hr)) {
            std::cerr << "[Lithos Error] Failed to initialize format converter" << std::endl;
            wicConverter->Release();
            wicConverter = nullptr;
            frame->Release();
            decoder->Release();
            return false;
        }

        // Get image dimensions
        UINT width, height;
        wicConverter->GetSize(&width, &height);
        imageWidth = static_cast<float>(width);
        imageHeight = static_cast<float>(height);

        // Clean up temporary resources
        frame->Release();
        decoder->Release();

        return true;
    }

    void Image::CreateBitmap(ID2D1DeviceContext* rt) {
        if (!wicConverter || bitmap) {
            return;  // Already have bitmap or no WIC data
        }

        // Create Direct2D bitmap from WIC converter
        HRESULT hr = rt->CreateBitmapFromWicBitmap(
            wicConverter,
            nullptr,
            &bitmap
        );

        if (FAILED(hr)) {
            std::cerr << "[Lithos Error] Failed to create Direct2D bitmap" << std::endl;
            return;
        }

        // Release WIC converter (no longer needed)
        if (wicConverter) {
            wicConverter->Release();
            wicConverter = nullptr;
        }
    }

    void Image::ReleaseResources() {
        if (bitmap) {
            bitmap->Release();
            bitmap = nullptr;
        }

        if (wicConverter) {
            wicConverter->Release();
            wicConverter = nullptr;
        }
    }

    D2D1_RECT_F Image::CalculateDestRect() const {
        float destX = bounds.x;
        float destY = bounds.y;
        float destWidth = bounds.width;
        float destHeight = bounds.height;

        switch (displayMode) {
            case ImageDisplayMode::Fill:
                // Use entire bounds (default)
                break;

            case ImageDisplayMode::Contain: {
                // Scale to fit inside while maintaining aspect ratio
                float scaleX = bounds.width / imageWidth;
                float scaleY = bounds.height / imageHeight;
                float scale = std::min(scaleX, scaleY);

                destWidth = imageWidth * scale;
                destHeight = imageHeight * scale;

                // Center the image
                destX = bounds.x + (bounds.width - destWidth) / 2.0f;
                destY = bounds.y + (bounds.height - destHeight) / 2.0f;
                break;
            }

            case ImageDisplayMode::Cover: {
                // Scale to cover entire bounds while maintaining aspect ratio
                float scaleX = bounds.width / imageWidth;
                float scaleY = bounds.height / imageHeight;
                float scale = std::max(scaleX, scaleY);

                destWidth = imageWidth * scale;
                destHeight = imageHeight * scale;

                // Center the image (may crop edges)
                destX = bounds.x + (bounds.width - destWidth) / 2.0f;
                destY = bounds.y + (bounds.height - destHeight) / 2.0f;
                break;
            }

            case ImageDisplayMode::None:
                // Use original image dimensions
                destWidth = imageWidth;
                destHeight = imageHeight;
                // Position at top-left of bounds
                break;
        }

        return D2D1::RectF(destX, destY, destX + destWidth, destY + destHeight);
    }

    void Image::Layout() {
        // If size not explicitly set and image is loaded, use image dimensions
        if (style.width == 0 && imageWidth > 0) {
            bounds.width = imageWidth;
        }
        if (style.height == 0 && imageHeight > 0) {
            bounds.height = imageHeight;
        }

        Node::Layout();
    }

    void Image::Draw(ID2D1DeviceContext* rt) {
        if (!visible) return;

        // Draw Node background/border/shadow first
        Node::Draw(rt);

        // Create bitmap on first draw if we have WIC data
        if (wicConverter && !bitmap) {
            CreateBitmap(rt);
        }

        // Draw bitmap
        if (bitmap) {
            D2D1_RECT_F destRect = CalculateDestRect();

            rt->DrawBitmap(
                bitmap,
                destRect,
                imageOpacity * style.opacity,
                D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
                nullptr  // Source rect (use entire image)
            );
        }
    }
}

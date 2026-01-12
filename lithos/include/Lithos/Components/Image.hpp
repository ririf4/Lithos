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
#include <string>
#include "Lithos/Core/Node.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

struct IWICFormatConverter;

namespace Lithos {
    enum class ImageDisplayMode {
        Fill,      // Stretch to fill entire bounds (may distort aspect ratio)
        Contain,   // Scale to fit inside bounds while maintaining aspect ratio
        Cover,     // Scale to cover entire bounds while maintaining aspect ratio (may crop)
        None       // Display at original size
    };

    class LITHOS_API Image : public Node {
        public:
            Image();
            explicit Image(const std::string& filePath);
            ~Image() override;

            Image(const Image&) = delete;
            Image& operator=(const Image&) = delete;
            Image(Image&&) = delete;
            Image& operator=(Image&&) = delete;

            // Image loading
            Image& SetSource(const std::string& filePath);
            const std::string& GetSource() const { return filePath; }

            // Display mode
            Image& SetDisplayMode(ImageDisplayMode mode);
            ImageDisplayMode GetDisplayMode() const { return displayMode; }

            // Image properties
            Image& SetImageOpacity(float opacity);
            float GetImageOpacity() const { return imageOpacity; }

            // Image info
            bool IsLoaded() const { return wicConverter != nullptr; }
            float GetImageWidth() const { return imageWidth; }
            float GetImageHeight() const { return imageHeight; }

            void Layout() override;
            void Draw(ID2D1DeviceContext* rt) override;

        private:
            std::string filePath;
            ImageDisplayMode displayMode;
            float imageOpacity;

            // WIC resource (persists until bitmap is created)
            IWICFormatConverter* wicConverter;

            // Direct2D resource (created on first Draw)
            ID2D1Bitmap* bitmap;

            // Cached image dimensions
            float imageWidth;
            float imageHeight;

            // Helper methods
            bool LoadImage();
            void CreateBitmap(ID2D1DeviceContext* rt);
            void ReleaseResources();
            D2D1_RECT_F CalculateDestRect() const;
    };
}

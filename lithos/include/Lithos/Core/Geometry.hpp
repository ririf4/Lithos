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
#include "../PCH.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    /**
     * @brief Abstract base class for geometric shapes used in hit testing and rendering
     */
    class LITHOS_API Geometry {
    public:
        virtual ~Geometry() = default;

        /**
         * @brief Fast bounding box check (AABB test)
         * @return true if point is within axis-aligned bounding box
         */
        virtual bool ContainsPointFast(float x, float y) const = 0;

        /**
         * @brief Precise geometric hit test
         * @return true if point is actually inside the shape
         */
        virtual bool ContainsPoint(float x, float y) const = 0;

        /**
         * @brief Get axis-aligned bounding box
         */
        virtual void GetBounds(float& left, float& top, float& right, float& bottom) const = 0;

        /**
         * @brief Get Direct2D geometry for rendering
         * @param factory D2D factory for creating geometries
         */
        virtual ID2D1Geometry* GetD2DGeometry(ID2D1Factory* factory) const = 0;

        /**
         * @brief Update geometry position and size
         */
        virtual void Update(float x, float y, float width, float height) = 0;

        /**
         * @brief Get area for optimization decisions
         */
        virtual float Area() const = 0;

        /**
         * @brief Check if this geometry intersects with another
         */
        virtual bool Intersects(const Geometry& other) const = 0;
    };

    /**
     * @brief Rectangle geometry - most common shape
     */
    class LITHOS_API RectGeometry : public Geometry {
    public:
        RectGeometry(float x = 0, float y = 0, float w = 0, float h = 0);

        bool ContainsPointFast(float px, float py) const override;
        bool ContainsPoint(float px, float py) const override;
        void GetBounds(float& left, float& top, float& right, float& bottom) const override;
        ID2D1Geometry* GetD2DGeometry(ID2D1Factory* factory) const override;
        void Update(float x, float y, float width, float height) override;
        float Area() const override;
        bool Intersects(const Geometry& other) const override;

    private:
        float x, y, width, height;
        mutable ComPtr<ID2D1RectangleGeometry> cachedGeometry;
        mutable ID2D1Factory* cachedFactory = nullptr;
    };

    /**
     * @brief Circular geometry
     */
    class LITHOS_API CircleGeometry : public Geometry {
    public:
        CircleGeometry(float cx = 0, float cy = 0, float r = 0);

        bool ContainsPointFast(float px, float py) const override;
        bool ContainsPoint(float px, float py) const override;
        void GetBounds(float& left, float& top, float& right, float& bottom) const override;
        ID2D1Geometry* GetD2DGeometry(ID2D1Factory* factory) const override;
        void Update(float x, float y, float width, float height) override;
        float Area() const override;
        bool Intersects(const Geometry& other) const override;

        void SetRadius(float r);

    private:
        float centerX, centerY, radius;
        mutable ComPtr<ID2D1EllipseGeometry> cachedGeometry;
        mutable ID2D1Factory* cachedFactory = nullptr;
    };

    /**
     * @brief Rounded rectangle geometry
     */
    class LITHOS_API RoundedRectGeometry : public Geometry {
    public:
        RoundedRectGeometry(float x = 0, float y = 0, float w = 0, float h = 0, float rx = 0, float ry = 0);

        bool ContainsPointFast(float px, float py) const override;
        bool ContainsPoint(float px, float py) const override;
        void GetBounds(float& left, float& top, float& right, float& bottom) const override;
        ID2D1Geometry* GetD2DGeometry(ID2D1Factory* factory) const override;
        void Update(float x, float y, float width, float height) override;
        float Area() const override;
        bool Intersects(const Geometry& other) const override;

        void SetRadii(float rx, float ry);

    private:
        float x, y, width, height;
        float radiusX, radiusY;
        mutable ComPtr<ID2D1RoundedRectangleGeometry> cachedGeometry;
        mutable ID2D1Factory* cachedFactory = nullptr;

        bool ContainsPointPrecise(float px, float py) const;
    };
}
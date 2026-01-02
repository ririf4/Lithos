// Element.hpp
#pragma once

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

class SkCanvas;

namespace Lithos {
    class LITHOS_API Element {
    public:
        virtual ~Element() = default;
        virtual void Draw(SkCanvas* canvas) const = 0;

        void SetPosition(float x, float y);
        void SetSize(float width, float height);
        void SetVisible(bool visible);

        float GetX() const { return x; }
        float GetY() const { return y; }
        float GetWidth() const { return width; }
        float GetHeight() const { return height; }
        bool IsVisible() const { return visible; }

    protected:
        float x = 0, y = 0;
        float width = 0, height = 0;
        bool visible = true;
    };
}
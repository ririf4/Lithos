// Element.hpp
#pragma once
#include <functional>

#include "Event.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

class SkCanvas;

namespace Lithos {
    class LITHOS_API Element {
    public:
        Element() = default;
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

        virtual bool OnEvent(const Event& event);

        bool HitTest(int x, int y) const;

        void OnResize(const std::function<void(Element*, int, int)>& callback);
        void OnClickDown(const std::function<void(Element*, MouseButton)>& callback);
        void OnClickUp(const std::function<void(Element*, MouseButton)>& callback);
        void OnMouseMove(const std::function<void(Element*, int, int)>& callback);
        void OnKeyDown(const std::function<void(Element*, int)>& callback);
        void OnKeyUp(const std::function<void(Element*, int)>& callback);

    protected:
        float x = 0, y = 0;
        float width = 0, height = 0;
        bool visible = true;

    private:
        std::function<void(Element*, int, int)> resizeCallback;
        std::function<void(Element*, MouseButton)> clickDownCallback;
        std::function<void(Element*, MouseButton)> clickUpCallback;
        std::function<void(Element*, int, int)> mouseMoveCallback;
        std::function<void(Element*, int)> keyDownCallback;
        std::function<void(Element*, int)> keyUpCallback;
    };
}
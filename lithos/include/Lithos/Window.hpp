#pragma once
#include <string>
#include <memory>

#include "Layer.hpp"

#ifdef LITHOS_EXPORTS
    #define LITHOS_API __declspec(dllexport)
#else
    #define LITHOS_API __declspec(dllimport)
#endif

namespace Lithos {
    class LITHOS_API Window {
    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        void Show() const;
        void RunMessageLoop() const;

        void AddLayer(std::unique_ptr<Layer> layer) const;  // 追加

    private:
        struct Impl;
        std::unique_ptr<Impl> pimpl;
    };
}
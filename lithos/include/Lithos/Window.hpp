#pragma once
#include <memory>
#include <string>

#include "Panel.hpp"

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

            void AddPanel(std::unique_ptr<Panel> panel) const;

        private:
            struct Impl;
            std::unique_ptr<Impl> pimpl;
    };
}

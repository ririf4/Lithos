#include <D2DBaseTypes.h>

#include "Lithos/Window.hpp"

int main() {
    Lithos::Window window(800, 600, "Lithos Test");
    window.Show();
    window.Run();
    return 0;
}
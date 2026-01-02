#include "Lithos/Window.hpp"

int main() {
    // 800x600のウィンドウを作成
    const Lithos::Window win(800, 600, "Lithos Graphics Test");
    
    win.Show();
    win.RunMessageLoop();
    
    return 0;
}
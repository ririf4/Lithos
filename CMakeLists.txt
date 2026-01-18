cmake_minimum_required(VERSION 4.1)
project(Lithos VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Lithos SHARED
        lithos/include/Lithos/PCH.hpp
        lithos/src/Lithos/PCH.cpp

        # Header Files
        lithos/include/Lithos/Core/Window.hpp
        lithos/include/Lithos/Core/Element.hpp

        lithos/include/Lithos/Core/Color.hpp

        # Source Files
        lithos/src/Lithos/Core/Window.cpp
)

target_precompile_headers(Lithos PRIVATE lithos/include/Lithos/PCH.hpp)

target_link_libraries(Lithos PRIVATE
        # --- [Graphics & Rendering] ---
        d2d1.lib            # Direct2D: 2Dグラフィックス描画の本体
        d3d11.lib           # Direct3D: ハードウェア加速・GPU操作の基盤
        dxgi.lib            # DXGI: 画面出力（スワップチェーン）やビデオカード管理
        dwrite.lib          # DirectWrite: 高品質なテキストレンダリングとフォント管理

        # --- [Windows OS Core & UI] ---
        user32.lib          # ウィンドウの作成、メッセージループ、マウス・キーボード入力
        imm32.lib           # IME (Input Method Editor): 日本語入力などの制御

        # --- [System Utilities & Resources] ---
        dxguid.lib          # DirectXのインターフェースID (GUID) 定義集
        shell32.lib         # Windowsシェル: ファイル操作、ドラッグ&ドロップ、通知など
        windowscodecs.lib   # WIC (Windows Imaging Component): PNG/JPEGなどの画像デコード
)

target_include_directories(Lithos PUBLIC lithos/include)

add_compile_definitions(UNICODE _UNICODE)

target_compile_definitions(Lithos PRIVATE LITHOS_EXPORTS)

if(MSVC)
    target_compile_options(Lithos PRIVATE /utf-8 /MP /W4 /WX)
else()
    target_compile_options(Lithos PRIVATE -Wall -Wextra -Werror)
endif()

# >==================== Example Application =================<


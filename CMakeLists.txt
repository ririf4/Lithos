cmake_minimum_required(VERSION 4.1)
project(Lithos)

set(CMAKE_CXX_STANDARD 23)

add_library(Lithos SHARED
        # HPP Header Files
        lithos/include/Lithos/Color.hpp
        lithos/include/Lithos/Rect.hpp
        lithos/include/Lithos/Event.hpp
        lithos/include/Lithos/Style.hpp

        lithos/include/Lithos/Node.hpp
        lithos/include/Lithos/TextNode.hpp
        lithos/include/Lithos/Container.hpp
        lithos/include/Lithos/ScrollableContainer.hpp
        lithos/include/Lithos/Button.hpp

        lithos/include/Lithos/Window.hpp

        # CPP Source Files
        lithos/src/Lithos/Node.cpp
        lithos/src/Lithos/TextNode.cpp
        lithos/src/Lithos/Container.cpp
        lithos/src/Lithos/ScrollableContainer.cpp
        lithos/src/Lithos/Button.cpp

        lithos/src/Lithos/Window.cpp
)

target_link_libraries(Lithos PRIVATE
        d2d1.lib
        d3d11.lib
        dxgi.lib
        dxguid.lib
        dwrite.lib
        user32.lib
        gdi32.lib
        shell32.lib
)

target_include_directories(Lithos PUBLIC lithos/include)

add_compile_definitions(UNICODE _UNICODE)

target_compile_definitions(Lithos PRIVATE LITHOS_EXPORTS)

if(MSVC)
    target_compile_options(Lithos PRIVATE /utf-8)
endif()

# >==================== Example Application =================<
project(LithosTestAndExample)

add_executable(LithosTestAndExample
        example/main.cpp
)

target_link_libraries(LithosTestAndExample PRIVATE Lithos)
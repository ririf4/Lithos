cmake_minimum_required(VERSION 4.1)
project(Lithos)

set(CMAKE_CXX_STANDARD 23)

find_package(unofficial-skia CONFIG REQUIRED)

add_library(Lithos SHARED
        # HPP Header Files
        lithos/include/Lithos/Window.hpp
        lithos/include/Lithos/Panel.hpp
        lithos/include/Lithos/Layer.hpp
        lithos/include/Lithos/Element.hpp

        lithos/include/Lithos/Event.hpp

        lithos/include/Lithos/Elements/RectElement.hpp
        lithos/include/Lithos/Elements/CircleElement.hpp
        lithos/include/Lithos/Elements/TextElement.hpp

        # CPP Source Files
        lithos/src/Lithos/Window.cpp
        lithos/src/Lithos/Panel.cpp
        lithos/src/Lithos/Layer.cpp
        lithos/src/Lithos/Element.cpp

        lithos/src/Lithos/Elements/RectElement.cpp
        lithos/src/Lithos/Elements/CircleElement.cpp
        lithos/src/Lithos/Elements/TextElement.cpp
)

target_link_libraries(Lithos PRIVATE
        unofficial::skia::skia
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
add_executable(LithosTest example/main.cpp)

set_target_properties(LithosTest PROPERTIES
        LINK_FLAGS "/ENTRY:mainCRTStartup"
)

target_link_libraries(LithosTest PRIVATE
        Lithos
        unofficial::skia::skia  # 追加
)
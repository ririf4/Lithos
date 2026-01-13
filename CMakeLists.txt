cmake_minimum_required(VERSION 3.15)
project(Lithos)

set(CMAKE_CXX_STANDARD 23)

add_library(Lithos SHARED
        lithos/include/Lithos/PCH.hpp

        # HPP Header Files
        lithos/include/Lithos/Core/Window.hpp
        lithos/include/Lithos/Core/Node.hpp
        lithos/include/Lithos/Core/Color.hpp
        lithos/include/Lithos/Core/Rect.hpp
        lithos/include/Lithos/Core/Event.hpp
        lithos/include/Lithos/Core/Style.hpp

        lithos/include/Lithos/Core/Animation/Easing.hpp
        lithos/include/Lithos/Core/Animation/AnimatableProperty.hpp
        lithos/include/Lithos/Core/Animation/Transition.hpp

        lithos/include/Lithos/Layout/Container.hpp
        lithos/include/Lithos/Layout/ScrollableContainer.hpp

        lithos/include/Lithos/Components/TextNode.hpp
        lithos/include/Lithos/Components/Button.hpp
        lithos/include/Lithos/Components/ToggleButton.hpp
        lithos/include/Lithos/Components/Image.hpp
        lithos/include/Lithos/Components/TextInput.hpp

        # CPP Source Files
        lithos/src/Lithos/Core/Window.cpp
        lithos/src/Lithos/Core/Node.cpp

        lithos/src/Lithos/Core/Animation/Transition.cpp

        lithos/src/Lithos/Layout/Container.cpp
        lithos/src/Lithos/Layout/ScrollableContainer.cpp

        lithos/src/Lithos/Components/TextNode.cpp
        lithos/src/Lithos/Components/Button.cpp
        lithos/src/Lithos/Components/ToggleButton.cpp
        lithos/src/Lithos/Components/Image.cpp
        lithos/src/Lithos/Components/TextInput.cpp
)

target_link_libraries(Lithos PRIVATE
        d2d1.lib
        d3d11.lib
        dxgi.lib
        dxguid.lib
        dwrite.lib
        imm32.lib
        user32.lib
        gdi32.lib
        shell32.lib
        windowscodecs.lib
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

add_executable(ToggleTest
        example/toggle_test.cpp
)

add_executable(TransitionDemo WIN32
        example/transition_demo.cpp
)

target_link_libraries(LithosTestAndExample PRIVATE Lithos)
target_link_libraries(ToggleTest PRIVATE Lithos)
target_link_libraries(TransitionDemo PRIVATE Lithos)
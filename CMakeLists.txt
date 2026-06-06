cmake_minimum_required(VERSION 3.16)
project(tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tetris
    main.cpp
    game.cpp
    render.cpp
)

if(WIN32)
    # Windows: SDL2 via vcpkg
    # Run: vcpkg install sdl2 sdl2-ttf
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
    target_link_libraries(tetris PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_ttf::SDL2_ttf
    )
else()
    # Linux / macOS
    find_package(SDL2 REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

    target_include_directories(tetris PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
    )
    target_link_libraries(tetris PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
    )
endif()

if(MSVC)
    target_compile_definitions(tetris PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
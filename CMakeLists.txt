cmake_minimum_required(VERSION 3.20)
project(2d-game-engine CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conaninfo.txt)
    list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")
endif ()

set(SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp
        ${CMAKE_SOURCE_DIR}/src/game/game.cpp
        ${CMAKE_SOURCE_DIR}/src/logger/logger.cpp
        src/ecs/ecs.cpp)
set(HEADERS ${CMAKE_SOURCE_DIR}/src/game/game.h
        ${CMAKE_SOURCE_DIR}/src/logger/logger.h
        src/components/transform-component.h
        src/systems/movement-system.h
        src/ecs/ecs.h)

find_package(sdl2 REQUIRED)
find_package(sdl2_image REQUIRED)
find_package(sdl2_mixer REQUIRED)
find_package(sdl2_ttf REQUIRED)
find_package(imgui REQUIRED)
find_package(glm REQUIRED)
find_package(lua REQUIRED)
find_package(sol2 REQUIRED)
find_package(spdlog REQUIRED)

include_directories(${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS})

add_executable(2d-game-engine ${SOURCES} ${HEADERS})

target_compile_features(2d-game-engine
        PRIVATE
        cxx_std_23
)
target_include_directories(2d-game-engine
        PRIVATE
        src
)
target_link_libraries(2d-game-engine PUBLIC SDL2::SDL2
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        imgui::imgui
        glm::glm
        lua::lua
        sol2::sol2
        spdlog::spdlog
)

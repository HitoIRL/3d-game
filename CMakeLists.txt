cmake_minimum_required(VERSION 3.21)
project(3d_game)

set(CMAKE_CXX_STANDARD 23)

add_executable(3d_game src/Main.cpp src/Core/Window.cpp src/Core/Shaders.cpp src/Debug/Log.cpp src/Core/Camera.cpp src/Core/Camera.hpp)

# deps
add_subdirectory(deps/glfw)
add_subdirectory(deps/glad)
add_subdirectory(deps/spdlog)
add_subdirectory(deps/assimp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF)

add_compile_definitions(GLFW_INCLUDE_NONE)

# linking
target_precompile_headers(3d_game PRIVATE <memory> <string_view>)
target_include_directories(3d_game PRIVATE deps/glm)
target_link_libraries(3d_game PRIVATE glfw glad spdlog::spdlog assimp)

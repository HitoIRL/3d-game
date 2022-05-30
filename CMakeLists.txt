cmake_minimum_required(VERSION 3.21)
project(3d_game)

set(CMAKE_CXX_STANDARD 23)

add_executable(3d_game main.cpp)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(deps/glfw)
add_compile_definitions(GLFW_INCLUDE_NONE)

# glad
add_subdirectory(deps/glad)

# linking
target_precompile_headers(3d_game PRIVATE <memory> <string_view>)
target_include_directories(3d_game PRIVATE deps/glm)
target_link_libraries(3d_game glfw glad)
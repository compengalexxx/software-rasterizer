# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(SoftwareRasterizer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Find SDL2 ---
find_package(SDL2 REQUIRED)

# --- The executable ---
add_executable(SoftwareRasterizer src/main.cpp)

# --- Link the executable against SDL2. ---
target_link_libraries(SoftwareRasterizer PRIVATE SDL2::SDL2main SDL2::SDL2)

# --- Add the include directory to the target's search path. ---
target_include_directories(SoftwareRasterizer PRIVATE ${PROJECT_SOURCE_DIR}/include)

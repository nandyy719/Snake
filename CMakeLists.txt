# CMakeList.txt : CMake project for CppSnake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.1...3.17)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/nandh/OneDrive/Desktop/Workspace/vcpkg/scripts/buildsystems/vcpkg.cmake")

project (CppSnake VERSION 1.0 DESCRIPTION "A simple snake game written in C++ using SFML")


find_package(SFML 2.5 REQUIRED COMPONENTS graphics system)

# Add source to this project's executable.
add_executable (Snake main.cpp)

target_link_libraries(Snake sfml-graphics sfml-system)

# TODO: Add tests and install targets if needed.

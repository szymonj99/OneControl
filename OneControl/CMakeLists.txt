# CMakeList.txt : CMake project for OneControl, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (OneControl "OneControl.cpp" "OneControl.h")

# TODO: Add tests and install targets if needed.

set(CMAKE_SUPPRESS_REGENERATION true)
cmake_minimum_required (VERSION 3.19 FATAL_ERROR)

project(${TESTS_NAME})

CPMAddPackage("gh:google/googletest#4556086674e4dfae2aa5f042d9e7844fc5ef9b89")

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_executable(${PROJECT_NAME} my_test.cc)

include(GoogleTest)

target_include_directories(${PROJECT_NAME} PUBLIC gtest gmock gtest_main)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARY_NAME} gtest gmock gtest_main)

gtest_discover_tests(${PROJECT_NAME})
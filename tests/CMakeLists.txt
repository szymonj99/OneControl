cmake_minimum_required (VERSION 3.19 FATAL_ERROR)

if(${ONECONTROL_USE_VCPKG})
    find_package(Catch2 3 CONFIG REQUIRED)
else()
    CPMAddPackage("gh:catchorg/Catch2#0a0ebf50034831718ffc63c6d12abb1fa469d6e9")
endif()

set(ONECONTROL_TESTS_NAME OneControlTests)

add_executable(${ONECONTROL_TESTS_NAME} ServerTests.cpp)

target_link_libraries(${ONECONTROL_TESTS_NAME} PRIVATE ${ONECONTROL_LIB_NAME} Catch2::Catch2WithMain)

target_include_directories(${ONECONTROL_TESTS_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_features(${ONECONTROL_TESTS_NAME} PRIVATE cxx_std_20)

set_target_properties(${ONECONTROL_TESTS_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${ONECONTROL_TESTS_NAME} PROPERTIES CMAKE_CXX_STANDARD 20)
set_target_properties(${ONECONTROL_TESTS_NAME} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED TRUE)
set_target_properties(${ONECONTROL_TESTS_NAME} PROPERTIES UNICODE TRUE)
set_target_properties(${ONECONTROL_TESTS_NAME} PROPERTIES CMAKE_BUILD_PARALLEL_LEVEL 8)
set_target_properties(${ONECONTROL_TESTS_NAME} PROPERTIES USE_FOLDERS TRUE)

if(CMAKE_HOST_WIN32)
    target_compile_definitions(${ONECONTROL_TESTS_NAME} PRIVATE OS_WINDOWS=1 WINVER=0x0A00 _WIN32_WINNT=0x0A00 _UNICODE=1 UNICODE=1 WIN32_LEAN_AND_MEAN=1 NOMINMAX=1)
    target_compile_options(${ONECONTROL_TESTS_NAME} PRIVATE "/MP")
elseif(CMAKE_HOST_APPLE)
    target_compile_definitions(${ONECONTROL_TESTS_NAME} PRIVATE OS_APPLE=1 _UNICODE=1 UNICODE=1)
    target_compile_options(${ONECONTROL_TESTS_NAME} PRIVATE "-Wfatal-errors")
elseif(CMAKE_HOST_LINUX)
    target_compile_definitions(${ONECONTROL_TESTS_NAME} PRIVATE OS_LINUX=1 _UNICODE=1 UNICODE=1)
    target_compile_options(${ONECONTROL_TESTS_NAME} PRIVATE "-pthread" "-Wfatal-errors")
else()
    target_compile_definitions(${ONECONTROL_TESTS_NAME} PRIVATE OS_ERROR=1)
endif()

include(CTest)
include(Catch)
catch_discover_tests(${ONECONTROL_TESTS_NAME})

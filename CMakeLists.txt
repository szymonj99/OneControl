cmake_minimum_required (VERSION 3.19 FATAL_ERROR)

set(BINARY_NAME OneControl)
set(BINARY_VERSION 0.0.1)
set(TESTS_NAME Tests)

project(${BINARY_NAME} LANGUAGES CXX VERSION ${BINARY_VERSION})
message(STATUS "${PROJECT_NAME} Version: ${PROJECT_VERSION}")

# TODO: Enable this once it's implemented
option(ONECONTROL_BUILD_TESTS "Build the tests" FALSE)
option(ONECONTROL_USE_LOCAL_ONELIBRARY "Use a local copy of OneLibrary" FALSE)
option(ONECONTROL_USE_BACKWARDCPP "Compile OneControl including Backward-CPP (only active in `Debug` mode)" TRUE)

if(NOT DEFINED ${ONECONTROL_LOCAL_ONELIBRARY_PATH})
    # For local development, change this to your own path.
    set(ONECONTROL_LOCAL_ONELIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../OneLibrary)
endif()

if(CMAKE_HOST_UNIX AND NOT CMAKE_HOST_APPLE)
	set(CMAKE_HOST_LINUX TRUE)
endif()

# https://stackoverflow.com/questions/70462926/how-to-check-my-cmake-using-debug-or-release
if (NOT CMAKE_BUILD_TYPE)
    message("Build Type not set, defaulting to Debug...")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Get CPM
include(cmake/GetCPMConfig.cmake)
include(GNUInstallDirs)

set(LINKER_LANGUAGE CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# I want this to be a self-contained application at first, so all the dependencies are included in it.
set(BUILD_SHARED_LIBS FALSE)
# set(SFML_STATIC_LIBRARIES TRUE)
if(CMAKE_HOST_WIN32)
    set(SFML_USE_STATIC_STD_LIBS TRUE)
endif()
set(SFML_GENERATE_PDB FALSE)

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_GRAPHICS FALSE)
set(SFML_BUILD_WINDOW FALSE)
set(SFML_BUILD_NETWORK TRUE)

# We use SPDLOG_FMT_EXTERNAL_HO as we use the header-only version of FMT.
set(SPDLOG_FMT_EXTERNAL_HO TRUE)
set(SPDLOG_FMT_EXTERNAL FALSE)

set(ARGS_BUILD_EXAMPLE FALSE)
set(ARGS_BUILD_UNITTESTS FALSE)

set(ONELIBRARY_USE_BACKWARDCPP TRUE)

set(UNICODE TRUE)

set(CMAKE_BUILD_PARALLEL_LEVEL 8)

set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)

# I use this to make development easier.
# This lets a user create a new file and it will be automatically compiled.
# Get all the files needed for the program
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "include/*.h" "src/*.cpp")

# Adding in CPM dependencies
CPMAddPackage("gh:SFML/sfml#fd1435d1c04750893369067d75e0f667a88e0f6c")
CPMAddPackage("gh:fmtlib/fmt#05be7a0764f8fbfbdf9c9750ec54d49fe3d2419f")
CPMAddPackage("gh:Taywee/args#3e70c396bcdf2216846810988f967d88eb165782")
if(ONECONTROL_USE_LOCAL_ONELIBRARY)
    message(STATUS "Using local OneLibrary")
    CPMFindPackage(NAME OneLibrary SOURCE_DIR ${ONECONTROL_LOCAL_ONELIBRARY_PATH})
else()
    CPMAddPackage(NAME OneLibrary
            GIT_REPOSITORY git@github.com:szymonj99/OneLibrary.git
            GIT_TAG 0aa3fba95301765926975f32e7f8d7017c105652
            VERSION 0.0.1)
endif()

# I need to remember that we 'add_library' first, then set the target options afterwards.
if(ONECONTROL_USE_BACKWARDCPP AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Adding Backward-CPP to ${PROJECT_NAME}")
    CPMAddPackage("gh:bombela/backward-cpp#90398eef20f4e7e0e939322d7e84f9c52078a325")
    add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${BACKWARD_ENABLE})
    add_backward(${PROJECT_NAME})
else()
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
# Keep in mind: Backward-CPP doesn't need to be included here, as we have already added it (in debug build) to the source files.
target_include_directories(${PROJECT_NAME} PUBLIC SFML fmt OneLibrary PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Keep in mind: We don't link backward-CPP here, as we already add backward.cpp to the source files we compile.
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-network fmt::fmt-header-only OneLibrary)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "Debug")

# Set Inter-Procedural Optimisation
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
    set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(CMAKE_HOST_WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC OS_WINDOWS=1 WINVER=0x0A00 _WIN32_WINNT=0x0A00)
    target_compile_options(${PROJECT_NAME} PUBLIC "/MP")
elseif(CMAKE_HOST_APPLE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC OS_APPLE=1)
    target_compile_options(${PROJECT_NAME} PUBLIC "-Wfatal-errors")
elseif(CMAKE_HOST_LINUX)
    target_compile_definitions(${PROJECT_NAME} PUBLIC OS_LINUX=1)
    target_compile_options(${PROJECT_NAME} PUBLIC "-pthread" "-Wfatal-errors")
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC OS_ERROR=1)
endif()

# TODO: Add `install` here.
#install(TARGETS ${PROJECT_NAME})

if(PACKAGE_TESTS)
    # enable_testing() needs to be here.
    # Reasoning: https://stackoverflow.com/questions/64099136/visual-studio-2019-test-explorer-did-not-find-c-google-tests
    #enable_testing()
    #add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

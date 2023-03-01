cmake_minimum_required (VERSION 3.19 FATAL_ERROR)

set(BINARY_NAME OneControl)
set(BINARY_VERSION 0.0.4)
set(TESTS_NAME Tests)

# TODO: Enable this once it's implemented
option(ONECONTROL_BUILD_TESTS "Build the tests" FALSE)
option(ONECONTROL_USE_LOCAL_ONELIBRARY "Use a local copy of OneLibrary" FALSE)
option(ONECONTROL_USE_BACKWARDCPP "Compile OneControl including Backward-CPP (only active in `Debug` mode)" TRUE)
option(ONECONTROL_USE_VCPKG "Use VCPKG to manage dependencies rather than the default CPM. Note: Some dependencies will still be downloaded with CPM if they are not available through VCPKG" TRUE)

if(ONECONTROL_USE_VCPKG)
    if(NOT DEFINED ENV{VCPKG_ROOT})
        message(FATAL_ERROR "VCPKG_ROOT is not set!")
    else()
        file(REAL_PATH $ENV{VCPKG_ROOT} VCPKG_ROOT)
        message(STATUS "VCPKG_ROOT is defined as: $ENV{VCPKG_ROOT} (Real Path: ${VCPKG_ROOT})")
    endif()
    # This needs to be done before calling project()
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    message(STATUS "Using VCPKG Toolchain from: ${CMAKE_TOOLCHAIN_FILE}")
endif()
project(${BINARY_NAME} LANGUAGES CXX VERSION ${BINARY_VERSION})
message(STATUS "${PROJECT_NAME} Version: ${PROJECT_VERSION}")

if(NOT DEFINED ${ONECONTROL_LOCAL_ONELIBRARY_PATH})
    # For local development, change this to your own path.
    set(ONECONTROL_LOCAL_ONELIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../OneLibrary)
endif()

if(CMAKE_HOST_UNIX AND NOT CMAKE_HOST_APPLE)
	set(CMAKE_HOST_LINUX TRUE)
endif()

# https://stackoverflow.com/questions/70462926/how-to-check-my-cmake-using-debug-or-release
if(NOT CMAKE_BUILD_TYPE)
    message("Build Type not set, defaulting to Debug...")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Get CPM
include(cmake/GetCPMConfig.cmake)
include(GNUInstallDirs)

if(CMAKE_HOST_WIN32)
    set(SFML_USE_STATIC_STD_LIBS FALSE)
endif()
set(SFML_GENERATE_PDB FALSE)
set(SFML_BUILD_EXAMPLES FALSE)

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_GRAPHICS FALSE)
set(SFML_BUILD_WINDOW FALSE)
set(SFML_BUILD_NETWORK TRUE)

# We use SPDLOG_FMT_EXTERNAL_HO as we use the header-only version of FMT.
set(SPDLOG_FMT_EXTERNAL_HO TRUE)
set(SPDLOG_FMT_EXTERNAL FALSE)

set(ARGS_BUILD_EXAMPLE FALSE)
set(ARGS_BUILD_UNITTESTS FALSE)

set(ONELIBRARY_USE_BACKWARDCPP ${ONECONTROL_USE_BACKWARDCPP})

# I use this to make development easier.
# This lets a user create a new file and it will be automatically compiled.
# Get all the files needed for the program
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h" "${PROJECT_SOURCE_DIR}/src/*.cpp")

if(ONECONTROL_USE_VCPKG)
    find_package(SFML COMPONENTS network CONFIG REQUIRED)
    find_package(fmt CONFIG REQUIRED)
    find_package(args CONFIG REQUIRED)
    find_package(spdlog CONFIG REQUIRED)
    find_package(cryptopp CONFIG REQUIRED)
else()
    # Adding in CPM dependencies
    CPMAddPackage("gh:SFML/sfml#fd1435d1c04750893369067d75e0f667a88e0f6c")
    CPMAddPackage("gh:fmtlib/fmt#05be7a0764f8fbfbdf9c9750ec54d49fe3d2419f")
    CPMAddPackage("gh:Taywee/args#3e70c396bcdf2216846810988f967d88eb165782")
    CPMAddPackage("gh:gabime/spdlog#05e3a73b162705b37ed464ceb4644addfb03f25b")
    CPMAddPackage("gh:weidai11/cryptopp#c364032d4c9e025704cf7caf379e8880ad87a59a")
endif()

if(ONECONTROL_USE_LOCAL_ONELIBRARY)
    message(STATUS "Using local OneLibrary")
    CPMFindPackage(NAME OneLibrary SOURCE_DIR ${ONECONTROL_LOCAL_ONELIBRARY_PATH})
else()
    # As OneLibrary is only available on GitHub and not through vcpkg, we need to include it separately.
    CPMAddPackage(NAME OneLibrary
            GIT_REPOSITORY git@github.com:szymonj99/OneLibrary.git
            GIT_TAG 598a6b072d73672c5c22e2bc183056c3a459dbf0
            VERSION 0.0.2)
endif()

# I need to remember that we 'add_library' first, then set the target options afterwards.
if(ONECONTROL_USE_BACKWARDCPP AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Adding Backward-CPP to ${PROJECT_NAME}")
    # Backward-cpp is not available through vcpkg
    CPMAddPackage("gh:bombela/backward-cpp#90398eef20f4e7e0e939322d7e84f9c52078a325")
    add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${BACKWARD_ENABLE})
    add_backward(${PROJECT_NAME})
else()
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
# I'm not sure if this is necessary when we use `find_package` :thinking:
#target_include_directories(${PROJECT_NAME} PRIVATE SFML fmt args spdlog ${DBG_MACRO_INCLUDE_DIRS} OneLibrary ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-network fmt::fmt-header-only taywee::args spdlog::spdlog_header_only cryptopp::cryptopp OneLibrary)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "Debug")

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CXX_STANDARD 20)
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES UNICODE TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_BUILD_PARALLEL_LEVEL 8)
set_target_properties(${PROJECT_NAME} PROPERTIES USE_FOLDERS TRUE)

# Set Inter-Procedural Optimisation
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
    set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(CMAKE_HOST_WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE OS_WINDOWS=1 WINVER=0x0A00 _WIN32_WINNT=0x0A00 _UNICODE=1 UNICODE=1 WIN32_LEAN_AND_MEAN=1 NOMINMAX=1)
    target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
elseif(CMAKE_HOST_APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE OS_APPLE=1 _UNICODE=1 UNICODE=1)
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wfatal-errors")
elseif(CMAKE_HOST_LINUX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE OS_LINUX=1 _UNICODE=1 UNICODE=1)
    target_compile_options(${PROJECT_NAME} PRIVATE "-pthread" "-Wfatal-errors")
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE OS_ERROR=1)
endif()

# TODO: Add `install` here.
#install(TARGETS ${PROJECT_NAME})

if(ONECONTROL_BUILD_TESTS)
    # enable_testing() needs to be here.
    # Reasoning: https://stackoverflow.com/questions/64099136/visual-studio-2019-test-explorer-did-not-find-c-google-tests
    #enable_testing()
    #add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

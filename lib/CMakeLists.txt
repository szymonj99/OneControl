cmake_minimum_required (VERSION 3.19 FATAL_ERROR)

project(${LIBRARY_NAME} LANGUAGES CXX VERSION 0.0.2)

message(STATUS "OneControl Library version: ${PROJECT_VERSION}")

CPMAddPackage("gh:SFML/sfml#fd1435d1c04750893369067d75e0f667a88e0f6c")
CPMAddPackage("gh:fmtlib/fmt#05be7a0764f8fbfbdf9c9750ec54d49fe3d2419f")
CPMAddPackage("gh:adishavit/argh#39b9a00046143f6b300dcf98ea989486cf62097d")

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.h" "*.cpp")

# Let's only add 'Backward-CPP' if we are running a debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Adding in Backward-CPP to ${PROJECT_NAME}")
	CPMAddPackage("gh:bombela/backward-cpp#74184aad55777f0c04227edd56c3dea84b6a272f")
	# packagename_SOURCE_DIR is provided here by CPM (I suppose it uses FetchPackage under the hood which actually gives us the definitions for the packages, similar to NuGet in C# :thumbs_up: )
	list(APPEND SOURCE_FILES "${backward_SOURCE_DIR}/backward.cpp")
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# Keep in mind: Backward-CPP doesn't need to be included here, as we have already added it (in debug build) to the source files.
target_include_directories(${PROJECT_NAME} PUBLIC SFML fmt argh "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
# Keep in mind: We don't link backward-CPP here, as we already add backward.cpp to the source files we compile.
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-network fmt::fmt-header-only argh)

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

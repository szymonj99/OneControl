cmake_minimum_required (VERSION 3.19 FATAL_ERROR)

project(${BINARY_NAME} LANGUAGES CXX VERSION 0.0.1)

message(STATUS "OneControl Version: ${PROJECT_VERSION}")

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.h" "*.cpp")

# Let's only add 'Backward-CPP' if we are running a debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Adding in Backward-CPP to ${PROJECT_NAME}")
	CPMAddPackage("gh:bombela/backward-cpp#74184aad55777f0c04227edd56c3dea84b6a272f")
	# packagename_SOURCE_DIR is provided here by CPM (I suppose it uses FetchPackage under the hood which actually gives us the definitions for the packages, similar to NuGet in C# :thumbs_up: )
	list(APPEND SOURCE_FILES "${backward_SOURCE_DIR}/backward.cpp")
endif()

# I need to remember that we 'add_library' first, then set the target options afterwards.
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/include" SFML fmt argh)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARY_NAME})

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(CMAKE_HOST_WIN32)
	target_compile_definitions(${PROJECT_NAME} PUBLIC OS_WINDOWS=1 WINVER=0x0A00 _WIN32_WINNT=0x0A00)
elseif(CMAKE_HOST_APPLE)
	target_compile_definitions(${PROJECT_NAME} PUBLIC OS_APPLE=1)
	target_compile_options(${PROJECT_NAME} PUBLIC "-Wfatal-errors")
elseif(CMAKE_HOST_LINUX)
	target_compile_definitions(${PROJECT_NAME} PUBLIC OS_LINUX=1)
	target_compile_options(${PROJECT_NAME} PUBLIC "-pthread" "-Wfatal-errors")
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC OS_ERROR=1)
endif()

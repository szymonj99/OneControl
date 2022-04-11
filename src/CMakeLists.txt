cmake_minimum_required (VERSION 3.19 FATAL_ERROR) # 3.14 minimum for FetchContent

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif(UNIX AND NOT APPLE)

if (LINUX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread")
endif(LINUX)

# Full optimisations for release, none for debug.
IF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
ENDIF()

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS "./*/*.h" "*.cpp" "./*/*.cpp" "./*/*/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include(FetchContent)

FetchContent_Declare(sfml
  GIT_REPOSITORY https://github.com/SFML/sfml.git
  GIT_TAG ef8d6ee7f710fff9bdaf69c6bb3660fa4d3a8958
)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG afbcf1e8eafc5d7f27e29c7397f22521eaa33fac
)

FetchContent_Declare(spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG 0b48976be49a69d8ee30b9b153b261704bbc02a0
)

FetchContent_Declare(argh
  GIT_REPOSITORY https://github.com/adishavit/argh.git
  GIT_TAG 1e52a8048cdc0b13b829dccfc1172768af3663a0
)

FetchContent_MakeAvailable(sfml fmt spdlog argh)

target_include_directories(${PROJECT_NAME} PRIVATE SFML fmt spdlog argh interception)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-network fmt::fmt-header-only spdlog argh)

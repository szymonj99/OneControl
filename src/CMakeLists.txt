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
  GIT_TAG 0ad6d1815cb2ee33289aa1b7c38d3831318a5db8
)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG a7aecbfcaa0e1586da9cf8175bd8d1c4383acd8c
)

FetchContent_Declare(spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG 8fb112158a64afe1bd58c82c06bf6a142c9b3823
)

FetchContent_Declare(argh
  GIT_REPOSITORY https://github.com/adishavit/argh.git
  GIT_TAG 1e52a8048cdc0b13b829dccfc1172768af3663a0
)

FetchContent_MakeAvailable(sfml fmt spdlog argh)
include_directories(SFML fmt spdlog argh)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-network fmt::fmt-header-only spdlog argh)

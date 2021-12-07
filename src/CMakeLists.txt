cmake_minimum_required (VERSION 3.19 FATAL_ERROR) # 3.14 minimum for FetchContent

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif(UNIX AND NOT APPLE)

if (LINUX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
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
  GIT_TAG 2.6.x
)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG d141cdbeb0fb422a3fb7173b285fd38e0d1772dc # Release 8.0.1
)

FetchContent_Declare(spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG eb3220622e73a4889eee355ffa37972b3cac3df5 # Release 1.9.2
)

FetchContent_Declare(argh
  GIT_REPOSITORY https://github.com/adishavit/argh.git
  GIT_TAG master
)

FetchContent_MakeAvailable(sfml fmt spdlog argh)
include_directories(SFML fmt spdlog argh)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-audio sfml-window sfml-network sfml-system fmt::fmt-header-only spdlog::spdlog_header_only argh)

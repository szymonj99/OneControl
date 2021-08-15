cmake_minimum_required (VERSION 3.19 FATAL_ERROR) # 3.14 minimum for FetchContent

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS "*.h" "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include(FetchContent)

FetchContent_Declare(sfml
  GIT_REPOSITORY https://github.com/SFML/sfml
  GIT_TAG master
)
FetchContent_MakeAvailable(sfml)
include_directories(SFML)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)
include_directories(fmt)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-audio sfml-network sfml-system)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)

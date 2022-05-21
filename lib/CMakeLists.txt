cmake_minimum_required (VERSION 3.19 FATAL_ERROR)

project(${LIBRARY_NAME} LANGUAGES CXX VERSION 0.0.1)

message(STATUS "OneControl Library version: ${PROJECT_VERSION}")

CPMAddPackage("gh:SFML/sfml#fd1435d1c04750893369067d75e0f667a88e0f6c")
CPMAddPackage("gh:fmtlib/fmt#48b7e3dafb27ece02cd6addc8bd1041c79d59c2c")
CPMAddPackage("gh:adishavit/argh#39b9a00046143f6b300dcf98ea989486cf62097d")

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.h" "*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC SFML fmt argh "include")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-network fmt::fmt-header-only argh)

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

cmake_minimum_required (VERSION 3.19 FATAL_ERROR) # 3.14 minimum for FetchContent

if(CMAKE_HOST_UNIX AND NOT CMAKE_HOST_APPLE)
	set(CMAKE_HOST_LINUX TRUE)
endif()

include(FetchContent)

FetchContent_Declare(sfml
  GIT_REPOSITORY https://github.com/SFML/sfml.git
  GIT_TAG f8c1ec283ad81b693d5c99b2c579173256f92693
)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG ffb5e6a7328986706b48c3b4964ec596026f5e01
)

FetchContent_Declare(argh
  GIT_REPOSITORY https://github.com/adishavit/argh.git
  GIT_TAG d17062c870b5919f6d1eae7fe12879869a893b32
)

FetchContent_Declare(spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG b3ce5ed379ed01d9147b079e605e84dd408a0c19
)

FetchContent_MakeAvailable(sfml fmt argh spdlog)

set(SOURCE_FILES "./main.cpp" "./Client/Client.h" "./Client/Client.cpp" "./Helpers/Constants.h" "./Helpers/Enums.h" "./Helpers/StaticHelpers.h" "./KeyboardReceiver/KeyboardReceiver.h" "./KeyboardSender/KeyboardSender.h"
"./MouseReceiver/MouseReceiver.h" "./MouseSender/MouseSender.h" "./Packet/Packet.h" "./Packet/Packet.cpp" "./Program/OneControl.h" "./Program/OneControl.cpp"
"./Server/Server.h" "./Server/Server.cpp" "./Version/Version.cpp" "./Version/Version.h")

if(CMAKE_HOST_WIN32)
	set(SOURCE_FILES ${SOURCE_FILES} "./Server/Windows/Server.cpp" "./KeyboardReceiver/Windows/KeyboardReceiver.cpp" "./KeyboardSender/Windows/KeyboardSender.cpp" "./MouseReceiver/Windows/MouseReceiver.cpp" "./MouseSender/Windows/MouseSender.cpp" "./Windows/MessageTimer/MessageTimer.h" "./Windows/MessageTimer/MessageTimer.cpp")
elseif(CMAKE_HOST_LINUX)
	set(SOURCE_FILES ${SOURCE_FILES} "./Server/Linux/Server.cpp" "./KeyboardReceiver/Linux/KeyboardReceiver.cpp" "./KeyboardSender/Linux/KeyboardSender.cpp" "./MouseReceiver/Linux/MouseReceiver.cpp" "./MouseSender/Linux/MouseSender.cpp")
elseif(CMAKE_HOST_APPLE)
	set(SOURCE_FILES ${SOURCE_FILES} "./Server/Mac/Server.cpp" "./KeyboardReceiver/Mac/KeyboardReceiver.cpp" "./KeyboardSender/Mac/KeyboardSender.cpp" "./MouseReceiver/Mac/MouseReceiver.cpp" "./MouseSender/Mac/MouseSender.cpp")
endif()

# I want this to be a self-contained application at first, so all the dependencies are included in it.
set(BUILD_SHARED_LIBS OFF)
set(SFML_STATIC_LIBRARIES ON)
set(SFML_USE_STATIC_STD_LIBS ON)
set(SFML_GENERATE_PDB OFF)

set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_GRAPHICS OFF)
set(SFML_BUILD_WINDOW OFF)
set(SFML_BUILD_NETWORK ON)

# We use SPDLOG_FMT_EXTERNAL_HO as we use the header-only version of FMT.
set(SPDLOG_FMT_EXTERNAL_HO ON)
set(SPDLOG_FMT_EXTERNAL OFF)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE SFML fmt argh spdlog)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-network fmt::fmt-header-only argh spdlog::spdlog_header_only)

target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE=1)

if(CMAKE_HOST_WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE OS_WINDOWS=1 WINVER=0x0A00 _WIN32_WINNT=0x0A00)
elseif(CMAKE_HOST_APPLE)
	target_compile_definitions(${PROJECT_NAME} PRIVATE OS_APPLE=1)
	target_compile_options(${PROJECT_NAME} PRIVATE "-Wfatal-errors")
elseif(CMAKE_HOST_LINUX)
	target_compile_definitions(${PROJECT_NAME} PRIVATE OS_LINUX=1)
	target_compile_options(${PROJECT_NAME} PRIVATE "-pthread" "-Wfatal-errors")
else()
	target_compile_definitions(${PROJECT_NAME} PRIVATE OS_ERROR=1)
endif()

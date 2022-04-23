cmake_minimum_required (VERSION 3.19 FATAL_ERROR)

if(CMAKE_HOST_UNIX AND NOT CMAKE_HOST_APPLE)
	set(CMAKE_HOST_LINUX TRUE)
endif()

CPMAddPackage("gh:SFML/sfml#3ae85854e4674debe3b30ae2f6cf9d9926738789")
CPMAddPackage("gh:fmtlib/fmt#7e4ad40171aa552d38cb99a5c181a0d7b150facc")
CPMAddPackage("gh:adishavit/argh#d17062c870b5919f6d1eae7fe12879869a893b32")
CPMAddPackage("gh:gabime/spdlog#b3ce5ed379ed01d9147b079e605e84dd408a0c19")
CPMAddPackage("gh:kwhat/libuiohook#88c45565f41bf83863bc436c289dd3bd2a28edfd")

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

target_include_directories(${PROJECT_NAME} PRIVATE SFML fmt argh spdlog uiohook)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-network fmt::fmt-header-only argh spdlog::spdlog_header_only uiohook)

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

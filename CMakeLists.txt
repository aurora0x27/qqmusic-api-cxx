cmake_minimum_required(VERSION 3.20)

# Be sure it is built as Release Mode
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

project(qmapi LANGUAGES CXX)
file(GLOB_RECURSE QQMUSIC_API_SOURCE CONFIGURE_DEPENDS src/*.cc src/utils/*.cc src/details/*.cc)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(botan CONFIG REQUIRED)
find_package(boost_asio CONFIG REQUIRED)
find_package(boost_beast CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS url)
find_package(boost_uuid REQUIRED CONFIG)
find_package(OpenSSL 3.3.0 REQUIRED)

add_library(qmapi SHARED ${QQMUSIC_API_SOURCE})
target_include_directories(
    qmapi PUBLIC
    include
)

# set platform info
if(WIN32)
    set(PLATFORM PLATFORM_WINDOWS)
    target_compile_definitions(qmapi PRIVATE _WIN32_WINNT=0x0A00)
    target_compile_options(qmapi PRIVATE /bigobj)
    set_target_properties(qmapi PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    target_link_libraries(
        qmapi PUBLIC
        ZLIB::ZLIB
        nlohmann_json::nlohmann_json
        Botan::Botan
        Boost::asio
        Boost::beast
        Boost::url
        Boost::uuid
        OpenSSL::SSL
    )
elseif(APPLE)
    set(PLATFORM PLATFORM_APPLE)
    target_link_libraries(
        qmapi PUBLIC
        ZLIB::ZLIB
        nlohmann_json::nlohmann_json
        Botan::Botan-static
        Boost::asio
        Boost::beast
        Boost::url
        Boost::uuid
        OpenSSL::SSL
    )
elseif(LINUX)
    set(PLATFORM PLATFORM_LINUX)
    target_link_libraries(
        qmapi PUBLIC
        ZLIB::ZLIB
        nlohmann_json::nlohmann_json
        Botan::Botan-static
        Boost::asio
        Boost::beast
        Boost::url
        Boost::uuid
        OpenSSL::SSL
    )
endif()

target_compile_definitions(
    qmapi PRIVATE
    ${PLATFORM}
)

set(IS_DEMO "OFF")
set(IS_SHARED_LIB "OFF")
set(IS_TEST "OFF")
if (NOT DEFINED QQMUSIC_API_BUILD_TYPE)
    message(FATAL_ERROR "==>QQMUSIC_API_BUILD_TYPE not set...")
elseif(${QQMUSIC_API_BUILD_TYPE} STREQUAL "Demo")
    set(IS_DEMO "ON ")
    add_subdirectory(demo)
elseif(${QQMUSIC_API_BUILD_TYPE} STREQUAL "Test")
    set(IS_TEST "ON ")
    add_subdirectory(test)
elseif(${QQMUSIC_API_BUILD_TYPE} STREQUAL "SharedLib")
    set(IS_SHARED_LIB "ON ")
else ()
    message(FATAL_ERROR "==>QQMUSIC_API_BUILD_TYPE set error...")
endif()

message("+---------------QQMUSIC API CXX---------------+")
message("|                                             |")
message("|              QQMUSIC BUILD TYPE             |")
message("|                                             |")
message("|    DEMO                           ${IS_DEMO}       |")
message("|                                             |")
message("|    SHARED LIBRARY                 ${IS_SHARED_LIB}       |")
message("|                                             |")
message("|    TEST                           ${IS_TEST}       |")
message("|                                             |")
message("+---------------------------------------------+")

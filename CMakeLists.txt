cmake_minimum_required(VERSION 3.20)

# Be sure it is built as Release Mode
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

project(qmapi LANGUAGES CXX)
file(GLOB_RECURSE QQMUSIC_API_SOURCE CONFIGURE_DEPENDS src/*.cc include/*.h)
find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(jsoncpp REQUIRED)
add_subdirectory(utils)

add_library(qmapi SHARED ${QQMUSIC_API_SOURCE})
target_include_directories(
    qmapi PUBLIC
    include
)
target_link_libraries(
    qmapi PUBLIC
    qmutils
    ZLIB::ZLIB
    CURL::libcurl
    JsonCpp::JsonCpp
)

if (NOT DEFINED QQMUSIC_API_BUILD_TYPE)
    message(FATAL_ERROR "==>QQMUSIC_API_BUILD_TYPE not set...")
elseif(${QQMUSIC_API_BUILD_TYPE} STREQUAL "DEMO")
    message("==>Building Demo...")
    add_executable(demo demo/main.cc)
    target_include_directories(
        demo PUBLIC
        include
    )
    target_link_libraries(
        demo PUBLIC
        qmapi
    )
elseif(${QQMUSIC_API_BUILD_TYPE} STREQUAL "SHARED_LIB")
    message("==>Building as shared library...")
else ()
    message(FATAL_ERROR "==>QQMUSIC_API_BUILD_TYPE set error...")
endif()

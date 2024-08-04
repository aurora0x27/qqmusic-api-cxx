cmake_minimum_required(VERSION 3.20)

# Be sure it is built as Release Mode
# if (NOT CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE Release)
# endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(utils/NetWork)

file(GLOB_RECURSE QQMUSIC_API_SOURCE CONFIGURE_DEPENDS src/*.cc include/*.h)

if (NOT DEFINED QQMUSIC_API_BUILD_TYPE)
    message(FATAL_ERROR "==>QQMUSIC_API_BUILD_TYPE not set...")
elseif(${QQMUSIC_API_BUILD_TYPE} STREQUAL "DEMO")
    message("==>Building Demo...")
    project(demo LANGUAGES CXX)
    set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
    add_executable(demo demo/main.cc)
    add_library(qqmusic-api SHARED ${QQMUSIC_API_SOURCE})
elseif(${QQMUSIC_API_BUILD_TYPE} STREQUAL "SHARED_LIB")
    message("==>Building as shared library...")
    add_library(qqmusic-api SHARED ${QQMUSIC_API_SOURCE})
elseif(${QQMUSIC_API_BUILD_TYPE} STREQUAL "STATIC_LIB")
    message("==>Building as static library...")
    add_library(qqmusic-api STATIC ${QQMUSIC_API_SOURCE})
else ()
    message(FATAL_ERROR "==>QQMUSIC_API_BUILD_TYPE set error...")
endif()

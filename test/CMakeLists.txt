# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE QQMUSIC_API_TEST_BUILD_SOURCE CONFIGURE_DEPENDS src/*.cc)
add_executable(test ${QQMUSIC_API_TEST_BUILD_SOURCE})
target_include_directories(
    test PUBLIC
    ../include/
    include/
)
target_link_libraries(
    test PUBLIC
    qmapi
    GTest::gtest_main
)

# include(GoogleTest)
# gtest_discover_tests(test)

# Be sure it is built as Release Mode
# if (NOT CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE Release)
# endif()

file(GLOB_RECURSE NETWORK_BUILD_SOURCE CONFIGURE_DEPENDS src/*.cc include/*.h)

find_package(boost REQUIRED)

add_library(network SHARED ${NETWORK_BUILD_SOURCE})
set(LIBRARY_OUTPUT_PATH lib/)
target_include_directories(network PUBLIC include)
target_link_libraries(
    network PUBLIC
	boost::asio
)

file(GLOB_RECURSE QMDETAILS_BUILD_SOURCE src/*.cc include/*.h include/*.hpp)
add_library(qmdetails STATIC ${QMDETAILS_BUILD_SOURCE})

target_include_directories(
    qmdetails PUBLIC
    include
    ../include
)

target_link_libraries(
    qmdetails PUBLIC
)

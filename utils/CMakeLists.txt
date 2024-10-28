file(GLOB_RECURSE QMUTILS_BIULD_SOURCE src/*.cc include/*.h include/*.hpp)
add_library(qmutils STATIC ${QMUTILS_BIULD_SOURCE})

target_include_directories(
    qmutils PUBLIC
    include
    ../include
)

# target_link_libraries(
#     qmutils PUBLIC
# )

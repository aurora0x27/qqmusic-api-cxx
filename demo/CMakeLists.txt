add_executable(qrc-decoder src/qrc-decoder.cc)
target_include_directories(
    qrc-decoder PUBLIC
    include
)
target_link_libraries(
    qrc-decoder PUBLIC
    qmapi
)

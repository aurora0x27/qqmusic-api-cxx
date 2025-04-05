include(cmake/subproj.cmake)

set(QRC_DECODER_FILES src/qrc-decoder.cc)
add_subproject(qrc-decoder ${QRC_DECODER_FILES})

set(SONG_GRAP_FILES src/song-grap.cc)
add_subproject(song-grap ${SONG_GRAP_FILES})

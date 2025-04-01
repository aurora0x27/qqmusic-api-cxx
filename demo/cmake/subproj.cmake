function(add_subproject PROJ_NAME FILE_LIST)
    add_executable(${PROJ_NAME} ${FILE_LIST})
    target_include_directories(
        ${PROJ_NAME} PUBLIC
        include
    )

    target_link_libraries(
        ${PROJ_NAME} PUBLIC
        qmapi
    )
endfunction(add_subproject)

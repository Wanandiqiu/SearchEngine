aux_source_directory(. SRC_LIST)

add_executable(ThreaedPool ${SRC_LIST})

target_link_libraries(ThreaedPool pthread)
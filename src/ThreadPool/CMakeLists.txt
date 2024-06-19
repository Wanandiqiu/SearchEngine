aux_source_directory(. SRC_LIST)

add_executable(ThreadPool ${SRC_LIST})

target_link_libraries(ThreadPool pthread)
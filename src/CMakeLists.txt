# add_subdirectory(ThreadPool)
# add_subdirectory(Reactor)

aux_source_directory(./Reactor SRC_LIST)
aux_source_directory(./ThreadPool SRC_LIST2)

add_executable(ReactorV1 ${SRC_LIST} ${SRC_LIST2})

target_link_libraries(ReactorV1 pthread)
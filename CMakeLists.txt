cmake_minimum_required(VERSION 3.0)
project(searchEngine)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

# 配置最终的可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 配置头文件的搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/ThreadPool)
include_directories(${PROJECT_SOURCE_DIR}/include/Reactor)

add_subdirectory(src)
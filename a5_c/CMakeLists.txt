cmake_minimum_required(VERSION 3.13)
project(feistel)

link_libraries(m)
file(GLOB_RECURSE src_files "src/*.c")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")
add_executable(feistel ${src_files})

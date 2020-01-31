cmake_minimum_required(VERSION 3.0)
project(Glad)

set(CMAKE_CXX_STANDARD 17)

add_library(
    glad
    glad/glad.h
    glad/glad.c
)

target_include_directories(
    glad PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
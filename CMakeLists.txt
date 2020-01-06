# Don't use commands 
#   add_compile_options
#   include_directories
#   link_directories
#   link_libraries
#   cmake_cxx_flags

cmake_minimum_required(VERSION 3.10)
project(OpenGL_Project VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/glfw)

add_subdirectory(extern/glad)

add_subdirectory(source)
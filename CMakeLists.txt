cmake_minimum_required(VERSION 3.29)
project(scheme-interpreter)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)

include(sources.cmake)

target_include_directories(scheme_tidy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SCHEME_COMMON_DIR})

add_executable(scheme_tidy_repl repl/main.cpp)
target_link_libraries(scheme_tidy_repl scheme_tidy)
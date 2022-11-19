cmake_minimum_required(VERSION 3.9)
project(Platformer CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)

file(WRITE ${CMAKE_BINARY_DIR}/.gitignore "*")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(glfw3)
find_package(raylib)

add_executable(platformer ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_include_directories(platformer PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(platformer PUBLIC glfw raylib)
add_custom_command(TARGET platformer PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Images ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Images)
add_custom_command(TARGET platformer PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Levels ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Levels)
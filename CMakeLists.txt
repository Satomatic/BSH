cmake_minimum_required(VERSION 3.5)
project(BSH)

include_directories(./src/include)

file(GLOB_RECURSE SRCS src/*.cpp)
file(GLOB_RECURSE HDRS src/*.h)

add_executable(bsh ${SRCS} ${HDRS})

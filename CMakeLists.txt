cmake_minimum_required(VERSION 2.6)
project(gofishproject)

# Set the compiler to use the C++11 standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(gofishproject Client.cpp)

install(TARGETS gofishproject RUNTIME DESTINATION bin)

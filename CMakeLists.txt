cmake_minimum_required(VERSION 3.9)
project(regkeymonitor)

set(CMAKE_CXX_STANDARD 11)

add_executable(regkeymonitor main.cpp)
add_executable(QueryKey QueryKey.cpp)
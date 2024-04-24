cmake_minimum_required(VERSION 3.10)
project(yas)

set(CMAKE_CXX_STANDARD 17)

add_library(yas INTERFACE)
target_include_directories(yas INTERFACE include)

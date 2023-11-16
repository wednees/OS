cmake_minimum_required(VERSION 3.10)

project(lab)

add_compile_options(-pedantic -Wall -pthread)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(lab_exe main.cpp src.cpp)
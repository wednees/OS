cmake_minimum_required(VERSION 3.10)

project(lab)

add_compile_options(-pedantic -Wall)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(main main.cpp funcs.cpp)
add_executable(child1 child1.cpp funcs.cpp)
add_executable(child2 child2.cpp funcs.cpp)
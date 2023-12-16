cmake_minimum_required(VERSION 3.10)

add_compile_options(-pedantic -Wall)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lab)

add_library(prime_count_dyn SHARED prime_count_lib/prime_count.cpp)
add_library(${PROJECT_NAME}_dyn_lib::lib1 ALIAS prime_count_dyn)
target_include_directories(prime_count_dyn PUBLIC ${PROJECT_SOURCE_DIR}/prime_count_lib)

add_library(num_sys_dyn SHARED num_sys_lib/num_sys.cpp)
add_library(${PROJECT_NAME}_dyn_lib::lib2 ALIAS num_sys_dyn)
target_include_directories(num_sys_dyn PUBLIC ${PROJECT_SOURCE_DIR}/num_sys_lib)

add_executable(${PROJECT_NAME}_runtime_exe main_runtime.cpp)

target_link_libraries(${PROJECT_NAME}_runtime_exe PRIVATE ${PROJECT_NAME}_dyn_lib::lib1 ${PROJECT_NAME}_dyn_lib::lib2)

add_executable(${PROJECT_NAME}_link_exe main_linking.cpp)

target_link_libraries(${PROJECT_NAME}_link_exe PRIVATE ${PROJECT_NAME}_dyn_lib::lib1 ${PROJECT_NAME}_dyn_lib::lib2)
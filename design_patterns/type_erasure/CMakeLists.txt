cmake_minimum_required(VERSION 3.10)
project(type_erasure)

set(CMAKE_CXX_STANDARD 17)

# Add the executable target
add_executable(type_erasure main.cpp algorithm_impl/algorithm.cpp algorithm_impl/cnt_impl.cpp algorithm_impl/graphene_impl.cpp)

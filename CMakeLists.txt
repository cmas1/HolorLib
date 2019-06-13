cmake_minimum_required(VERSION 3.10.0)
project(BSTools VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 17)



add_definitions(-DDEFINE_ASSERT_LEVEL=AssertionLevel::debug)

add_executable(poly_example src/examples/poly_example.cpp)
add_executable(tensor_example src/examples/tensor_example.cpp)
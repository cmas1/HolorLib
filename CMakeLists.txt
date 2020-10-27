cmake_minimum_required(VERSION 3.10.0)
project(Holor VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 20)

add_definitions(-DDEFINE_ASSERT_LEVEL=AssertionLevel::release)
add_definitions(-DDEFINE_EXCEPTION_POLICY=ExceptionPolicy::off)


include_directories(./include/)


add_subdirectory(examples)
#add_subdirectory(benchmark)
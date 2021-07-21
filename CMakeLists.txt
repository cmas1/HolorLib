cmake_minimum_required(VERSION 3.10.0)
project(Holor VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-11")


add_definitions(-DDEFINE_ASSERT_LEVEL=AssertionLevel::release)

#set build path
#first build library to destination

#if build_examples on, compile them
include_directories(./include/)
add_subdirectory(examples)

#if test on, compile tests

#if benchmark on, compile benchmarks

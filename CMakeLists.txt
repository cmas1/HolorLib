cmake_minimum_required(VERSION 3.12)
project("Holor" VERSION 1.0.1
             DESCRIPTION "A C++ header-only template library for multi-dimensional containers."
             HOMEPAGE_URL "https://github.com/cmas1/Holor")


include(CheckCXXCompilerFlag)
include(GNUInstallDirs)
include(CMakeDependentOption)
             


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-11")



add_definitions(-DDEFINE_ASSERT_LEVEL=AssertionLevel::release)


#Library
add_library(holor INTERFACE)
target_include_directories(holor INTERFACE 
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

#set build path
#first build library to destination

#if build_examples on, compile them
include_directories(./include/)
add_subdirectory(examples)

#if test on, compile tests

#if benchmark on, compile benchmarks

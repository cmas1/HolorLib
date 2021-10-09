cmake_minimum_required(VERSION 3.12)

project("Holor_Tests")
enable_testing()

find_package(Holor REQUIRED)
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(include/)

add_executable(test_layout src/test_layout.cpp)
target_link_libraries(test_layout PUBLIC GTest::GTest GTest::Main Holor::Holor)

add_executable(test_holor src/test_holor.cpp)
target_link_libraries(test_holor PUBLIC GTest::GTest GTest::Main Holor::Holor)
# add_test(test-1 test1)
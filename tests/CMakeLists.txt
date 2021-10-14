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

add_executable(test_holor_ref src/test_holor_ref.cpp)
target_link_libraries(test_holor_ref PUBLIC GTest::GTest GTest::Main Holor::Holor)

add_executable(test_comparisons src/test_comparisons.cpp)
target_link_libraries(test_comparisons PUBLIC GTest::GTest GTest::Main Holor::Holor)

add_executable(test_iterators src/test_iterators.cpp)
target_link_libraries(test_iterators PUBLIC GTest::GTest GTest::Main Holor::Holor)

set_target_properties( test_layout test_holor test_holor_ref test_comparisons test_iterators
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)
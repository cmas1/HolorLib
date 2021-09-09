cmake_minimum_required(VERSION 3.12)

project("Holor_Examples")

find_package(Holor REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example_constructors src/example_constructors.cpp)
target_link_libraries(example_constructors PUBLIC Holor::Holor)

set_target_properties( example_constructors
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)
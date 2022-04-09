cmake_minimum_required(VERSION 3.12)

project("Holor_Examples")

find_package(Holor REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example_constructors src/example_constructors.cpp)
target_link_libraries(example_constructors PUBLIC Holor::Holor)

add_executable(example_interface src/example_interface.cpp)
target_link_libraries(example_interface PUBLIC Holor::Holor)

add_executable(example_access src/example_access.cpp)
target_link_libraries(example_access PUBLIC Holor::Holor)

add_executable(example_iterators src/example_iterators.cpp)
target_link_libraries(example_iterators PUBLIC Holor::Holor)

add_executable(example_concepts src/example_concepts.cpp)
target_link_libraries(example_concepts PUBLIC Holor::Holor)

add_executable(example_operations src/example_operations.cpp)
target_link_libraries(example_operations PUBLIC Holor::Holor)

set_target_properties( example_constructors example_interface example_access example_iterators example_concepts example_operations
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)
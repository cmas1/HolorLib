cmake_minimum_required(VERSION 3.12)

project("Holor_Pacakge_Test")

find_package(Holor REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_package ./src/test_package.cpp)
target_link_libraries(test_package PUBLIC Holor::Holor)

set_target_properties(test_package
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)
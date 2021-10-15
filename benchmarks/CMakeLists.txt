cmake_minimum_required(VERSION 3.12)

project("Holor_Benchmarkss")

find_package(Holor REQUIRED)
find_package(benchmark REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


add_executable(bm_holor src/bm_holor.cpp)
target_link_libraries(bm_holor benchmark::benchmark Holor::Holor)

add_executable(bm_holor_ref src/bm_holor_ref.cpp)
target_link_libraries(bm_holor_ref benchmark::benchmark Holor::Holor)

add_executable(bm_layout src/bm_layout.cpp)
target_link_libraries(bm_layout benchmark::benchmark Holor::Holor)

set_target_properties( bm_holor bm_holor_ref bm_layout
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/benchmarks"
)
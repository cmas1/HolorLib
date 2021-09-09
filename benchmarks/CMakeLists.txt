find_package(benchmark REQUIRED)

add_executable(bm_slice src/bm_slice.cpp)
target_link_libraries(bm_slice benchmark::benchmark)


set_target_properties( layout_example holor_example iterator_example
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/benchmarks"
)
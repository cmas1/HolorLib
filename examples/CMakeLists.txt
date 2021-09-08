add_executable(layout_example src/layout_example.cpp)
add_executable(holor_example src/holor_example.cpp)
add_executable(iterator_example src/iterator_example.cpp)

set_target_properties( layout_example holor_example iterator_example
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)
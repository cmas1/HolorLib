#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(benchmark REQUIRED)

add_executable(bm_polynomial src/bm_polynomial.cpp)
target_link_libraries(bm_polynomial benchmark::benchmark)

add_executable(bm_static_knot_sequence src/bm_static_knot_sequence.cpp)
target_link_libraries(bm_static_knot_sequence benchmark::benchmark)

add_executable(bm_dynamic_knot_sequence src/bm_dynamic_knot_sequence.cpp)
target_link_libraries(bm_dynamic_knot_sequence benchmark::benchmark)

#add_executable(bm_bases src/bm_bases.cpp)
#target_link_libraries(bm_bases benchmark::benchmark)
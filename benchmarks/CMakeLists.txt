#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(benchmark REQUIRED)

add_executable(bm_slice src/bm_slice.cpp)
target_link_libraries(bm_slice benchmark::benchmark)
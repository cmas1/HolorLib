#include <benchmark/benchmark.h>
#include "../../include/holor/slice.h"



/*=============================================================================
 ====================           CONSTRUCTOR             =======================
 ============================================================================*/

// Benchmark default constructor
static void BM_DefaultConstructor(benchmark::State& state) {
    for (auto _ : state){
        holor::Slice<2> my_slice;
    }
}
BENCHMARK(BM_DefaultConstructor);





// Benchmark Constructor from lvalue vector
static void BM_RangeCopyConstructor(benchmark::State& state) {
    for (auto _ : state){
        std::array<size_t,10> ref{1,2,3,4,5,6,7,8,9,10};
        holor::Slice<10>(0, ref);
    }
}
BENCHMARK(BM_RangeCopyConstructor);





// Benchmark Constructor from lvalue vector
static void BM_RangeMoveConstructor(benchmark::State& state) {
    for (auto _ : state){
        holor::Slice<10>(0, std::array<size_t,10>{1,2,3,4,5,6,7,8,9,10});
    }
}
BENCHMARK(BM_RangeMoveConstructor);


    // std::cout << "average elapsed time = " << utils::elapsed_time_ms(100000, ([&holor2D](){holor2D(1,1);})) << "ms" << std::endl;
    // std::cout << "average elapsed time = " << utils::elapsed_time_ns(100000, ([&holor2D](){holor2D(1,1);})) << "ns" << std::endl;

BENCHMARK_MAIN();
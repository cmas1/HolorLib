#include <benchmark/benchmark.h>
#include "../../include/polynomials/polynomial_core.h"
#include "../../include/knots_sequence/dynamic_knot_sequence.h"

using namespace bst;

/*=============================================================================
 ====================           CONSTRUCTOR             =======================
 ============================================================================*/

// Benchmark default constructor
template<int N, typename Version>
static void BM_DefaultConstructor(benchmark::State& state) {
    for (auto _ : state){
        DynamicKnotSequence<Polyd, Version> k;
    }
}
BENCHMARK_TEMPLATE(BM_DefaultConstructor,10, BoundedBS);
BENCHMARK_TEMPLATE(BM_DefaultConstructor,50, BoundedBS);
BENCHMARK_TEMPLATE(BM_DefaultConstructor,10, PeriodicBS);
BENCHMARK_TEMPLATE(BM_DefaultConstructor,50, PeriodicBS);



// Benchmark array copy constructor
template<int N, typename Version>
static void BM_ArrayCopyConstructor(benchmark::State& state) {
    std::array<double, N> arr;
    for (auto _ : state){
        DynamicKnotSequence<Polyd, Version> k(arr);
    }
}
BENCHMARK_TEMPLATE(BM_ArrayCopyConstructor,10, BoundedBS);
BENCHMARK_TEMPLATE(BM_ArrayCopyConstructor,50, BoundedBS);
BENCHMARK_TEMPLATE(BM_ArrayCopyConstructor,10, PeriodicBS);
BENCHMARK_TEMPLATE(BM_ArrayCopyConstructor,50, PeriodicBS);


// Benchmark array move constructor
template<int N, typename Version>
static void BM_ArrayMoveConstructor(benchmark::State& state) {
    std::array<double, N> arr;
    for (auto _ : state){
        DynamicKnotSequence<Polyd, Version> k(std::move(arr));
    }
}
BENCHMARK_TEMPLATE(BM_ArrayMoveConstructor,10, BoundedBS);
BENCHMARK_TEMPLATE(BM_ArrayMoveConstructor,50, BoundedBS);
BENCHMARK_TEMPLATE(BM_ArrayMoveConstructor,10, PeriodicBS);
BENCHMARK_TEMPLATE(BM_ArrayMoveConstructor,50, PeriodicBS);




// Benchmark vector copy constructor
template<int N, typename Version>
static void BM_VectorCopyConstructor(benchmark::State& state) {
    std::vector<double> vec(N, 1.0);
    for (auto _ : state){
        DynamicKnotSequence<Polyd, Version> k(vec);
    }
}
BENCHMARK_TEMPLATE(BM_VectorCopyConstructor,10, BoundedBS);
BENCHMARK_TEMPLATE(BM_VectorCopyConstructor,50, BoundedBS);
BENCHMARK_TEMPLATE(BM_VectorCopyConstructor,10, PeriodicBS);
BENCHMARK_TEMPLATE(BM_VectorCopyConstructor,50, PeriodicBS);


// Benchmark array move constructor
template<int N, typename Version>
static void BM_VectorMoveConstructor(benchmark::State& state) {
    std::vector<double> vec(N,1.0);
    for (auto _ : state){
        DynamicKnotSequence<Polyd, Version> k(std::move(vec));
    }
}
BENCHMARK_TEMPLATE(BM_VectorMoveConstructor,10, BoundedBS);
BENCHMARK_TEMPLATE(BM_VectorMoveConstructor,50, BoundedBS);
BENCHMARK_TEMPLATE(BM_VectorMoveConstructor,10, PeriodicBS);
BENCHMARK_TEMPLATE(BM_VectorMoveConstructor,50, PeriodicBS);





// Benchmark copy constructor
template<int N, typename Version>
static void BM_CopyConstructor(benchmark::State& state) {
    std::vector<double> vec(N, 1.0);
    DynamicKnotSequence<Polyd, Version> k_original;
    for (auto _ : state){
        DynamicKnotSequence<Polyd, Version> k_copy(k_original);
    }
}
BENCHMARK_TEMPLATE(BM_CopyConstructor,10, BoundedBS);
BENCHMARK_TEMPLATE(BM_CopyConstructor,50, BoundedBS);
BENCHMARK_TEMPLATE(BM_CopyConstructor,10, PeriodicBS);
BENCHMARK_TEMPLATE(BM_CopyConstructor,50, PeriodicBS);


// Benchmark move constructor
template<int N, typename Version>
static void BM_MoveConstructor(benchmark::State& state) {
    std::vector<double> vec(N, 1.0);
    DynamicKnotSequence<Polyd, Version> k_original;
    for (auto _ : state){
        DynamicKnotSequence<Polyd, Version> k_copy(std::move(k_original));
    }
}
BENCHMARK_TEMPLATE(BM_MoveConstructor,10, BoundedBS);
BENCHMARK_TEMPLATE(BM_MoveConstructor,50, BoundedBS);
BENCHMARK_TEMPLATE(BM_MoveConstructor,10, PeriodicBS);
BENCHMARK_TEMPLATE(BM_MoveConstructor,50, PeriodicBS);




/*=============================================================================
 ====================           Operations           =======================
 ============================================================================*/
// Benchmark find interval
template<typename Version>
static void BM_FindInterval(benchmark::State& state) {
    std::vector<double> vec;
    vec.resize(101);
    std::iota(vec.begin(), vec.end(), 0);
    DynamicKnotSequence<Polyd, Version> k(vec);
    for (auto _ : state){
        k.find_interval(state.range(0));
    }
}
BENCHMARK_TEMPLATE(BM_FindInterval,BoundedBS)->DenseRange(0, 100, 20);
BENCHMARK_TEMPLATE(BM_FindInterval,PeriodicBS)->DenseRange(0, 100, 20);


// Benchmark Shift
template<typename Version>
static void BM_Shift(benchmark::State& state) {
    std::vector<double> vec;
    vec.resize(30);
    std::iota(vec.begin(), vec.end(), 0);
    DynamicKnotSequence<Polyd, Version> k(vec);
    for (auto _ : state){
        k.shift(10, state.range(0));
    }
}
BENCHMARK_TEMPLATE(BM_Shift,BoundedBS)->DenseRange(-30, 30, 10);
BENCHMARK_TEMPLATE(BM_Shift,PeriodicBS)->DenseRange(-30, 30, 10);



// Forward Length
template<typename Version>
static void BM_FwdLength(benchmark::State& state) {
    std::vector<double> vec;
    vec.resize(50);
    std::iota(vec.begin(), vec.end(), 0);
    DynamicKnotSequence<Polyd, Version> k(vec);
    auto knot1 = k.knot(25);
    auto knot2 = k.knot(k.shift(30, 40));
    for (auto _ : state){
        k.forward_length(knot1, knot2);
    }
}
BENCHMARK_TEMPLATE(BM_FwdLength,BoundedBS);
BENCHMARK_TEMPLATE(BM_FwdLength,PeriodicBS);


BENCHMARK_MAIN();
#include <benchmark/benchmark.h>
#include "../../include/control_points/point.h"

using namespace bst;

/*=============================================================================
 ====================           CONSTRUCTOR             =======================
 ============================================================================*/

// Benchmark default constructor
template<size_t N>
static void BM_DefaultConstructor(benchmark::State& state) {
    for (auto _ : state){
        Point<double, N> point;
    }
}
BENCHMARK_TEMPLATE(BM_DefaultConstructor,1);
BENCHMARK_TEMPLATE(BM_DefaultConstructor,2);
BENCHMARK_TEMPLATE(BM_DefaultConstructor,3);
BENCHMARK_TEMPLATE(BM_DefaultConstructor,10);



// Benchmark value constructor
static void BM_ValueConstructor1(benchmark::State& state) {
    for (auto _ : state){
        Point<double, 1> point(1.0);
    }
}
BENCHMARK(BM_ValueConstructor1);

static void BM_ValueConstructor3(benchmark::State& state) {
    for (auto _ : state){
        Point<double, 3> point{1.0, 2.0, 3.0};
    }
}
BENCHMARK(BM_ValueConstructor3);

static void BM_ValueConstructor5(benchmark::State& state) {
    for (auto _ : state){
        Point<double, 5> point{1.0, 2.0, 3.0, 4.0, 5.0};
    }
}
BENCHMARK(BM_ValueConstructor5);


// Benchmark scalar multiplication
template<size_t N>
static void BM_ScalarMultiplication(benchmark::State& state) {
    Point<double, N> point;
    for (auto _ : state){
        benchmark::DoNotOptimize( point*=1.1);
    }
}
BENCHMARK_TEMPLATE(BM_ScalarMultiplication,1);
BENCHMARK_TEMPLATE(BM_ScalarMultiplication,2);
BENCHMARK_TEMPLATE(BM_ScalarMultiplication,3);
BENCHMARK_TEMPLATE(BM_ScalarMultiplication,4);
BENCHMARK_TEMPLATE(BM_ScalarMultiplication,5);
BENCHMARK_TEMPLATE(BM_ScalarMultiplication,6);
BENCHMARK_TEMPLATE(BM_ScalarMultiplication,7);
BENCHMARK_TEMPLATE(BM_ScalarMultiplication,8);
BENCHMARK_TEMPLATE(BM_ScalarMultiplication,9);
BENCHMARK_TEMPLATE(BM_ScalarMultiplication,10);


// Benchmark scalar multiplication
template<size_t N>
static void BM_ScalarMultiplicationExt(benchmark::State& state) {
    Point<double, N> point;
    Point<double, N> point_new;
    for (auto _ : state){
        benchmark::DoNotOptimize( point_new = point*1.1);
    }
}
BENCHMARK_TEMPLATE(BM_ScalarMultiplicationExt,1);
BENCHMARK_TEMPLATE(BM_ScalarMultiplicationExt,2);
BENCHMARK_TEMPLATE(BM_ScalarMultiplicationExt,3);
BENCHMARK_TEMPLATE(BM_ScalarMultiplicationExt,4);
BENCHMARK_TEMPLATE(BM_ScalarMultiplicationExt,5);
BENCHMARK_TEMPLATE(BM_ScalarMultiplicationExt,6);
BENCHMARK_TEMPLATE(BM_ScalarMultiplicationExt,7);
BENCHMARK_TEMPLATE(BM_ScalarMultiplicationExt,8);
BENCHMARK_TEMPLATE(BM_ScalarMultiplicationExt,9);
BENCHMARK_TEMPLATE(BM_ScalarMultiplicationExt,10);




// Benchmark point sum
template<size_t N>
static void BM_PointSum(benchmark::State& state) {
    Point<double, N> point1;
    Point<double, N> point2;
    for (auto _ : state){
        benchmark::DoNotOptimize(point1 + point2);
    }
}
BENCHMARK_TEMPLATE(BM_PointSum,1);
BENCHMARK_TEMPLATE(BM_PointSum,2);
BENCHMARK_TEMPLATE(BM_PointSum,3);
BENCHMARK_TEMPLATE(BM_PointSum,4);
BENCHMARK_TEMPLATE(BM_PointSum,5);
BENCHMARK_TEMPLATE(BM_PointSum,6);
BENCHMARK_TEMPLATE(BM_PointSum,7);
BENCHMARK_TEMPLATE(BM_PointSum,8);
BENCHMARK_TEMPLATE(BM_PointSum,9);
BENCHMARK_TEMPLATE(BM_PointSum,10);


BENCHMARK_MAIN();


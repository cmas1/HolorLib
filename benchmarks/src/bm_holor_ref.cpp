// This file is part of Holor, a C++ template library for multi-dimensional containers

// Copyright 2020-2021 Carlo Masone

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or suholorantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.


#include <benchmark/benchmark.h>
#include <holor/holor_full.h>

#include <string>

using namespace holor;

/*=============================================================================
 ====================           CONSTRUCTORS            =======================
 ============================================================================*/
template<size_t N>
static void BM_DefaultConstructor(benchmark::State& state) {
    for (auto _ : state){
        HolorRef<int, N> h;
    }
}
BENCHMARK_TEMPLATE(BM_DefaultConstructor,1);
BENCHMARK_TEMPLATE(BM_DefaultConstructor,3);
BENCHMARK_TEMPLATE(BM_DefaultConstructor,5);



static void BM_LayoutConstructor1D(benchmark::State& state) {
    std::vector<int> vec(8,2);
    for (auto _ : state){
        HolorRef<int, 1> h(vec.data(), Layout<1>{8});
    }
}
BENCHMARK(BM_LayoutConstructor1D);


static void BM_LayoutConstructor2D(benchmark::State& state) {
    std::vector<int> vec(8,2);
    for (auto _ : state){
        HolorRef<int, 2> h(vec.data(), Layout<2>{2,4});
    }
}
BENCHMARK(BM_LayoutConstructor2D);

static void BM_LayoutConstructor3D(benchmark::State& state) {
    std::vector<int> vec(8,2);
    for (auto _ : state){
        HolorRef<int, 3> h(vec.data(), Layout<3>{2,2,2});
    }
}
BENCHMARK(BM_LayoutConstructor3D);


static void BM_ResizeableLengthConstructor1D(benchmark::State& state) {
    std::vector<int> vec(8,2);
    for (auto _ : state){
        HolorRef<int, 1> h(vec.data(), std::vector<size_t>{8});
    }
}
BENCHMARK(BM_ResizeableLengthConstructor1D);


static void BM_ResizeableLengthConstructor2D(benchmark::State& state) {
    std::vector<int> vec(8,2);
    for (auto _ : state){
        HolorRef<int, 2> h(vec.data(), std::vector<size_t>{2,4});
    }
}
BENCHMARK(BM_ResizeableLengthConstructor2D);

static void BM_ResizeableLengthConstructor3D(benchmark::State& state) {
    std::vector<int> vec(8,2);
    for (auto _ : state){
        HolorRef<int, 3> h(vec.data(), std::vector<size_t>{2,2,2});
    }
}
BENCHMARK(BM_ResizeableLengthConstructor3D);


static void BM_SizedLengthConstructor1D(benchmark::State& state) {
    std::vector<int> vec(8,2);
    for (auto _ : state){
        HolorRef<int, 1> h(vec.data(), std::array<size_t, 1>{8});
    }
}
BENCHMARK(BM_SizedLengthConstructor1D);


static void BM_SizedLengthConstructor2D(benchmark::State& state) {
    std::vector<int> vec(8,2);
    for (auto _ : state){
        HolorRef<int, 2> h(vec.data(), std::array<size_t, 2>{2,4});
    }
}
BENCHMARK(BM_SizedLengthConstructor2D);

static void BM_SizedLengthConstructor3D(benchmark::State& state) {
    std::vector<int> vec(8,2);
    for (auto _ : state){
        HolorRef<int, 3> h(vec.data(), std::array<size_t, 3>{2,2,2});
    }
}
BENCHMARK(BM_SizedLengthConstructor3D);



/*=============================================================================
 ====================           INDEXING                =======================
 ============================================================================*/
static void BM_HolorRefIndexing1D(benchmark::State& state) {
    std::vector<int> vec(8,2);
    HolorRef<int, 1> h(vec.data(), std::vector<size_t>{8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0)));
    }
}
BENCHMARK(BM_HolorRefIndexing1D)->Arg(0)->Arg(7);

static void BM_HolorRefIndexing2D(benchmark::State& state) {
    std::vector<int> vec(8^2,2);
    HolorRef<int, 2> h(vec.data(), std::vector<size_t>{8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0), state.range(1)));
    }
}
BENCHMARK(BM_HolorRefIndexing2D)->Args({0,0})->Args({7,7});


static void BM_HolorRefIndexing3D(benchmark::State& state) {
    std::vector<int> vec(8^3,2);
    HolorRef<int, 3> h(vec.data(), std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0), state.range(1), state.range(2)));
    }
}
BENCHMARK(BM_HolorRefIndexing3D)->Args({0,0,0})->Args({7,7,7});


static void BM_HolorRefIndexing4D(benchmark::State& state) {
    std::vector<int> vec(8^4,2);
    HolorRef<int, 4> h(vec.data(), std::vector<size_t>{8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0), state.range(1), state.range(2), state.range(3)));
    }
}
BENCHMARK(BM_HolorRefIndexing4D)->Args({0,0,0,0})->Args({7,7,7,7});


static void BM_HolorRefIndexing5D(benchmark::State& state) {
    std::vector<int> vec(8^5,2);
    HolorRef<int, 5> h(vec.data(), std::vector<size_t>{8,8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0), state.range(1), state.range(2), state.range(3), state.range(4)));
    }
}
BENCHMARK(BM_HolorRefIndexing5D)->Args({0,0,0,0,0})->Args({7,7,7,7,7});


/*=============================================================================
 ====================           SLICING                =======================
 ============================================================================*/
static void BM_HolorRefSlicing2D_1(benchmark::State& state) {
    std::vector<int> vec(8^2,2);
    HolorRef<int, 2> h(vec.data(), std::vector<size_t>{8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), 4));
    }
}
BENCHMARK(BM_HolorRefSlicing2D_1);


static void BM_HolorRefSlicing2D_2(benchmark::State& state) {
    std::vector<int> vec(8^2,2);
    HolorRef<int, 2> h(vec.data(), std::vector<size_t>{8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6)));
    }
}
BENCHMARK(BM_HolorRefSlicing2D_2);

static void BM_HolorRefSlicing3D_1(benchmark::State& state) {
    std::vector<int> vec(8^3,2);
    HolorRef<int, 3> h(vec.data(), std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), 4, 4));
    }
}
BENCHMARK(BM_HolorRefSlicing3D_1);


static void BM_HolorRefSlicing3D_2(benchmark::State& state) {
    std::vector<int> vec(8^3,2);
    HolorRef<int, 3> h(vec.data(), std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6), 4));
    }
}
BENCHMARK(BM_HolorRefSlicing3D_2);

static void BM_HolorRefSlicing3D_3(benchmark::State& state) {
    std::vector<int> vec(8^3,2);
    HolorRef<int, 3> h(vec.data(), std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6), range(1,5)));
    }
}
BENCHMARK(BM_HolorRefSlicing3D_3);


static void BM_HolorRefSlicing4D_1(benchmark::State& state) {
    Holor<int, 4> h(std::vector<size_t>{8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), 4, 4, 4));
    }
}
BENCHMARK(BM_HolorRefSlicing4D_1);


static void BM_HolorRefSlicing4D_2(benchmark::State& state) {
    std::vector<int> vec(8^4,2);
    HolorRef<int, 4> h(vec.data(), std::vector<size_t>{8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6), 4, 4));
    }
}
BENCHMARK(BM_HolorRefSlicing4D_2);

static void BM_HolorRefSlicing4D_3(benchmark::State& state) {
    std::vector<int> vec(8^4,2);
    HolorRef<int, 4> h(vec.data(), std::vector<size_t>{8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6), range(1,5), 4));
    }
}
BENCHMARK(BM_HolorRefSlicing4D_3);


static void BM_RowSlicing(benchmark::State& state) {
    std::vector<int> vec(8^3,2);
    HolorRef<int, 3> h(vec.data(), std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h.row(3));
    }
}
BENCHMARK(BM_RowSlicing);

static void BM_ColSlicing(benchmark::State& state) {
    std::vector<int> vec(8^3,2);
    HolorRef<int, 3> h(vec.data(), std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h.col(3));
    }
}
BENCHMARK(BM_ColSlicing);

static void BM_DimSlicing(benchmark::State& state) {
    std::vector<int> vec(8^3,2);
    HolorRef<int, 3> h(vec.data(), std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h.slice<2>(3));
    }
}
BENCHMARK(BM_DimSlicing);



BENCHMARK_MAIN();
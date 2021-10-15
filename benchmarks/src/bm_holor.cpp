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



using namespace holor;

/*=============================================================================
 ====================           CONSTRUCTORS            =======================
 ============================================================================*/
static void BM_HolorNestedConstructor(benchmark::State& state) {
    for (auto _ : state){
        Holor<int, 3> h{ {{1,2,3}, {4,5,6}, {7,8,9}}, {{10,11,12}, {13,14,15}, {16,17,18}}, {{19,20,21}, {22,23,24}, {25,26,27}} };
    }
}
BENCHMARK(BM_HolorNestedConstructor);


template<size_t N>
static void BM_ResizeableLenghtsConstructor(benchmark::State& state) {
    std::vector<int> vec(N,2);
    for (auto _ : state){
        Holor<int, N> h(vec);
    }
}
BENCHMARK_TEMPLATE(BM_ResizeableLenghtsConstructor,1);
BENCHMARK_TEMPLATE(BM_ResizeableLenghtsConstructor,3);
BENCHMARK_TEMPLATE(BM_ResizeableLenghtsConstructor,5);
BENCHMARK_TEMPLATE(BM_ResizeableLenghtsConstructor,7);


template<size_t N>
static void BM_SizedLenghtsConstructor(benchmark::State& state) {
    std::array<size_t,N> arr;
    arr.fill(2);
    for (auto _ : state){
        Holor<int, N> h(arr);
    }
}
BENCHMARK_TEMPLATE(BM_SizedLenghtsConstructor,1);
BENCHMARK_TEMPLATE(BM_SizedLenghtsConstructor,3);
BENCHMARK_TEMPLATE(BM_SizedLenghtsConstructor,5);
BENCHMARK_TEMPLATE(BM_SizedLenghtsConstructor,7);



template<size_t N>
static void BM_CopyConstructor(benchmark::State& state) {
    std::array<size_t,N> arr;
    arr.fill(2);
    for (auto _ : state){
        Holor<int, N> h(arr);
        Holor<int, N>h2(h);
    }
}
BENCHMARK_TEMPLATE(BM_CopyConstructor,1);
BENCHMARK_TEMPLATE(BM_CopyConstructor,3);
BENCHMARK_TEMPLATE(BM_CopyConstructor,5);
BENCHMARK_TEMPLATE(BM_CopyConstructor,7);


template<size_t N>
static void BM_MoveConstructor(benchmark::State& state) {
    std::array<size_t,N> arr;
    arr.fill(2);
    for (auto _ : state){
        Holor<int, N>h2( (Holor<int, N>(arr)) );
    }
}
BENCHMARK_TEMPLATE(BM_MoveConstructor,1);
BENCHMARK_TEMPLATE(BM_MoveConstructor,3);
BENCHMARK_TEMPLATE(BM_MoveConstructor,5);
BENCHMARK_TEMPLATE(BM_MoveConstructor,7);
/*=============================================================================
 ====================           INDEXING                =======================
 ============================================================================*/
static void BM_HolorIndexing1D(benchmark::State& state) {
    Holor<int, 1> h(std::vector<size_t>{8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0)));
    }
}
BENCHMARK(BM_HolorIndexing1D)->Arg(0)->Arg(7);

static void BM_HolorIndexing2D(benchmark::State& state) {
    Holor<int, 2> h(std::vector<size_t>{8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0), state.range(1)));
    }
}
BENCHMARK(BM_HolorIndexing2D)->Args({0,0})->Args({7,7});


static void BM_HolorIndexing3D(benchmark::State& state) {
    Holor<int, 3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0), state.range(1), state.range(2)));
    }
}
BENCHMARK(BM_HolorIndexing3D)->Args({0,0,0})->Args({7,7,7});


static void BM_HolorIndexing4D(benchmark::State& state) {
    Holor<int, 4> h(std::vector<size_t>{8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0), state.range(1), state.range(2), state.range(3)));
    }
}
BENCHMARK(BM_HolorIndexing4D)->Args({0,0,0,0})->Args({7,7,7,7});


static void BM_HolorIndexing5D(benchmark::State& state) {
    Holor<int, 5> h(std::vector<size_t>{8,8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0), state.range(1), state.range(2), state.range(3), state.range(4)));
    }
}
BENCHMARK(BM_HolorIndexing5D)->Args({0,0,0,0,0})->Args({7,7,7,7,7});


/*=============================================================================
 ====================           SLICING                =======================
 ============================================================================*/
static void BM_HolorSlicing2D_1(benchmark::State& state) {
    Holor<int, 2> h(std::vector<size_t>{8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), 4));
    }
}
BENCHMARK(BM_HolorSlicing2D_1);


static void BM_HolorSlicing2D_2(benchmark::State& state) {
    Holor<int, 2> h(std::vector<size_t>{8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6)));
    }
}
BENCHMARK(BM_HolorSlicing2D_2);

static void BM_HolorSlicing3D_1(benchmark::State& state) {
    Holor<int, 3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), 4, 4));
    }
}
BENCHMARK(BM_HolorSlicing3D_1);


static void BM_HolorSlicing3D_2(benchmark::State& state) {
    Holor<int, 3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6), 4));
    }
}
BENCHMARK(BM_HolorSlicing3D_2);

static void BM_HolorSlicing3D_3(benchmark::State& state) {
    Holor<int, 3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6), range(1,5)));
    }
}
BENCHMARK(BM_HolorSlicing3D_3);


static void BM_HolorSlicing4D_1(benchmark::State& state) {
    Holor<int, 4> h(std::vector<size_t>{8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), 4, 4, 4));
    }
}
BENCHMARK(BM_HolorSlicing4D_1);


static void BM_HolorSlicing4D_2(benchmark::State& state) {
    Holor<int, 4> h(std::vector<size_t>{8,8,8, 8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6), 4, 4));
    }
}
BENCHMARK(BM_HolorSlicing4D_2);

static void BM_HolorSlicing4D_3(benchmark::State& state) {
    Holor<int, 4> h(std::vector<size_t>{8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6), range(1,5), 4));
    }
}
BENCHMARK(BM_HolorSlicing4D_3);


static void BM_RowSlicing(benchmark::State& state) {
    Holor<int, 3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h.row(3));
    }
}
BENCHMARK(BM_RowSlicing);

static void BM_ColSlicing(benchmark::State& state) {
    Holor<int, 3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h.col(3));
    }
}
BENCHMARK(BM_ColSlicing);

static void BM_DimSlicing(benchmark::State& state) {
    Holor<int, 3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h.slice<2>(3));
    }
}
BENCHMARK(BM_DimSlicing);

BENCHMARK_MAIN();
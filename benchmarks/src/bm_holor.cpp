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

BENCHMARK_MAIN();
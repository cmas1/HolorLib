// This file is part of Holor, a C++ header-only template library for multi-dimensional containers

// Copyright 2020-2022 Carlo Masone

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

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
template<size_t N>
static void BM_LayoutDefaultConstructor(benchmark::State& state) {
    for (auto _ : state){
        Layout<N> layout;
    }
}
BENCHMARK_TEMPLATE(BM_LayoutDefaultConstructor,1);
BENCHMARK_TEMPLATE(BM_LayoutDefaultConstructor,5);
BENCHMARK_TEMPLATE(BM_LayoutDefaultConstructor,10);


template<size_t N>
static void BM_ResizeableLenghtsConstructor(benchmark::State& state) {
    std::vector<int> vec(N,2);
    for (auto _ : state){
        Layout<N> h(vec);
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
        Layout<N> h(arr);
    }
}
BENCHMARK_TEMPLATE(BM_SizedLenghtsConstructor,1);
BENCHMARK_TEMPLATE(BM_SizedLenghtsConstructor,3);
BENCHMARK_TEMPLATE(BM_SizedLenghtsConstructor,5);
BENCHMARK_TEMPLATE(BM_SizedLenghtsConstructor,7);



static void BM_LayoutListConstructor1D(benchmark::State& state) {
    for (auto _ : state){
        Layout<1> h(2);
    }
}
BENCHMARK(BM_LayoutListConstructor1D);

static void BM_LayoutListConstructor2D(benchmark::State& state) {
    for (auto _ : state){
        Layout<2> h(2,2);
    }
}
BENCHMARK(BM_LayoutListConstructor2D);

static void BM_LayoutListConstructor3D(benchmark::State& state) {
    for (auto _ : state){
        Layout<3> h(2, 2, 2);
    }
}
BENCHMARK(BM_LayoutListConstructor3D);

static void BM_LayoutListConstructor4D(benchmark::State& state) {
    for (auto _ : state){
        Layout<4> h{2, 2, 2, 2};
    }
}
BENCHMARK(BM_LayoutListConstructor4D);



/*=============================================================================
 ====================           INDEXING                =======================
 ============================================================================*/
static void BM_LayoutIndexing1D(benchmark::State& state) {
    Layout<1> h(std::vector<size_t>{8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0)));
    }
}
BENCHMARK(BM_LayoutIndexing1D)->Arg(0)->Arg(7);

static void BM_LayoutIndexing2D(benchmark::State& state) {
    Layout<2> h(std::vector<size_t>{8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0), state.range(1)));
    }
}
BENCHMARK(BM_LayoutIndexing2D)->Args({0,0})->Args({7,7});


static void BM_LayoutIndexing3D(benchmark::State& state) {
    Layout<3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0), state.range(1), state.range(2)));
    }
}
BENCHMARK(BM_LayoutIndexing3D)->Args({0,0,0})->Args({7,7,7});


static void BM_LayoutIndexing4D(benchmark::State& state) {
    Layout<4> h(std::vector<size_t>{8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0), state.range(1), state.range(2), state.range(3)));
    }
}
BENCHMARK(BM_LayoutIndexing4D)->Args({0,0,0,0})->Args({7,7,7,7});


static void BM_LayoutIndexing5D(benchmark::State& state) {
    Layout<5> h(std::vector<size_t>{8,8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(state.range(0), state.range(1), state.range(2), state.range(3), state.range(4)));
    }
}
BENCHMARK(BM_LayoutIndexing5D)->Args({0,0,0,0,0})->Args({7,7,7,7,7});


/*=============================================================================
 ====================           SLICING                =======================
 ============================================================================*/
static void BM_LayoutSlicing2D_1(benchmark::State& state) {
    Layout<2> h(std::vector<size_t>{8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), 4));
    }
}
BENCHMARK(BM_LayoutSlicing2D_1);


static void BM_LayoutSlicing2D_2(benchmark::State& state) {
    Layout<2> h(std::vector<size_t>{8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6)));
    }
}
BENCHMARK(BM_LayoutSlicing2D_2);

static void BM_LayoutSlicing3D_1(benchmark::State& state) {
    Layout<3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), 4, 4));
    }
}
BENCHMARK(BM_LayoutSlicing3D_1);


static void BM_LayoutSlicing3D_2(benchmark::State& state) {
    Layout<3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6), 4));
    }
}
BENCHMARK(BM_LayoutSlicing3D_2);

static void BM_LayoutSlicing3D_3(benchmark::State& state) {
    Layout<3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6), range(1,5)));
    }
}
BENCHMARK(BM_LayoutSlicing3D_3);


static void BM_LayoutSlicing4D_1(benchmark::State& state) {
    Layout<4> h(std::vector<size_t>{8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), 4, 4, 4));
    }
}
BENCHMARK(BM_LayoutSlicing4D_1);


static void BM_LayoutSlicing4D_2(benchmark::State& state) {
    Layout<4> h(std::vector<size_t>{8,8,8, 8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6), 4, 4));
    }
}
BENCHMARK(BM_LayoutSlicing4D_2);

static void BM_LayoutSlicing4D_3(benchmark::State& state) {
    Layout<4> h(std::vector<size_t>{8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h(range(3,7), range(2,6), range(1,5), 4));
    }
}
BENCHMARK(BM_LayoutSlicing4D_3);


static void BM_LayoutSlicingUnreduced4D_3(benchmark::State& state) {
    Layout<4> h(std::vector<size_t>{8,8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h.slice_unreduced(range(3,7), range(2,6), range(1,5), 4));
    }
}
BENCHMARK(BM_LayoutSlicingUnreduced4D_3);


static void BM_DimIndexSlicing(benchmark::State& state) {
    Layout<3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h.slice_dimension<2>(3));
    }
}
BENCHMARK(BM_DimIndexSlicing);

static void BM_DimRangeSlicing(benchmark::State& state) {
    Layout<3> h(std::vector<size_t>{8,8,8});
    for (auto _ : state){
        benchmark::DoNotOptimize(h.slice_dimension<2>(range(3,6)));
    }
}
BENCHMARK(BM_DimRangeSlicing);

BENCHMARK_MAIN();
#include <benchmark/benchmark.h>
#include "../../include/knots_sequence/static_knot_sequence.h"
#include "../../include/polynomials/polynomial_core.h"
#include "../../include/bspline/bs_basis.h"


using namespace bst;

/*=============================================================================
 ====================           CONSTRUCTOR             =======================
 ============================================================================*/
// Benchmark default constructor
static void BM_BoundedConstructor(benchmark::State& state) {
    StaticKnotSequence<Polyd, BoundedBS, 4>ks{0, 0.3, 0.6, 1.0};
    for (auto _ : state){
        BSBasis<decltype(ks), 2> test_base(ks);
    }
}
BENCHMARK(BM_BoundedConstructor);

static void BM_PeriodicConstructor(benchmark::State& state) {
    StaticKnotSequence<Polyd, PeriodicBS, 4>ks{0, 0.3, 0.6, 1.0};
    for (auto _ : state){
        BSBasis<decltype(ks), 2> test_base(ks);
    }
}
BENCHMARK(BM_PeriodicConstructor);


/*=============================================================================
 ====================           EVALUATION             =======================
 ============================================================================*/
static void BM_BoundedEval(benchmark::State& state) {
    StaticKnotSequence<Polyd, BoundedBS, 4>ks{0, 0.3, 0.6, 1.0};
    BSBasis<decltype(ks), 2> test_base(ks);
    for (auto _ : state){
        benchmark::DoNotOptimize(test_base.eval_active(state.range(0)));
    }
}
BENCHMARK(BM_BoundedEval)->Arg(0)->Arg(0.2)->Arg(0.4)->Arg(0.6)->Arg(0.8)->Arg(1);

static void BM_PeriodicEval(benchmark::State& state) {
    StaticKnotSequence<Polyd, PeriodicBS, 4>ks{0, 0.3, 0.6, 1.0};
    BSBasis<decltype(ks), 2> test_base(ks);
    for (auto _ : state){
        benchmark::DoNotOptimize(test_base.eval_active(state.range(0)));
    }
}
BENCHMARK(BM_PeriodicEval)->Arg(0)->Arg(0.2)->Arg(0.4)->Arg(0.6)->Arg(0.8)->Arg(1);



BENCHMARK_MAIN();
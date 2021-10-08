#include <benchmark/benchmark.h>
#include "../../include/polynomials/polynomial.h"



/*=============================================================================
 ====================           CONSTRUCTOR             =======================
 ============================================================================*/

// Benchmark default constructor
static void BM_DefaultConstructor(benchmark::State& state) {
    for (auto _ : state){
        bst::Polynomial<double> poly;
    }
}
BENCHMARK(BM_DefaultConstructor);





// Benchmark Constructor from lvalue vector
static void BM_VectorCopyConstructor(benchmark::State& state) {
    std::vector<double> ref;
    for (auto _ : state){
        ref = std::vector<double>(state.range(0), 3.0);
        bst::Polynomial<double> poly(ref);
    }
}
BENCHMARK(BM_VectorCopyConstructor)->DenseRange(1, 151, 50);





// Benchmark Constructor from lvalue vector
static void BM_VectorMoveConstructor(benchmark::State& state) {
    std::vector<double> ref;
    for (auto _ : state){
        ref = std::vector<double>(state.range(0), 3.0);
        bst::Polynomial<double> poly(std::move(ref));
    }
}
BENCHMARK(BM_VectorMoveConstructor)->DenseRange(1, 151, 50);





// Benchmark Degree/Value Constructor
static void BM_DegreeValueConstructor(benchmark::State& state) {
    for (auto _ : state){
        bst::Polynomial<double> poly(state.range(0), 3.0);
    }
}
BENCHMARK(BM_DegreeValueConstructor)->DenseRange(1, 151, 50);






// Benchmark Copy constructor
static void BM_CopyConstructor(benchmark::State& state) {
    bst::Polynomial<double> ref;
    for (auto _ : state){
        ref = bst::Polynomial<double>(state.range(0), -5.0);
        bst::Polynomial<double> poly(ref);
    }
}
BENCHMARK(BM_CopyConstructor)->DenseRange(1, 151, 50);





// Benchmark Move constructor
static void BM_MoveConstructor(benchmark::State& state) {
    bst::Polynomial<double> ref;
    for (auto _ : state){
        ref = bst::Polynomial<double>(state.range(0), -5.0);
        bst::Polynomial<double> poly(std::move(ref));
    }
}
BENCHMARK(BM_MoveConstructor)->DenseRange(1, 151, 50);





/*=============================================================================
 ====================            ASSIGNMENT             =======================
 ============================================================================*/



// Benchmark Copy assignment
static void BM_CopyAssignment(benchmark::State& state) {
    bst::Polynomial<double> ref;
    for (auto _ : state){
        ref = bst::Polynomial<double>(state.range(0), -5.0);
        bst::Polynomial<double> poly = ref;
    }
}
BENCHMARK(BM_CopyAssignment)->DenseRange(1, 151, 50);





// Benchmark Move constructor
static void BM_MoveAssignment(benchmark::State& state) {
    bst::Polynomial<double> ref;
    for (auto _ : state){
        ref = bst::Polynomial<double>(state.range(0), -5.0);
        bst::Polynomial<double> poly = std::move(ref);
    }
}
BENCHMARK(BM_MoveAssignment)->DenseRange(1, 151, 50);




/*=============================================================================
 ====================            EVALUATION             =======================
 ============================================================================*/
// Benchmark Polynomial<double> Evaluation
static void BM_Evaluation(benchmark::State& state) {
    bst::Polynomial<double> poly(state.range(0), -5.0);
    for (auto _ : state){
        benchmark::DoNotOptimize(poly(2.0));
    }
}
BENCHMARK(BM_Evaluation)->DenseRange(1, 10, 1);



// Benchmark Polynomial<int> Evaluation
static void BM_EvaluationInt(benchmark::State& state) {
    bst::Polynomial<int> poly(state.range(0), -5);
    for (auto _ : state){
        benchmark::DoNotOptimize(poly(2));
    }
}
BENCHMARK(BM_EvaluationInt)->DenseRange(1, 10, 1);




/*=============================================================================
 ====================            OPERATIONS             =======================
 ============================================================================*/
// Benchmark += scalar
static void BM_PlusAssignment_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly(state.range(0), -5);
    for (auto _ : state){
        benchmark::DoNotOptimize(poly += 3.0);
    }
}
BENCHMARK(BM_PlusAssignment_ScalarOp)->DenseRange(1, 11, 5);


// Benchmark *= scalar
static void BM_MultiplyAssignment_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly(state.range(0), -5);
    auto val = 1.0/3.0;
    for (auto _ : state){
        benchmark::DoNotOptimize(poly *= val);
    }
}
BENCHMARK(BM_MultiplyAssignment_ScalarOp)->DenseRange(1, 11, 5);


// Benchmark *= 0
static void BM_ZeroMultiplyAssignment_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly(state.range(0), -5);
    for (auto _ : state){
        benchmark::DoNotOptimize(poly *= 0.0);
    }
}
BENCHMARK(BM_ZeroMultiplyAssignment_ScalarOp)->DenseRange(1, 11, 5);





// Benchmark /= scalar
static void BM_DivisionAssignment_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly(state.range(0), -5);
    for (auto _ : state){
        benchmark::DoNotOptimize(poly /= 3.0);
    }
}
BENCHMARK(BM_DivisionAssignment_ScalarOp)->DenseRange(1, 11, 5);




// Benchmark div(scalar not zero)
static void BM_Div_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly(state.range(0), -5);
    for (auto _ : state){
        benchmark::DoNotOptimize(poly.div(3.0));
    }
}
BENCHMARK(BM_Div_ScalarOp)->DenseRange(1, 11, 5);


// Benchmark div(0)
static void BM_ZeroDiv_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly(state.range(0), -5);
    for (auto _ : state){
        benchmark::DoNotOptimize(poly.div(0.0));
    }
}
BENCHMARK(BM_ZeroDiv_ScalarOp)->DenseRange(1, 11, 5);


// Benchmark div_inplace(scalar not zero)
static void BM_DivInplace_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly(state.range(0), -5);
    for (auto _ : state){
        benchmark::DoNotOptimize(poly.div_inplace(3.0));
    }
}
BENCHMARK(BM_DivInplace_ScalarOp)->DenseRange(1, 11, 5);


// Benchmark div_inplace(0)
static void BM_ZeroDivInplace_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly(state.range(0), -5);
    for (auto _ : state){
        benchmark::DoNotOptimize(poly.div_inplace(0.0));
    }
}
BENCHMARK(BM_ZeroDivInplace_ScalarOp)->DenseRange(1, 11, 5);




// Benchmark lvalue + scalar
static void BM_PlusLvalue_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(poly + 3.0);
    }
}
BENCHMARK(BM_PlusLvalue_ScalarOp)->DenseRange(1, 11, 5);


// Benchmark rvalue + scalar
static void BM_PlusRvalue_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(std::move(poly) + 3.0);
    }
}
BENCHMARK(BM_PlusRvalue_ScalarOp)->DenseRange(1, 11, 5);




// Benchmark lvalue * scalar
static void BM_MultiplyLvalue_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(poly * 3.0);
    }
}
BENCHMARK(BM_MultiplyLvalue_ScalarOp)->DenseRange(1, 11, 5);


// Benchmark rvalue * scalar
static void BM_MultiplyRvalue_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(std::move(poly) * 3.0);
    }
}
BENCHMARK(BM_MultiplyRvalue_ScalarOp)->DenseRange(1, 11, 5);




// Benchmark lvalue / scalar
static void BM_DivisionLvalue_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(poly / 3.0);
    }
}
BENCHMARK(BM_DivisionLvalue_ScalarOp)->DenseRange(1, 11, 5);


// Benchmark rvalue / scalar
static void BM_DivisionRvalue_ScalarOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(std::move(poly) / 3.0);
    }
}
BENCHMARK(BM_DivisionRvalue_ScalarOp)->DenseRange(1, 11, 5);


/*=============================================================================
 ====================       POLYNOMIAL OPERATIONS       =======================
 ============================================================================*/
// Benchmark += polynomial
static void BM_PlusAssignment_PolyOp(benchmark::State& state) {
    bst::Polynomial<double> poly{1,2,3,4,5};
    bst::Polynomial<double> poly2(state.range(0), -5);
    for (auto _ : state){
        benchmark::DoNotOptimize(poly += poly2);
    }
}
BENCHMARK(BM_PlusAssignment_PolyOp)->DenseRange(1, 11, 5);


// Benchmark -= polynomial
static void BM_MinusAssignment_PolyOp(benchmark::State& state) {
    bst::Polynomial<double> poly{1,2,3,4,5};
    bst::Polynomial<double> poly2(state.range(0), -5);
    for (auto _ : state){
        benchmark::DoNotOptimize(poly -= poly2);
    }
}
BENCHMARK(BM_MinusAssignment_PolyOp)->DenseRange(1, 11, 5);





// Benchmark poly + poly
static void BM_Plus_PolyOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    bst::Polynomial<double> poly2;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), 15);
        poly2 = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(poly + poly2);
    }
}
BENCHMARK(BM_Plus_PolyOp)->DenseRange(1, 101, 50);

// Benchmark poly + poly&&
static void BM_LvaluePlusRvalue_PolyOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    bst::Polynomial<double> poly2;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), 15);
        poly2 = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(poly + std::move(poly2));
    }
}
BENCHMARK(BM_LvaluePlusRvalue_PolyOp)->DenseRange(1, 101, 50);


// Benchmark poly&& + poly
static void BM_RvaluePlusLvalue_PolyOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    bst::Polynomial<double> poly2;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), 15);
        poly2 = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(std::move(poly) + poly2);
    }
}
BENCHMARK(BM_RvaluePlusLvalue_PolyOp)->DenseRange(1, 101, 50);


// Benchmark poly&& + poly&&
static void BM_RvaluePlusRvalue_PolyOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    bst::Polynomial<double> poly2;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), 15);
        poly2 = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(std::move(poly) + std::move(poly2));
    }
}
BENCHMARK(BM_RvaluePlusRvalue_PolyOp)->DenseRange(1, 101, 50);






// Benchmark poly + poly
static void BM_Minus_PolyOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    bst::Polynomial<double> poly2;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), 15);
        poly2 = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(poly - poly2);
    }
}
BENCHMARK(BM_Minus_PolyOp)->DenseRange(1, 101, 50);

// Benchmark poly + poly&&
static void BM_LvalueMinusRvalue_PolyOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    bst::Polynomial<double> poly2;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), 15);
        poly2 = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(poly - std::move(poly2));
    }
}
BENCHMARK(BM_LvalueMinusRvalue_PolyOp)->DenseRange(1, 101, 50);


// Benchmark poly&& + poly
static void BM_RvalueMinusLvalue_PolyOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    bst::Polynomial<double> poly2;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), 15);
        poly2 = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(std::move(poly) - poly2);
    }
}
BENCHMARK(BM_RvalueMinusLvalue_PolyOp)->DenseRange(1, 101, 50);


// Benchmark poly&& + poly&&
static void BM_RvalueMinusRvalue_PolyOp(benchmark::State& state) {
    bst::Polynomial<double> poly;
    bst::Polynomial<double> poly2;
    for (auto _ : state){
        poly = bst::Polynomial<double> (state.range(0), 15);
        poly2 = bst::Polynomial<double> (state.range(0), -5);
        benchmark::DoNotOptimize(std::move(poly) - std::move(poly2));
    }
}
BENCHMARK(BM_RvalueMinusRvalue_PolyOp)->DenseRange(1, 101, 50);




// Benchmark poly&& + poly&&
static void BM_Multiply_PolyOp(benchmark::State& state) {
    bst::Polynomial<double> poly(state.range(0), 15);;
    bst::Polynomial<double> poly2(state.range(0), -3.5);
    for (auto _ : state){
        benchmark::DoNotOptimize(poly * poly2);
    }
}
BENCHMARK(BM_Multiply_PolyOp)->DenseRange(1, 101, 50);






/*=============================================================================
 ====================       Special OPERATIONS       =======================
 ============================================================================*/
// Benchmark power
static void BM_Power_SpecialOp(benchmark::State& state) {
    bst::Polynomial<double> poly{1,2};
    for (auto _ : state){
        benchmark::DoNotOptimize(bst::pow(poly, state.range(0)));
    }
}
BENCHMARK(BM_Power_SpecialOp)->DenseRange(0, 5, 1);


//Derivative
static void BM_Derivative_SpecialOp(benchmark::State& state) {
    bst::Polynomial<double> poly{1,2,3,4,5,6,7,8,9,10};
    for (auto _ : state){
        benchmark::DoNotOptimize(bst::der(poly, state.range(0)));
    }
}
BENCHMARK(BM_Derivative_SpecialOp)->DenseRange(0, 11, 1);


//Indefinite Integral
static void BM_IndefiniteIntegral_SpecialOp(benchmark::State& state) {
    bst::Polynomial<double> poly(state.range(0), 1.2);
    for (auto _ : state){
        benchmark::DoNotOptimize(bst::indefinite_integral(poly));
    }
}
BENCHMARK(BM_IndefiniteIntegral_SpecialOp)->DenseRange(0, 11, 1);



//Indefinite Integral
static void BM_Integral_SpecialOp(benchmark::State& state) {
    bst::Polynomial<double> poly(state.range(0), 1.2);
    for (auto _ : state){
        benchmark::DoNotOptimize(bst::integral(poly, 0.3, 1.7));
    }
}
BENCHMARK(BM_Integral_SpecialOp)->DenseRange(0, 11, 1);

BENCHMARK_MAIN();
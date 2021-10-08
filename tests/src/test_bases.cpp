// This file is part of BSTools, a C++ template library for B-Splines

// Copyright 2020 Carlo Masone

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



#include "../../include/knots_sequence/static_knot_sequence.h"
#include "../../include/polynomials/polynomial_core.h"
#include "../../include/bspline/bs_basis.h"
#include "../include/test_bases_impl.h"


#include "gtest/gtest.h"

namespace bst{
namespace test{


    // ==============================================================================
    // =======================           TESTS          =============================
    // ==============================================================================
    // Tests polynomial constructors.
    TEST(TestBases, CheckConstructors){
        {
            StaticKnotSequence<Polyd, PeriodicBS, 4>ks{0, 0.3, 0.6, 1.0};
            BSBasis<StaticKnotSequence<Polyd, PeriodicBS, 4>, 2> test_base(ks);
            auto bases = test_base.get_bases();
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(0,0), Polyd{0.42857, -2.8571, 4.7619}, 0.00005 ) );
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(1,0), Polyd{0.57143, 2.8571, -10.3175}, 0.00005 ) );
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(2,0), Polyd{0, 0, 5.5556}, 0.00005 ) );

            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(0,1), Polyd{2, -6.6667, 5.5556}, 0.00005 ) );
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(1,1), Polyd{-1.4286, 9.5238, -10.3175}, 0.00005 ) );
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(2,1), Polyd{0.42857, -2.8571, 4.7619}, 0.00005 ) );

            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(0,2), Polyd{3.5714, -7.1429, 3.5714}, 0.00005 ) );
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(1,2), Polyd{-3.8571, 11.4286, -7.1429}, 0.00005 ) );
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(2,2), Polyd{1.2857, -4.2857, 3.5714}, 0.00005 ) );
        }


        {
            StaticKnotSequence<Polyd, BoundedBS, 4>ks{0, 0.3, 0.6, 1.0};
            BSBasis<StaticKnotSequence<Polyd, BoundedBS, 4>, 2> test_base(ks);
            auto bases = test_base.get_bases();
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(0,0), Polyd{1, -6.6667, 11.1111}, 0.00005 ) );
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(1,0), Polyd{0, 6.6667, -16.6667}, 0.00005 ) );
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(2,0), Polyd{0, 0, 5.5556}, 0.00005 ) );

            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(0,1), Polyd{2, -6.6667, 5.5556}, 0.00005 ) );
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(1,1), Polyd{-1.4286, 9.5238, -10.3175}, 0.00005 ) );
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(2,1), Polyd{0.42857, -2.8571, 4.7619}, 0.00005 ) );

            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(0,2), Polyd{3.5714, -7.1429, 3.5714}, 0.00005 ) );
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(1,2), Polyd{-4.8214, 14.6429, -9.8214}, 0.00005 ) );
            EXPECT_TRUE( TestBasesHelper<decltype(test_base)>::check_polybase(bases(2,2), Polyd{2.25, -7.5, 6.25}, 0.00005 ) );
        }
    };



    TEST(TestBases, CheckEvaluation){
        {
            StaticKnotSequence<Polyd, PeriodicBS, 4>ks{0, 0.3, 0.6, 1.0};
            BSBasis<StaticKnotSequence<Polyd, PeriodicBS, 4>, 2> base(ks);
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.0, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.00001, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.15, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.2, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.3, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.55, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.8, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.9999, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 1, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_periodic_evaluation(base, 0.00005) );
        }

        {
            StaticKnotSequence<Polyd, BoundedBS, 4>ks{0, 0.3, 0.6, 1.0};
            BSBasis<StaticKnotSequence<Polyd, BoundedBS, 4>, 2> base(ks);
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.0, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.00001, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.15, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.2, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.3, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.55, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.8, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 0.9999, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_evalbase(base, 1, 0.00005) );
            EXPECT_TRUE( TestBasesHelper<decltype(base)>::check_bounded_evaluation(base, 0.00005) );
        }
    };



} //namespace test
} //namespace bst



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
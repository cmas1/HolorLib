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



#include "../../include/knots_sequence/dynamic_knot_sequence.h"
#include "../../include/polynomials/polynomial_core.h"
#include "../include/ks_setup.h"


#include <list>

#include "gtest/gtest.h"

namespace bst{
namespace test{


    // ==============================================================================
    // =======================           TESTS          =============================
    // ==============================================================================
    // Tests polynomial constructors.
    TEST(TestDynamicKnotSequence, CheckConstructors){
        
        //test default constructor
        {
            DynamicKnotSequence<Polyd, BoundedBS> ks;
            std::vector<double> test_knots{0,1};
            std::vector<double> test_uknots{0,1};
            std::vector<size_t> test_interval_indices{0};
            EXPECT_TRUE( KSTestHelper::check_ks_constructor(ks, test_knots, test_uknots, test_interval_indices) );
        }


        //test constructor from std::vector
        {
            std::vector<double> test_knots{0,1,2,2,3,4,4,5};
            std::vector<double> test_uknots{0,1,2,3,4,5};
            std::vector<size_t> test_interval_indices{0,1,3,4,6};
            DynamicKnotSequence<Polyd, BoundedBS> ks(test_knots);
            EXPECT_TRUE( KSTestHelper::check_ks_constructor(ks, test_knots, test_uknots, test_interval_indices) );
        }
        {
            std::vector<double> test_knots{0,1,2,2,3,4,4,5};
            std::vector<double> test_uknots{0,1,2,3,4,5};
            std::vector<size_t> test_interval_indices{0,1,3,4,6};
            DynamicKnotSequence<Polyd, BoundedBS> ks(std::vector<double>{0,1,2,2,3,4,4,5});
            EXPECT_TRUE( KSTestHelper::check_ks_constructor(ks, test_knots, test_uknots, test_interval_indices) );
        }

        //test constructor from std::array
        {
            std::array<double,8> test_knots{0,1,2,2,3,4,4,5};
            std::vector<double> test_uknots{0,1,2,3,4,5};
            std::vector<size_t> test_interval_indices{0,1,3,4,6};
            DynamicKnotSequence<Polyd, BoundedBS> ks(test_knots);
            EXPECT_TRUE( KSTestHelper::check_ks_constructor(ks, test_knots, test_uknots, test_interval_indices) );
        }
        {
            std::array<double,8> test_knots{0,1,2,2,3,4,4,5};
            std::vector<double> test_uknots{0,1,2,3,4,5};
            std::vector<size_t> test_interval_indices{0,1,3,4,6};
            DynamicKnotSequence<Polyd, BoundedBS> ks(std::array<double,8>{0,1,2,2,3,4,4,5});
            EXPECT_TRUE( KSTestHelper::check_ks_constructor(ks, test_knots, test_uknots, test_interval_indices) );
        }


        //test constructor from std::list
        {
            std::list<double> test_knots{0,1,2,2,3,4,4,5};
            std::vector<double> test_uknots{0,1,2,3,4,5};
            std::vector<size_t> test_interval_indices{0,1,3,4,6};
            DynamicKnotSequence<Polyd, BoundedBS> ks(test_knots);
            EXPECT_TRUE( KSTestHelper::check_ks_constructor(ks, test_knots, test_uknots, test_interval_indices) );
        }
        {
            std::list<double> test_knots{0,1,2,2,3,4,4,5};
            std::vector<double> test_uknots{0,1,2,3,4,5};
            std::vector<size_t> test_interval_indices{0,1,3,4,6};
            DynamicKnotSequence<Polyd, BoundedBS> ks(std::list<double>{0,1,2,2,3,4,4,5});
            EXPECT_TRUE( KSTestHelper::check_ks_constructor(ks, test_knots, test_uknots, test_interval_indices) );
        }

        //test constructor from std::initializer_list
        {
            std::initializer_list<double> test_knots{0,1,2,2,3,4,4,5};
            std::vector<double> test_uknots{0,1,2,3,4,5};
            std::vector<size_t> test_interval_indices{0,1,3,4,6};
            DynamicKnotSequence<Polyd, BoundedBS> ks(test_knots);
            EXPECT_TRUE( KSTestHelper::check_ks_constructor(ks, test_knots, test_uknots, test_interval_indices) );
        }
        {
            std::initializer_list<double> test_knots{0,1,2,2,3,4,4,5};
            std::vector<double> test_uknots{0,1,2,3,4,5};
            std::vector<size_t> test_interval_indices{0,1,3,4,6};
            DynamicKnotSequence<Polyd, BoundedBS> ks{0,1,2,2,3,4,4,5};
            EXPECT_TRUE( KSTestHelper::check_ks_constructor(ks, test_knots, test_uknots, test_interval_indices) );
        }
    };


    // Test find_interval
    TEST(TestDynamicKnotSequence, CheckFindInterval){
        DynamicKnotSequence<Polyd, BoundedBS> ksb{0.0, 0.3, 0.3, 0.5, 0.65, 0.65, 0.8, 1.0};
        DynamicKnotSequence<Polyd, PeriodicBS> ksp{0.0, 0.3, 0.3, 0.5, 0.65, 0.65, 0.8, 1.0};
        EXPECT_EQ(ksb.find_interval(0), 0);
        EXPECT_EQ(ksb.find_interval(0.2), 0);
        EXPECT_EQ(ksb.find_interval(0.3), 1);
        EXPECT_EQ(ksb.find_interval(0.45), 1);
        EXPECT_EQ(ksb.find_interval(0.5), 2);
        EXPECT_EQ(ksb.find_interval(0.6), 2);
        EXPECT_EQ(ksb.find_interval(0.65), 3);
        EXPECT_EQ(ksb.find_interval(0.7), 3);
        EXPECT_EQ(ksb.find_interval(0.8), 4);
        EXPECT_EQ(ksb.find_interval(0.9), 4);
        EXPECT_EQ(ksb.find_interval(1.0), 4);
        
        EXPECT_EQ(ksp.find_interval(0), 0);
        EXPECT_EQ(ksp.find_interval(0.2), 0);
        EXPECT_EQ(ksp.find_interval(0.3), 1);
        EXPECT_EQ(ksp.find_interval(0.45), 1);
        EXPECT_EQ(ksp.find_interval(0.5), 2);
        EXPECT_EQ(ksp.find_interval(0.6), 2);
        EXPECT_EQ(ksp.find_interval(0.65), 3);
        EXPECT_EQ(ksp.find_interval(0.7), 3);
        EXPECT_EQ(ksp.find_interval(0.8), 4);
        EXPECT_EQ(ksp.find_interval(0.9), 4);
        EXPECT_EQ(ksp.find_interval(1.0), 4);
    }


    // Test shift
    TEST(TestDynamicKnotSequence, CheckShift){
        DynamicKnotSequence<Polyd, BoundedBS> ksb{0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
        DynamicKnotSequence<Polyd, PeriodicBS> ksp{0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
        EXPECT_EQ(ksb.shift(1,3), 4);
        EXPECT_EQ(ksb.shift(1,6), 5);
        EXPECT_EQ(ksb.shift(1,-3), 0);

        EXPECT_EQ(ksp.shift(1,3), 4);
        EXPECT_EQ(ksp.shift(1,6), 2);
        EXPECT_EQ(ksp.shift(1,-3), 3);
    }


    //Test forward length
    TEST(TestDynamicKnotSequence, CheckFwdLength){
        DynamicKnotSequence<Polyd, BoundedBS> ksb{0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
        auto knotsb = ksb.knots();
        EXPECT_DOUBLE_EQ(ksb.forward_length(knotsb[3], knotsb[ksb.shift(3,4)]), 0.4);
        EXPECT_DOUBLE_EQ(ksb.forward_length(knotsb[3],knotsb[4]), 0.2);
        EXPECT_DOUBLE_EQ(ksb.forward_length(knotsb[3],knotsb[3]), 0.0);

        DynamicKnotSequence<Polyd, PeriodicBS> ksp{0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
        auto knotsp = ksp.knots();
        EXPECT_DOUBLE_EQ(ksp.forward_length(knotsp[3], knotsb[ksp.shift(3,4)]), 0.8);
        EXPECT_DOUBLE_EQ(ksp.forward_length(knotsp[3],knotsp[4]), 0.2);
        EXPECT_DOUBLE_EQ(ksp.forward_length(knotsp[3],knotsp[3]), 0.0);
    }


    //Test forward length from knot
    TEST(TestDynamicKnotSequence, CheckFwdLengthFrom){
        DynamicKnotSequence<Polyd, BoundedBS> ksb{0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
        auto knotsb = ksb.knots();
        EXPECT_TRUE( (ksb.forward_length_from_knot(3,4) == Polyd{-0.6, 1}) );
        EXPECT_TRUE( (ksb.forward_length_from_knot(ksb.shift(1,-3),4) == Polyd{0.0, 1}) );

        DynamicKnotSequence<Polyd, PeriodicBS> ksp{0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
        auto knotsp = ksp.knots();
        EXPECT_TRUE( (ksp.forward_length_from_knot(3,4) == Polyd{-0.6, 1}) );
        EXPECT_TRUE( (ksp.forward_length_from_knot(ksp.shift(1,-3),4) == Polyd{-0.6, 1}) );
    }


    //Test forward length to knot
    TEST(TestDynamicKnotSequence, CheckFwdLengthTo){
        DynamicKnotSequence<Polyd, BoundedBS> ksb{0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
        auto knotsb = ksb.knots();
        EXPECT_TRUE( (ksb.forward_length_to_knot(2,4) == Polyd{0.8, -1}) );
        EXPECT_TRUE( (ksb.forward_length_to_knot(2, ksb.shift(4,2)) == Polyd{1.0, -1}) );

        DynamicKnotSequence<Polyd, PeriodicBS> ksp{0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
        auto knotsp = ksp.knots();
        EXPECT_TRUE( (ksp.forward_length_to_knot(2,4) == Polyd{0.8, -1}) );
        EXPECT_TRUE( (ksp.forward_length_to_knot(2, ksp.shift(4,2)) == Polyd{1.2, -1}) );
    }

}
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
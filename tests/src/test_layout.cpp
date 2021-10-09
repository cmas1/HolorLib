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



#include <algorithm>
#include <holor/holor_full.h>
#include <gtest/gtest.h>

using namespace holor;


/*=================================================================================
                                Static Aliases
=================================================================================*/
TEST(TestLayout, CheckAliases){
    {
        EXPECT_EQ(Layout<1>::order, 1);
        EXPECT_EQ(Layout<2>::order, 2);
        EXPECT_EQ(Layout<3>::order, 3);
        EXPECT_EQ(Layout<100>::order, 100);
    }
}


/*=================================================================================
                                Constructors Tests
=================================================================================*/
TEST(TestLayout, CheckConstructors){
    //test for default constructor
    {
        {
            Layout<1> layout;
            EXPECT_EQ(layout.lengths().size(), 1);
            EXPECT_EQ(layout.strides().size(), 1);
            EXPECT_EQ(layout.offset(), 0);
            EXPECT_TRUE(std::all_of(layout.lengths().begin(), layout.lengths().end(), [](size_t i){return i==0;}));
            EXPECT_TRUE(std::all_of(layout.strides().begin(), layout.strides().end(), [](size_t i){return i==0;}));
        }

        {
            Layout<5> layout;
            EXPECT_EQ(layout.lengths().size(), 5);
            EXPECT_EQ(layout.strides().size(), 5);
            EXPECT_EQ(layout.offset(), 0);
            for(auto&& l : layout.lengths()){
                std::cout << l << ", ";
            }
            std::cout << std::endl;
            EXPECT_TRUE(std::all_of(layout.lengths().begin(), layout.lengths().end(), [](size_t i){return i==0;}));
            EXPECT_TRUE(std::all_of(layout.strides().begin(), layout.strides().end(), [](size_t i){return i==0;}));
        }
    }

    // //test constructor from lvalue std::initializer_list
    // {
    //     std::initializer_list<double> init_list = {-1.0, 3.2, 0.0, -0.3, 0.7, 0.9};
    //     EXPECT_TRUE(PolyTestHelper<double>::check_poly_constructor(Polynomial<double>(init_list), init_list)) << "Constructor yield different results";
    //     std::initializer_list<double> init_list2 = {-1.0, 3.2, 0.0, -0.3, 0.7, 0.0, 0.0};
    //     EXPECT_TRUE(PolyTestHelper<double>::check_poly_constructor(Polynomial<double>(init_list2), init_list2)) << "Constructor yield different results";
    // }

    // //test constructor from rvalue std::initializer_list
    // {
    //     EXPECT_TRUE(PolyTestHelper<double>::check_poly_constructor(Polynomial<double>{-1.0, 0.0, -0.3}, std::initializer_list<double>{-1.0, 0.0, -0.3})) << "Constructor yield different results";
    //     EXPECT_TRUE(PolyTestHelper<double>::check_poly_constructor(Polynomial<double>{-1.0, 0.0, -0.3, 0.0}, std::initializer_list<double>{-1.0, 0.0, -0.3, 0.0})) << "Constructor yield different results";
    // }


    // //test constructor from lvalue std::vector
    // {
    //     std::vector<double> init_vec{-1.0, 3.2, 0.0, -0.3, 0.7, 0.9};
    //     EXPECT_TRUE(PolyTestHelper<double>::check_poly_constructor(Polynomial<double>(init_vec), init_vec)) << "Constructor yield different results";
    //     std::vector<double> init_vec2{-1.0, 3.2, 0.0, -0.3, 0.7, 0.0, 0.0};
    //     EXPECT_TRUE(PolyTestHelper<double>::check_poly_constructor(Polynomial<double>(init_vec2), init_vec2)) << "Constructor yield different results";
    // }

    // //test constructor from rvalue std::vector
    // {
    //     EXPECT_TRUE(PolyTestHelper<double>::check_poly_constructor(Polynomial<double>(std::vector<double>{-1.0, 0.0, -0.3}), std::vector<double>{-1.0, 0.0, -0.3})) << "Constructor yield different results";
    //     EXPECT_TRUE(PolyTestHelper<double>::check_poly_constructor(Polynomial<double>(std::vector<double>{-1.0, 0.0, -0.3, 0.0}), std::vector<double>{-1.0, 0.0, -0.3, 0.0})) << "Constructor yield different results";
    // }

    // //test constructor from degree, value
    // {
    //     EXPECT_TRUE(PolyTestHelper<double>::check_dv_constructor(3, 7.31)) << "Constructor yield different results";
    //     EXPECT_TRUE(PolyTestHelper<double>::check_dv_constructor(3, 0)) << "Constructor yield different results";
    //     EXPECT_TRUE(PolyTestHelper<double>::check_dv_constructor(3)) << "Constructor yield different results";
        
    // }

};




// TEST(TestPolynomial, CheckEvalutation){
//     {
//         Polynomial<double> poly{1, -2, 0, 2};
//         EXPECT_EQ(poly(0), 1);
//         EXPECT_EQ(poly(-2), -11 );
//         EXPECT_EQ(poly(1), 1);
//     }
//     {
//         Polynomial<double> poly{0, 1};
//         EXPECT_EQ(poly(0), 0);
//         EXPECT_EQ(poly(2), 2);
//         EXPECT_EQ(poly(-2), -2);
//     }
//     {
//         Polynomial<double> poly{0, 0, 1};
//         EXPECT_EQ(poly(0), 0);
//         EXPECT_EQ(poly(2), 4);
//         EXPECT_EQ(poly(-2), 4);
//     }
// }



// TEST(TestPolynomial, CheckScalarOperations){
//     {
//         Polynomial<double> poly{1, -2, 0, 2};
//         poly += 3;
//         EXPECT_TRUE((poly  == Polynomial<double>{4, -2, 0, 2}));
//         poly += -4;
//         EXPECT_TRUE((poly  == Polynomial<double>{0, -2, 0, 2}));
//         poly -= 5;
//         EXPECT_TRUE((poly  == Polynomial<double>{-5, -2, 0, 2}));
//         poly -= -7;
//         EXPECT_TRUE((poly  == Polynomial<double>{2, -2, 0, 2}));
//         poly *= 4;
//         EXPECT_TRUE((poly  == Polynomial<double>{8, -8, 0, 8}));
//         poly /= 8;
//         EXPECT_TRUE((poly  == Polynomial<double>{1, -1, 0, 1}));
//         poly *= 0;
//         EXPECT_TRUE((poly  == Polynomial<double>{0}));
//         EXPECT_FALSE((poly  != Polynomial<double>{0}));
//         poly = Polynomial<double>{2, -2};
//         EXPECT_TRUE((poly.div(2)  == Polynomial<double>{1, -1}));
//         EXPECT_TRUE((poly.div(0)  == Polynomial<double>{0}));

//         poly.div_inplace(2);
//         EXPECT_TRUE((poly  == Polynomial<double>{1, -1}));
//         poly.div_inplace(0);
//         EXPECT_TRUE((poly  == Polynomial<double>{0}));



//         EXPECT_TRUE((Polynomial<double>{1, 0, -2}+3 == Polynomial<double>{4, 0, -2}));
//         EXPECT_TRUE(( 3+Polynomial<double>{1, 0, -2} == Polynomial<double>{4, 0, -2}));
//         EXPECT_TRUE((Polynomial<double>{1, 0, -2}+0 == Polynomial<double>{1, 0, -2}));
//         EXPECT_TRUE((0+Polynomial<double>{1, 0, -2} == Polynomial<double>{1, 0, -2}));

//         EXPECT_TRUE((Polynomial<double>{1, 0, -2}-3 == Polynomial<double>{-2, 0, -2}));
//         EXPECT_TRUE((3-Polynomial<double>{1, 0, -2} == Polynomial<double>{2, 0, +2}));
//         EXPECT_TRUE((Polynomial<double>{1, 0, -2}-0 == Polynomial<double>{1, 0, -2}));
//         EXPECT_TRUE((0-Polynomial<double>{1, 0, -2} == Polynomial<double>{-1, 0, +2}));

//         EXPECT_TRUE((Polynomial<double>{1, 0, -2}*3 == Polynomial<double>{3, 0, -6}));
//         EXPECT_TRUE((3*Polynomial<double>{1, 0, -2} == Polynomial<double>{3, 0, -6}));
//         EXPECT_TRUE((Polynomial<double>{1, 0, -2}*1 == Polynomial<double>{1, 0, -2}));
//         EXPECT_TRUE((1*Polynomial<double>{1, 0, -2} == Polynomial<double>{1, 0, -2}));
//         EXPECT_TRUE((Polynomial<double>{1, 0, -2}*0 == Polynomial<double>{0}));
//         EXPECT_TRUE((0*Polynomial<double>{1, 0, -2} == Polynomial<double>{0}));

//         EXPECT_TRUE((Polynomial<double>{4, 0, -2}/2 == Polynomial<double>{2, 0, -1}));
//         EXPECT_TRUE((Polynomial<double>{4, 0, -2}/1 == Polynomial<double>{4, 0, -2}));

//     }

// }



// TEST(TestPolynomial, CheckPolyOperations){
//     EXPECT_TRUE( ( Polynomial<double>{0,2} + Polynomial<double>{-1,2,4} == Polynomial<double>{-1,4,4}) );
//     EXPECT_TRUE( ( Polynomial<double>{-1,2,4} + Polynomial<double>{0,2} == Polynomial<double>{-1,4,4}) );
//     EXPECT_TRUE( ( Polynomial<double>{-1,2,4} + Polynomial<double>{0,0,-4} == Polynomial<double>{-1,2}) );
//     EXPECT_TRUE( ( Polynomial<double>{-1,2,4} + Polynomial<double>{1,-2,-4} == Polynomial<double>{0}) );
//     EXPECT_TRUE( ( Polynomial<double>{1,2} + Polynomial<double>{0} == Polynomial<double>{1,2}) );
//     EXPECT_TRUE( ( Polynomial<double>{0} + Polynomial<double>{1,2} == Polynomial<double>{1,2}) );

//     EXPECT_TRUE( ( Polynomial<double>{0,2} - Polynomial<double>{-1,2,4} == Polynomial<double>{1,0,-4}) );
//     EXPECT_TRUE( ( Polynomial<double>{-1,2,4} - Polynomial<double>{0,2} == Polynomial<double>{-1,0,4}) );
//     EXPECT_TRUE( ( Polynomial<double>{-1,2,4} - Polynomial<double>{0,0,4} == Polynomial<double>{-1,2}) );
//     EXPECT_TRUE( ( Polynomial<double>{-1,2,4} - Polynomial<double>{-1,2,4} == Polynomial<double>{0}) );
//     EXPECT_TRUE( ( Polynomial<double>{1,2} - Polynomial<double>{0} == Polynomial<double>{1,2}) );
//     EXPECT_TRUE( ( Polynomial<double>{0} - Polynomial<double>{1,2} == Polynomial<double>{-1,-2}) );

//     EXPECT_TRUE( ( Polynomial<double>{-1,0,4} * Polynomial<double>{1,2} == Polynomial<double>{-1, -2, 4, 8}) );
//     EXPECT_TRUE( ( Polynomial<double>{1,2} * Polynomial<double>{-1,0,4} == Polynomial<double>{-1, -2, 4, 8}) );
//     EXPECT_TRUE( ( Polynomial<double>{1,2} * Polynomial<double>{0} == Polynomial<double>{0}) );
//     EXPECT_TRUE( ( Polynomial<double>{0} * Polynomial<double>{1,2} == Polynomial<double>{0}) );
//     EXPECT_TRUE( ( Polynomial<double>{1,2} * Polynomial<double>{1} == Polynomial<double>{1,2}) );
//     EXPECT_TRUE( ( Polynomial<double>{1} * Polynomial<double>{1,2} == Polynomial<double>{1,2}) );
// }


// TEST(TestPolynomial, CheckOtherFunctions){
//     //test power function
//     Polynomial<double> poly{2,3,1};
//     EXPECT_EQ(bst::pow(poly,0), Polynomial<double>{1});
//     EXPECT_EQ(bst::pow(poly,1), poly);
//     EXPECT_TRUE(  (bst::pow(poly,2) == Polynomial<double>{4,12,13,6,1})   );

//     //test derivative function
//     EXPECT_EQ( bst::der(poly,0), poly);
//     EXPECT_TRUE(  (bst::der(poly,1) == Polynomial<double>{3,2})  );
//     EXPECT_TRUE(  (bst::der(poly,2) == Polynomial<double>{2})  );
//     EXPECT_TRUE(  (bst::der(poly,3) == Polynomial<double>{0})  );
//     EXPECT_TRUE(  (bst::der(poly,14) == Polynomial<double>{0})  );

//     //test integration function
//     auto integral_poly = bst::indefinite_integral(poly);
//     EXPECT_DOUBLE_EQ(integral_poly.coeffs()[0], 0);
//     EXPECT_DOUBLE_EQ(integral_poly.coeffs()[1], 2);
//     EXPECT_DOUBLE_EQ(integral_poly.coeffs()[2], 3.0/2.0);
//     EXPECT_DOUBLE_EQ(integral_poly.coeffs()[3], 1.0/3.0);

//     EXPECT_DOUBLE_EQ(bst::integral(poly, 2.0, 2.0), 0.0);
//     EXPECT_DOUBLE_EQ(bst::integral(poly, 1.0, 2.0), 8.83333333333333333);
// }




int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
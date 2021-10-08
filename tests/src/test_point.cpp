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




#include "../include/test_point_impl.h"
#include "gtest/gtest.h"

namespace bst{
namespace test{


    // ==============================================================================
    // =======================           TESTS          =============================
    // ==============================================================================
    // Tests constructors.
    TEST(TestPoint, CheckConstructors){
        {
            Point<double, 5> p;
            auto b = TestPointNHelper<double, 5>::check_constructor(p, std::vector<double>{0, 0, 0, 0 ,0});
            EXPECT_TRUE( b ) << "Constructor yield different results";
        }

        {
            std::vector<double> vec{1.0, 2.0, 3.0, 4.0};
            Point<double, 4> p(vec);
            auto b = TestPointNHelper<double, 4>::check_constructor(p, vec);
            EXPECT_TRUE( b ) << "Constructor yield different results";
        }

        {
            Point<double, 4> p(std::vector<double> {1.0, 2.0, 3.0, 4.0});
            auto b = TestPointNHelper<double, 4>::check_constructor(p, std::vector<double> {1.0, 2.0, 3.0, 4.0});
            EXPECT_TRUE( b ) << "Constructor yield different results";
        }

        {
            Point<double, 4> p{1.0, 2.0, 3.0, 4.0};
            auto b = TestPointNHelper<double, 4>::check_constructor(p, std::vector<double> {1.0, 2.0, 3.0, 4.0});
            EXPECT_TRUE( b ) << "Constructor yield different results";
        }

        {
            std::initializer_list<double> list = {1.0, 2.0, 3.0, 4.0};
            Point<double, 4> p(list);
            auto b = TestPointNHelper<double, 4>::check_constructor(p, list);
            EXPECT_TRUE( b ) << "Constructor yield different results";
        }

        {
            Point<double, 4> p{0.0, 1.0, 6.0, 8.0};
            Point<double, 4> p2(p);
            Point<double, 4> p3 = p;
            EXPECT_TRUE( (p2 == p) );
            EXPECT_TRUE( (p3 == p) );
            Point<double, 4> p4(std::move(p2));
            Point<double, 4> p5 = std::move(p3);
            EXPECT_TRUE( (p4 == p) );
            EXPECT_TRUE( (p5 == p) );
        }
    };


    TEST(TestPoint, CheckIndexing){
        {
            Point<double, 3> p{1.0, 2.0, 3.0};
            p[0] = 5;
            EXPECT_DOUBLE_EQ(p.data()[0], 5);
        }
    }


    TEST(TestPoint, CheckScalarOperations){
        Point<double,4> point{1, -2, 0, 2};
        point *= 3;
        EXPECT_TRUE((point  == Point<double, 4>{3, -6, 0, 6}));
        auto point3 = point * 0.5;
        EXPECT_DOUBLE_EQ(point3[0], 1.5);
        EXPECT_DOUBLE_EQ(point3[1], -3);
        EXPECT_DOUBLE_EQ(point3[2], 0);
        EXPECT_DOUBLE_EQ(point3[3], 3);
        
        point3 = 0.5*point;
        EXPECT_DOUBLE_EQ(point3[0], 1.5);
        EXPECT_DOUBLE_EQ(point3[1], -3);
        EXPECT_DOUBLE_EQ(point3[2], 0);
        EXPECT_DOUBLE_EQ(point3[3], 3);

        point3 = Point<double,4>{3, -6, 0, 6} * 0.5;
        EXPECT_DOUBLE_EQ(point3[0], 1.5);
        EXPECT_DOUBLE_EQ(point3[1], -3);
        EXPECT_DOUBLE_EQ(point3[2], 0);
        EXPECT_DOUBLE_EQ(point3[3], 3);

        point3 = 0.5 * Point<double,4>{3, -6, 0, 6};
        EXPECT_DOUBLE_EQ(point3[0], 1.5);
        EXPECT_DOUBLE_EQ(point3[1], -3);
        EXPECT_DOUBLE_EQ(point3[2], 0);
        EXPECT_DOUBLE_EQ(point3[3], 3);
    }



    TEST(TestPoint, CheckPointOperations){
        {
            Point<double,4> point{1, -2, 0, 2};
            Point<double,4> point2{3, 0.5, 1.6, 2};
            point+=point2;
            EXPECT_DOUBLE_EQ(point[0], 4);
            EXPECT_DOUBLE_EQ(point[1], -1.5);
            EXPECT_DOUBLE_EQ(point[2], 1.6);
            EXPECT_DOUBLE_EQ(point[3], 4);
        }

        {
            Point<double,4> point{1, -2, 0, 2};
            Point<double,4> point2{3, 0.5, 1.6, 2};
            auto point3 = point + point2;
            EXPECT_DOUBLE_EQ(point3[0], 4);
            EXPECT_DOUBLE_EQ(point3[1], -1.5);
            EXPECT_DOUBLE_EQ(point3[2], 1.6);
            EXPECT_DOUBLE_EQ(point3[3], 4);
        }

        {
            Point<double,4> point{1, -2, 0, 2};
            auto point3 = point + Point<double,4>{3, 0.5, 1.6, 2};
            EXPECT_DOUBLE_EQ(point3[0], 4);
            EXPECT_DOUBLE_EQ(point3[1], -1.5);
            EXPECT_DOUBLE_EQ(point3[2], 1.6);
            EXPECT_DOUBLE_EQ(point3[3], 4);
        }

        {                
            Point<double,4> point2{3, 0.5, 1.6, 2};
            auto point3 = Point<double,4>{1, -2, 0, 2} + point2;
            EXPECT_DOUBLE_EQ(point3[0], 4);
            EXPECT_DOUBLE_EQ(point3[1], -1.5);
            EXPECT_DOUBLE_EQ(point3[2], 1.6);
            EXPECT_DOUBLE_EQ(point3[3], 4);
        }

        {
            auto point3 = Point<double,4>{1, -2, 0, 2} + Point<double,4>{3, 0.5, 1.6, 2};
            EXPECT_DOUBLE_EQ(point3[0], 4);
            EXPECT_DOUBLE_EQ(point3[1], -1.5);
            EXPECT_DOUBLE_EQ(point3[2], 1.6);
            EXPECT_DOUBLE_EQ(point3[3], 4);
        }
    }

}
}





int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
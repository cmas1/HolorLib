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




#include "../../include/control_points/point.h"
#include "gtest/gtest.h"

namespace bst{
namespace test{


    // ==============================================================================
    // =======================           TESTS          =============================
    // ==============================================================================
    // Tests constructors.
    TEST(TestPoint, CheckConstructors){
        {
            Point<double, 1> p;
            EXPECT_DOUBLE_EQ( p.data(), 0.0);
        }

        {
            double val{5.0};
            Point<double, 1> p(val);
            EXPECT_DOUBLE_EQ( p.data(), 5.0);
        }

        {
            Point<double, 1> p(5.0);
            EXPECT_DOUBLE_EQ( p.data(), 5.0);
        }

        {
            Point<double, 1> p(5.0);
            Point<double, 1> p2(p);
            Point<double, 1> p3 = p;
            EXPECT_TRUE( (p2 == p) );
            EXPECT_TRUE( (p3 == p) );
            Point<double, 1> p4(std::move(p2));
            Point<double, 1> p5 = std::move(p3);
            EXPECT_TRUE( (p4 == p) );
            EXPECT_TRUE( (p5 == p) );
        }
    };



    TEST(TestPoint, CheckScalarOperations){
        Point<double,1> point(2.0);
        point *= 3;
        EXPECT_DOUBLE_EQ(point.data(), 6.0);
        auto point3 = point * 4;
        EXPECT_DOUBLE_EQ(point3.data(), 24.0);

        point3 = 4*point;
        EXPECT_DOUBLE_EQ(point3.data(), 24.0);

        point3 = Point<double,1>{6.0} * 4;
        EXPECT_DOUBLE_EQ(point3.data(), 24.0);

        point3 = 4 * Point<double,1>(6.0);
        EXPECT_DOUBLE_EQ(point3.data(), 24.0);
    }



    TEST(TestPoint, CheckPointOperations){
        {
            Point<double,1> point{1};
            Point<double,1> point2{3};
            point+=point2;
            EXPECT_DOUBLE_EQ(point.data(), 4.0);
        }

        {
            Point<double,1> point{1};
            Point<double,1> point2{3};
            auto point3 = point + point2;
            EXPECT_DOUBLE_EQ(point3.data(), 4.0);
        }
        {
            Point<double,1> point{1};
            auto point3 = point + Point<double,1>(3);
            EXPECT_DOUBLE_EQ(point3.data(), 4.0);
        }
        {
            Point<double,1> point2{3};
            auto point3 = Point<double,1>{1} + point2;
            EXPECT_DOUBLE_EQ(point3.data(), 4.0);
        }
        {
            auto point3 = Point<double,1>{1} + Point<double,1>{3};
            EXPECT_DOUBLE_EQ(point3.data(), 4.0);
        }
    }

}
}





int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
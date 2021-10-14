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
#include <array>
#include <vector>
#include <holor/holor_full.h>
#include <gtest/gtest.h>

using namespace holor;



TEST(TestHolorComparisons, CheckComparisons){
    {
        Holor<int, 2> h1 {{1,2}, {3,4}};
        Holor<int, 2> h2 {{1,2}, {3,4}};
        EXPECT_TRUE( (h1==h2) );
        EXPECT_TRUE( (h2==h1) );
        EXPECT_TRUE( (h1==Holor<int, 2>{{1,2}, {3,4}}) );
        EXPECT_TRUE( (Holor<int, 2>{{1,2}, {3,4}} == h1) );
    }

    {
        std::vector<int> vec{1,2,3,4};
        std::array<int,4> arr{1,2,3,4};
        HolorRef<int, 2> h1 (vec.data(), Layout<2>(2,2));
        HolorRef<int, 2> h2 (arr.data(), Layout<2>(2,2));
        EXPECT_TRUE( (h1 == h2) );
        EXPECT_TRUE( (h2 == h1) );
        EXPECT_TRUE( (h1 == HolorRef<int, 2>(arr.data(), Layout<2>(2,2))) );
        EXPECT_TRUE( (HolorRef<int, 2>(arr.data(), Layout<2>(2,2)) == h1) );
    }


    {
        std::vector<int> vec{1,2,3,4};
        HolorRef<int, 2> h1 (vec.data(), Layout<2>(2,2));
        Holor<int, 2> h2{{1, 2}, {3, 4}};
        EXPECT_TRUE( (h1 == h2) );
        EXPECT_TRUE( (h2 == h1) );
        EXPECT_TRUE( (h2 == HolorRef<int, 2>(vec.data(), Layout<2>(2,2))) );
        EXPECT_TRUE( (HolorRef<int, 2>(vec.data(), Layout<2>(2,2)) == h2) );
        EXPECT_TRUE( (h1 == Holor<int, 2>{{1,2}, {3,4}}) );
        EXPECT_TRUE( (Holor<int, 2>{{1,2}, {3,4}} == h1) );
    }
}




int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

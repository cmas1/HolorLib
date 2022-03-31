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



#include <algorithm>
#include <array>
#include <vector>
#include <holor/holor_full.h>
#include <gtest/gtest.h>

using namespace holor;



TEST(TestIterators, CheckHolorRefIterators){
    {   
        std::vector<int> vec{1,2,3,4};
        HolorRef<int, 2> h(vec.data(), Layout<2>{2,2});
        auto it1 = h.cbegin();
        auto it2 = h.cend();
        EXPECT_TRUE ( (it2-it1 == 4) );
        EXPECT_TRUE ( (*it1== 1) ); it1++;
        EXPECT_TRUE ( (*it1== 2) ); it1++;
        EXPECT_TRUE ( (*it1== 3) ); it1++;
        EXPECT_TRUE ( (*it1== 4) ); it1++;
        EXPECT_TRUE ( (it1== it2) );
        it1 = h.cbegin();
        EXPECT_TRUE ( (*it1== 1) ); ++it1;
        EXPECT_TRUE ( (*it1== 2) ); ++it1;
        EXPECT_TRUE ( (*it1== 3) ); ++it1;
        EXPECT_TRUE ( (*it1== 4) ); ++it1;
        EXPECT_TRUE ( (it1== it2) );
        it1 = h.cbegin();
        --it2; EXPECT_TRUE ( (*it2== 4) ); 
        --it2; EXPECT_TRUE ( (*it2== 3) ); 
        --it2; EXPECT_TRUE ( (*it2== 2) ); 
        --it2; EXPECT_TRUE ( (*it2== 1) ); 
        EXPECT_TRUE ( (it1== it2) );
        it2 = h.cend();
        it2--; EXPECT_TRUE ( (*it2== 4) ); 
        it2--; EXPECT_TRUE ( (*it2== 3) ); 
        it2--; EXPECT_TRUE ( (*it2== 2) ); 
        it2--; EXPECT_TRUE ( (*it2== 1) ); 
        EXPECT_TRUE ( (it1== it2) );
        it1 = h.cbegin();
        it2 = h.cend();
        EXPECT_EQ(it2-it1, 4);
        EXPECT_EQ(it1-it2, -4);

        it1+=2;
        EXPECT_EQ( *it1, 3);
        it1-=2;
        EXPECT_EQ( *it1, 1);
    }


    {   
        Holor<int,3> holor{{{1,5}, {2,6}}, {{3,7}, {4, 8}}};
        auto hr = holor.slice<2>(0);
        auto it1 = hr.cbegin();
        auto it2 = hr.cend();
        EXPECT_TRUE ( (it2-it1 == 4) );
        EXPECT_TRUE ( (*it1== 1) ); it1++;
        EXPECT_TRUE ( (*it1== 2) ); it1++;
        EXPECT_TRUE ( (*it1== 3) ); it1++;
        EXPECT_TRUE ( (*it1== 4) ); it1++;
        EXPECT_TRUE ( (it1== it2) );
        it1 = hr.cbegin();
        EXPECT_TRUE ( (*it1== 1) ); ++it1;
        EXPECT_TRUE ( (*it1== 2) ); ++it1;
        EXPECT_TRUE ( (*it1== 3) ); ++it1;
        EXPECT_TRUE ( (*it1== 4) ); ++it1;
        EXPECT_TRUE ( (it1== it2) );
        it1 = hr.cbegin();
        --it2; EXPECT_TRUE ( (*it2== 4) ); 
        --it2; EXPECT_TRUE ( (*it2== 3) ); 
        --it2; EXPECT_TRUE ( (*it2== 2) ); 
        --it2; EXPECT_TRUE ( (*it2== 1) ); 
        EXPECT_TRUE ( (it1== it2) );
        it2 = hr.cend();
        it2--; EXPECT_TRUE ( (*it2== 4) ); 
        it2--; EXPECT_TRUE ( (*it2== 3) ); 
        it2--; EXPECT_TRUE ( (*it2== 2) ); 
        it2--; EXPECT_TRUE ( (*it2== 1) ); 
        EXPECT_TRUE ( (it1== it2) );
        it1 = hr.cbegin();
        it2 = hr.cend();
        EXPECT_EQ(it2-it1, 4);
        EXPECT_EQ(it1-it2, -4);

        it1+=2;
        EXPECT_EQ( *it1, 3);
        it1-=2;
        EXPECT_EQ( *it1, 1);
    }
}




int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

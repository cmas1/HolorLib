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


/*=================================================================================
                                Static Aliases
=================================================================================*/
TEST(TestHolor, CheckAliases){
    EXPECT_EQ( (Holor<int, 1>::dimensions), 1);
    EXPECT_EQ( (Holor<double, 1>::dimensions), 1);
    EXPECT_EQ( (Holor<int, 5>::dimensions), 5);
    EXPECT_EQ( (Holor<double, 5>::dimensions), 5);

    EXPECT_TRUE(  (std::is_same_v<Holor<int, 1>::value_type, int>)  );
    EXPECT_TRUE(  (std::is_same_v<Holor<double, 1>::value_type, double>)  );
    EXPECT_TRUE(  (std::is_same_v<Holor<float, 1>::value_type, float>)  );
}


/*=================================================================================
                                Constructors Tests
=================================================================================*/
TEST(TestHolor, CheckConstructors){
    //tests for default constructor
    {
        {
            Holor<int,1>my_holor;
            Layout<1> tmp_layout;
            auto my_layout = my_holor.layout();
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor.size(), 0 );
            EXPECT_EQ( my_holor.lengths(0), 0 );
        }
        {
            Holor<int,3>my_holor;
            Layout<3> tmp_layout;
            auto my_layout = my_holor.layout();
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor.size(), 0 );
            EXPECT_EQ( my_holor.lengths(0), 0 );
            EXPECT_EQ( my_holor.lengths(1), 0 );
            EXPECT_EQ( my_holor.lengths(2), 0 );
        }
    }

    //test for constructor from nested list
    {
        {
            Holor<int, 1> my_holor{1,2,3,4};
            auto my_layout = my_holor.layout();
            Layout<1> tmp_layout{4};
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor.size(), 4 );
            EXPECT_EQ( my_holor.lengths(0), 4 );
            EXPECT_EQ( my_holor.data()[0], 1 );
            EXPECT_EQ( my_holor.data()[1], 2 );
            EXPECT_EQ( my_holor.data()[2], 3 );
            EXPECT_EQ( my_holor.data()[3], 4 );
        }

        {
            Holor<int, 2> my_holor{{1,2},{3,4}};
            auto my_layout = my_holor.layout();
            Layout<2> tmp_layout{2,2};
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor.size(), 4 );
            EXPECT_EQ( my_holor.lengths(0), 2 );
            EXPECT_EQ( my_holor.lengths(1), 2 );
            EXPECT_EQ( my_holor.data()[0], 1 );
            EXPECT_EQ( my_holor.data()[1], 2 );
            EXPECT_EQ( my_holor.data()[2], 3 );
            EXPECT_EQ( my_holor.data()[3], 4 );
        }

        {
            Holor<int, 3> my_holor{{{1,2},{3,4}}, {{5,6},{7,8}}};
            auto my_layout = my_holor.layout();
            Layout<3> tmp_layout{2,2,2};
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor.size(), 8 );
            EXPECT_EQ( my_holor.lengths(0), 2 );
            EXPECT_EQ( my_holor.lengths(1), 2 );
            EXPECT_EQ( my_holor.lengths(2), 2 );
            EXPECT_EQ( my_holor.data()[0], 1 );
            EXPECT_EQ( my_holor.data()[1], 2 );
            EXPECT_EQ( my_holor.data()[2], 3 );
            EXPECT_EQ( my_holor.data()[3], 4 );
            EXPECT_EQ( my_holor.data()[4], 5 );
            EXPECT_EQ( my_holor.data()[5], 6 );
            EXPECT_EQ( my_holor.data()[6], 7 );
            EXPECT_EQ( my_holor.data()[7], 8 );
        }
    }

    //test for constructor from sized container of lengths
    {
        {
            Holor<float, 3> my_holor( std::array<size_t,3>{2,2,2} );
            auto my_layout = my_holor.layout();
            Layout<3> tmp_layout{2,2,2};
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor.size(), 8 );
            EXPECT_EQ( my_holor.lengths(0), 2 );
            EXPECT_EQ( my_holor.lengths(1), 2 );
            EXPECT_EQ( my_holor.lengths(2), 2 );
        }
        {
            std::array<size_t,3> my_lengths{2,2,2};
            Holor<float, 3> my_holor( my_lengths );
            auto my_layout = my_holor.layout();
            Layout<3> tmp_layout{2,2,2};
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor.size(), 8 );
            EXPECT_EQ( my_holor.lengths(0), 2 );
            EXPECT_EQ( my_holor.lengths(1), 2 );
            EXPECT_EQ( my_holor.lengths(2), 2 );
        }
    }

    //test for constructor from resizeable container
    {
        {
            Holor<float, 3> my_holor( std::vector<size_t>{2,2,2} );
            auto my_layout = my_holor.layout();
            Layout<3> tmp_layout{2,2,2};
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor.size(), 8 );
            EXPECT_EQ( my_holor.lengths(0), 2 );
            EXPECT_EQ( my_holor.lengths(1), 2 );
            EXPECT_EQ( my_holor.lengths(2), 2 );
        }
        {
            std::vector<size_t> my_lengths{2,2,2};
            Holor<float, 3> my_holor( my_lengths );
            auto my_layout = my_holor.layout();
            Layout<3> tmp_layout{2,2,2};
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor.size(), 8 );
            EXPECT_EQ( my_holor.lengths(0), 2 );
            EXPECT_EQ( my_holor.lengths(1), 2 );
            EXPECT_EQ( my_holor.lengths(2), 2 );
        }
    }

    //copy/move constructors
    {
        {
            Holor<int,2> h1{{1,2,3}, {4,5,6}};
            Holor<int,2> h2(h1);
            EXPECT_TRUE((h1==h2));
        }
        {
            Holor<int,2> h1{{1,2,3}, {4,5,6}};
            Holor<int,2> h2( Holor<int,2> {{1,2,3}, {4,5,6}} );
            EXPECT_TRUE((h1==h2));
        }
    }

    //constructor from HolorRef
    {
        std::vector<double> my_vec{1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
        HolorRef<double,2> hr(my_vec.data(), Layout<2>{2,3});
        Holor<double,2> h(hr);
        EXPECT_TRUE( (h==hr) );
        auto my_layout = h.layout();
        EXPECT_EQ(h.size(), 6);
        EXPECT_EQ(h.lengths(0), 2);
        EXPECT_EQ(h.lengths(1), 3);
        EXPECT_EQ(my_layout.stride(0), 3);
        EXPECT_EQ(my_layout.stride(1), 1);
        EXPECT_EQ(my_layout.offset(), 0);

    }
    
};


/*=================================================================================
                                Assignment Tests
// =================================================================================*/
// TEST(TestHolor, CheckAssignments){
// }



/*=================================================================================
                                Get/Set Tests
=================================================================================*/
// TEST(TestHolor, CheckGetSet){
// }


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
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
TEST(TestHolor, CheckConstructors){
    //test for default constructor
    {
        {
            Layout<1> layout;
            auto lengths = layout.lengths();
            auto strides = layout.strides();
            auto offset = layout.offset();
            EXPECT_EQ(lengths.size(), 1);
            EXPECT_EQ(strides.size(), 1);
            EXPECT_EQ(offset, 0);
            EXPECT_TRUE(std::ranges::all_of(lengths.cbegin(), lengths.cend(), [](int i){return i==0;}));
            EXPECT_TRUE(std::ranges::all_of(strides.cbegin(), strides.cend(), [](size_t i){return i==0;}));
        }

        {
            Layout<5> layout;
            auto lengths = layout.lengths();
            auto strides = layout.strides();
            auto offset = layout.offset();
            EXPECT_EQ(lengths.size(), 5);
            EXPECT_EQ(strides.size(), 5);
            EXPECT_EQ(offset, 0);
            EXPECT_TRUE(std::ranges::all_of(lengths.cbegin(), lengths.cend(), [](int i){return i==0;}));
            EXPECT_TRUE(std::ranges::all_of(strides.cbegin(), strides.cend(), [](size_t i){return i==0;}));
        }

        {
            Layout<50> layout;
            auto lengths = layout.lengths();
            auto strides = layout.strides();
            auto offset = layout.offset();
            EXPECT_EQ(lengths.size(), 50);
            EXPECT_EQ(strides.size(), 50);
            EXPECT_EQ(offset, 0);
            EXPECT_TRUE(std::ranges::all_of(lengths.cbegin(), lengths.cend(), [](int i){return i==0;}));
            EXPECT_TRUE(std::ranges::all_of(strides.cbegin(), strides.cend(), [](size_t i){return i==0;}));
        }
    }

    //test for constructor from nested list
    {
        {
            Layout<1> layout{4};
            auto lengths = layout.lengths();
            auto strides = layout.strides();
            auto offset = layout.offset();
            EXPECT_EQ(lengths.size(), 1);
            EXPECT_EQ(strides.size(), 1);
            EXPECT_EQ(offset, 0);
            EXPECT_EQ(layout.length(0),4);
            EXPECT_EQ(layout.stride(0),1); 
        }

        {
            Layout<3> layout{4, 5, 6};
            auto lengths = layout.lengths();
            auto strides = layout.strides();
            auto offset = layout.offset();
            EXPECT_EQ(lengths.size(), 3);
            EXPECT_EQ(strides.size(), 3);
            EXPECT_EQ(offset, 0);
            EXPECT_EQ(layout.length(0),4);
            EXPECT_EQ(layout.length(1),5);
            EXPECT_EQ(layout.length(2),6);
            EXPECT_EQ(layout.stride(0),30);
            EXPECT_EQ(layout.stride(1),6);
            EXPECT_EQ(layout.stride(2),1);
        }

        {
            Layout<4> layout{2, 2, 2, 2};
            auto lengths = layout.lengths();
            auto strides = layout.strides();
            auto offset = layout.offset();
            EXPECT_EQ(lengths.size(), 4);
            EXPECT_EQ(strides.size(), 4);
            EXPECT_EQ(offset, 0);
            EXPECT_TRUE( std::all_of(lengths.cbegin(), lengths.cend(), [](size_t i){return i==2;}) );
            EXPECT_EQ(layout.stride(0),8);
            EXPECT_EQ(layout.stride(1),4);
            EXPECT_EQ(layout.stride(2),2);
            EXPECT_EQ(layout.stride(3),1);
        }
    }


    //test for constructor from sized container
    {
        {
            Layout<4> layout{std::array<size_t, 4>{5, 4, 3, 2}};
            auto lengths = layout.lengths();
            auto strides = layout.strides();
            auto offset = layout.offset();
            EXPECT_EQ(lengths.size(), 4);
            EXPECT_EQ(strides.size(), 4);
            EXPECT_EQ(offset, 0);
            EXPECT_EQ(layout.length(3),2);
            EXPECT_EQ(layout.length(2),3);
            EXPECT_EQ(layout.length(1),4);
            EXPECT_EQ(layout.length(0),5);
            EXPECT_EQ(layout.stride(3),1);
            EXPECT_EQ(layout.stride(2),2);
            EXPECT_EQ(layout.stride(1),6);
            EXPECT_EQ(layout.stride(0),24);
        }
        {
            std::array<size_t, 4> container_lenghts{5, 4, 3, 2};
            Layout<4> layout{container_lenghts};
            auto lengths = layout.lengths();
            auto strides = layout.strides();
            auto offset = layout.offset();
            EXPECT_EQ(lengths.size(), 4);
            EXPECT_EQ(strides.size(), 4);
            EXPECT_EQ(offset, 0);
            EXPECT_EQ(layout.length(3),2);
            EXPECT_EQ(layout.length(2),3);
            EXPECT_EQ(layout.length(1),4);
            EXPECT_EQ(layout.length(0),5);
            EXPECT_EQ(layout.stride(3),1);
            EXPECT_EQ(layout.stride(2),2);
            EXPECT_EQ(layout.stride(1),6);
            EXPECT_EQ(layout.stride(0),24); 
        }
    }

    //test for constructor from resizeable container
    {
        {
            Layout<4> layout{std::vector<size_t>{5, 4, 3, 2}};
            auto lengths = layout.lengths();
            auto strides = layout.strides();
            auto offset = layout.offset();
            EXPECT_EQ(lengths.size(), 4);
            EXPECT_EQ(strides.size(), 4);
            EXPECT_EQ(offset, 0);
            EXPECT_EQ(layout.length(3),2);
            EXPECT_EQ(layout.length(2),3);
            EXPECT_EQ(layout.length(1),4);
            EXPECT_EQ(layout.length(0),5);
            EXPECT_EQ(layout.stride(3),1);
            EXPECT_EQ(layout.stride(2),2);
            EXPECT_EQ(layout.stride(1),6);
            EXPECT_EQ(layout.stride(0),24);
        }
        {
            std::vector<size_t> container_lenghts{5, 4, 3, 2};
            Layout<4> layout{container_lenghts};
            auto lengths = layout.lengths();
            auto strides = layout.strides();
            auto offset = layout.offset();
            EXPECT_EQ(lengths.size(), 4);
            EXPECT_EQ(strides.size(), 4);
            EXPECT_EQ(offset, 0);
            EXPECT_EQ(layout.length(3),2);
            EXPECT_EQ(layout.length(2),3);
            EXPECT_EQ(layout.length(1),4);
            EXPECT_EQ(layout.length(0),5);
            EXPECT_EQ(layout.stride(3),1);
            EXPECT_EQ(layout.stride(2),2);
            EXPECT_EQ(layout.stride(1),6);
            EXPECT_EQ(layout.stride(0),24); 
        }
    }

    // test copy/move constructors
    {
        {
            Layout<4> og_layout(std::array<size_t, 4>{5, 4, 3, 2});
            Layout<4> layout(og_layout);
            auto lengths = layout.lengths();
            auto strides = layout.strides();
            auto offset = layout.offset();
            EXPECT_EQ(lengths.size(), 4);
            EXPECT_EQ(strides.size(), 4);
            EXPECT_EQ(offset, 0);
            EXPECT_EQ(layout.length(3),2);
            EXPECT_EQ(layout.length(2),3);
            EXPECT_EQ(layout.length(1),4);
            EXPECT_EQ(layout.length(0),5);
            EXPECT_EQ(layout.stride(3),1);
            EXPECT_EQ(layout.stride(2),2);
            EXPECT_EQ(layout.stride(1),6);
            EXPECT_EQ(layout.stride(0),24);
        }

        {
            Layout<4> layout(Layout<4> (std::array<size_t, 4>{5, 4, 3, 2}));
            auto lengths = layout.lengths();
            auto strides = layout.strides();
            auto offset = layout.offset();
            EXPECT_EQ(lengths.size(), 4);
            EXPECT_EQ(strides.size(), 4);
            EXPECT_EQ(offset, 0);
            EXPECT_EQ(layout.length(3),2);
            EXPECT_EQ(layout.length(2),3);
            EXPECT_EQ(layout.length(1),4);
            EXPECT_EQ(layout.length(0),5);
            EXPECT_EQ(layout.stride(3),1);
            EXPECT_EQ(layout.stride(2),2);
            EXPECT_EQ(layout.stride(1),6);
            EXPECT_EQ(layout.stride(0),24);
        }
    }

};


/*=================================================================================
                                Assignment Tests
=================================================================================*/
TEST(TestHolor, CheckAssignments){
    {
        Layout<4> og_layout(std::array<size_t, 4>{5, 4, 3, 2});
        Layout<4> layout = og_layout;
        auto lengths = layout.lengths();
        auto strides = layout.strides();
        auto offset = layout.offset();
        EXPECT_EQ(lengths.size(), 4);
        EXPECT_EQ(strides.size(), 4);
        EXPECT_EQ(offset, 0);
        EXPECT_EQ(layout.length(3),2);
        EXPECT_EQ(layout.length(2),3);
        EXPECT_EQ(layout.length(1),4);
        EXPECT_EQ(layout.length(0),5);
        EXPECT_EQ(layout.stride(3),1);
        EXPECT_EQ(layout.stride(2),2);
        EXPECT_EQ(layout.stride(1),6);
        EXPECT_EQ(layout.stride(0),24);
    }

    {
        Layout<4> layout = Layout<4> (std::array<size_t, 4>{5, 4, 3, 2});
        auto lengths = layout.lengths();
        auto strides = layout.strides();
        auto offset = layout.offset();
        EXPECT_EQ(lengths.size(), 4);
        EXPECT_EQ(strides.size(), 4);
        EXPECT_EQ(offset, 0);
        EXPECT_EQ(layout.length(3),2);
        EXPECT_EQ(layout.length(2),3);
        EXPECT_EQ(layout.length(1),4);
        EXPECT_EQ(layout.length(0),5);
        EXPECT_EQ(layout.stride(3),1);
        EXPECT_EQ(layout.stride(2),2);
        EXPECT_EQ(layout.stride(1),6);
        EXPECT_EQ(layout.stride(0),24);
    }
}



/*=================================================================================
                                Assignment Tests
=================================================================================*/
TEST(TestHolor, CheckGetSet){
    {
        Layout<4> layout(std::array<size_t, 4>{5, 4, 3, 2});
        auto lengths = layout.lengths();
        auto strides = layout.strides();
        auto offset = layout.offset();
        EXPECT_EQ(layout.dimensions(), 4);
        EXPECT_EQ(layout.size(), 120);
        EXPECT_EQ(offset, 0);
        EXPECT_EQ(lengths.size(), 4);
        EXPECT_EQ(strides.size(), 4);
        EXPECT_EQ(layout.length(3),2); EXPECT_EQ(layout.length(3),lengths[3]);
        EXPECT_EQ(layout.length(2),3); EXPECT_EQ(layout.length(2),lengths[2]);
        EXPECT_EQ(layout.length(1),4); EXPECT_EQ(layout.length(1),lengths[1]);
        EXPECT_EQ(layout.length(0),5); EXPECT_EQ(layout.length(0),lengths[0]);
        EXPECT_EQ(layout.stride(3),1); EXPECT_EQ(layout.stride(3),strides[3]);
        EXPECT_EQ(layout.stride(2),2); EXPECT_EQ(layout.stride(2),strides[2]);
        EXPECT_EQ(layout.stride(1),6); EXPECT_EQ(layout.stride(1),strides[1]);
        EXPECT_EQ(layout.stride(0),24); EXPECT_EQ(layout.stride(0),strides[0]);
    }
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
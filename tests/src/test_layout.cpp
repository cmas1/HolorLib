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
#include <type_traits>
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
TEST(TestLayout, CheckAssignments){
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
                                Get/Set Tests
=================================================================================*/
TEST(TestLayout, CheckGetSet){
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


TEST(TestLayout, Resize){
    {
        Layout<3> my_layout;
        my_layout.set_lengths(1,2,3);
        EXPECT_EQ(my_layout.length(0),1);
        EXPECT_EQ(my_layout.length(1),2);
        EXPECT_EQ(my_layout.length(2),3);
        my_layout.set_lengths(std::vector<size_t>{1,1,1});
        EXPECT_EQ(my_layout.length(0),1);
        EXPECT_EQ(my_layout.length(1),1);
        EXPECT_EQ(my_layout.length(2),1);
        my_layout.set_lengths(std::array<size_t,3>{2,2,2});
        EXPECT_EQ(my_layout.length(0),2);
        EXPECT_EQ(my_layout.length(1),2);
        EXPECT_EQ(my_layout.length(2),2);
        my_layout.set_length(0,5);
        EXPECT_EQ(my_layout.length(0),5);
        EXPECT_EQ(my_layout.length(1),2);
        EXPECT_EQ(my_layout.length(2),2);
    }
}

/*=================================================================================
                                Indexing Tests
=================================================================================*/
TEST(TestLayout, CheckIndexing){
    {
        Layout<1> layout(3);
        EXPECT_EQ(layout(2), 2);
        EXPECT_EQ(layout(std::array<size_t, 1>{2}), 2);
    }

    {
        Layout<2> layout(3, 3);
        EXPECT_EQ(layout(2, 2), 8);
        EXPECT_EQ(layout(std::array<size_t, 2>{2, 2}), 8);
    }

    {
        Layout<3> layout(3, 3, 3);
        EXPECT_EQ(layout(2, 2, 2), 26);
        EXPECT_EQ(layout(std::array<size_t, 3>{2, 2, 2}), 26);
    }

    {
        Layout<4> layout(3, 3, 3, 3);
        EXPECT_EQ(layout(2, 2, 2, 2), 80);
        EXPECT_EQ(layout(std::array<size_t, 4>{2, 2, 2, 2}), 80);
    }

    {
        Layout<5> layout(3, 3, 3, 3, 3);
        EXPECT_EQ(layout(2, 2, 2, 2, 2), 242);
        EXPECT_EQ(layout(std::array<size_t, 5>{2, 2, 2, 2, 2}), 242);
    }
}


/*=================================================================================
                                Slicing Tests
=================================================================================*/
TEST(TestLayout, CheckSlicing){
    // normal slicing
    {
        Layout<3> layout{3, 3, 3};
        auto test_slice = layout(range(0,1), range(1,2), range(1,2));
        EXPECT_TRUE( (std::is_same_v<decltype(test_slice),Layout<3>>) );
        EXPECT_EQ(test_slice.offset(), 4);
        EXPECT_EQ(test_slice.size(), 8);
        EXPECT_EQ(test_slice.length(0), 2);
        EXPECT_EQ(test_slice.length(1), 2);
        EXPECT_EQ(test_slice.length(2), 2);
        EXPECT_EQ(test_slice.stride(0), 9);
        EXPECT_EQ(test_slice.stride(1), 3);
        EXPECT_EQ(test_slice.stride(2), 1);
    }

    {
        Layout<3> layout{3, 3, 3};
        auto test_slice = layout(1, range(1,2), range(1,2));
        EXPECT_TRUE( (std::is_same_v<decltype(test_slice),Layout<2>>) );
        EXPECT_EQ(test_slice.offset(), 13);
        EXPECT_EQ(test_slice.size(), 4);
        EXPECT_EQ(test_slice.length(0), 2);
        EXPECT_EQ(test_slice.length(1), 2);
        EXPECT_EQ(test_slice.stride(0), 3);
        EXPECT_EQ(test_slice.stride(1), 1);
    }

    // slice unreduced
    {
        Layout<3> layout{3, 3, 3};
        auto test_slice = layout.slice_unreduced(range(0,1), range(1,2), range(1,2));
        EXPECT_TRUE( (std::is_same_v<decltype(test_slice),Layout<3>>) );
        EXPECT_EQ(test_slice.offset(), 4);
        EXPECT_EQ(test_slice.size(), 8);
        EXPECT_EQ(test_slice.length(0), 2);
        EXPECT_EQ(test_slice.length(1), 2);
        EXPECT_EQ(test_slice.length(2), 2);
        EXPECT_EQ(test_slice.stride(0), 9);
        EXPECT_EQ(test_slice.stride(1), 3);
        EXPECT_EQ(test_slice.stride(2), 1);
    }

    {
        Layout<3> layout{3, 3, 3};
        auto test_slice = layout.slice_unreduced(1, range(1,2), range(1,2));
        EXPECT_TRUE( (std::is_same_v<decltype(test_slice),Layout<3>>) );
        EXPECT_EQ(test_slice.offset(), 13);
        EXPECT_EQ(test_slice.size(), 4);
        EXPECT_EQ(test_slice.length(0), 1);
        EXPECT_EQ(test_slice.length(1), 2);
        EXPECT_EQ(test_slice.length(2), 2);
        EXPECT_EQ(test_slice.stride(0), 0);
        EXPECT_EQ(test_slice.stride(1), 3);
        EXPECT_EQ(test_slice.stride(2), 1);
    }

    // slice_dimension
    {
        Layout<3> layout{3, 3, 3};
        auto test_slice = layout.slice_dimension<0>(range(1,2));
        EXPECT_TRUE( (std::is_same_v<decltype(test_slice),Layout<3>>) );
        EXPECT_EQ(test_slice.offset(), 9);
        EXPECT_EQ(test_slice.size(), 18);
        EXPECT_EQ(test_slice.length(0), 2);
        EXPECT_EQ(test_slice.length(1), 3);
        EXPECT_EQ(test_slice.length(2), 3);
        EXPECT_EQ(test_slice.stride(0), 9);
        EXPECT_EQ(test_slice.stride(1), 3);
        EXPECT_EQ(test_slice.stride(2), 1);
    }

    // slice_dimension
    {
        Layout<3> layout{3, 3, 3};
        auto test_slice = layout.slice_dimension<0>(1);
        EXPECT_TRUE( (std::is_same_v<decltype(test_slice),Layout<2>>) );
        EXPECT_EQ(test_slice.offset(), 9);
        EXPECT_EQ(test_slice.size(), 9);
        EXPECT_EQ(test_slice.length(0), 3);
        EXPECT_EQ(test_slice.length(1), 3);
        EXPECT_EQ(test_slice.stride(0), 3);
        EXPECT_EQ(test_slice.stride(1), 1);
    }
}



/*=================================================================================
                                Comparison Tests
=================================================================================*/
TEST(TestLayout, CheckComparisons){
    Layout<2> l1{5,9};
    Layout<2> l2(std::vector{5,9});
    Layout<2> l3(std::vector{9,5});
    EXPECT_TRUE( l1==l2);
    EXPECT_FALSE( l1!=l2);
    EXPECT_FALSE( l1==l3);
    EXPECT_TRUE( l1!=l3);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
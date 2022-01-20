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
            EXPECT_EQ( my_holor.length(0), 0 );
        }        {            Holor<int,3>my_holor;
            Layout<3> tmp_layout;
            auto my_layout = my_holor.layout();
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor.size(), 0 );
            EXPECT_EQ( my_holor.length(0), 0 );
            EXPECT_EQ( my_holor.length(1), 0 );
            EXPECT_EQ( my_holor.length(2), 0 );
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
            EXPECT_EQ( my_holor.length(0), 4 );
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
            EXPECT_EQ( my_holor.length(0), 2 );
            EXPECT_EQ( my_holor.length(1), 2 );
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
            EXPECT_EQ( my_holor.length(0), 2 );
            EXPECT_EQ( my_holor.length(1), 2 );
            EXPECT_EQ( my_holor.length(2), 2 );
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
            EXPECT_EQ( my_holor.length(0), 2 );
            EXPECT_EQ( my_holor.length(1), 2 );
            EXPECT_EQ( my_holor.length(2), 2 );
        }
        {
            std::array<size_t,3> my_lengths{2,2,2};
            Holor<float, 3> my_holor( my_lengths );
            auto my_layout = my_holor.layout();
            Layout<3> tmp_layout{2,2,2};
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor.size(), 8 );
            EXPECT_EQ( my_holor.length(0), 2 );
            EXPECT_EQ( my_holor.length(1), 2 );
            EXPECT_EQ( my_holor.length(2), 2 );
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
            EXPECT_EQ( my_holor.length(0), 2 );
            EXPECT_EQ( my_holor.length(1), 2 );
            EXPECT_EQ( my_holor.length(2), 2 );
        }
        {
            std::vector<size_t> my_lengths{2,2,2};
            Holor<float, 3> my_holor( my_lengths );
            auto my_layout = my_holor.layout();
            Layout<3> tmp_layout{2,2,2};
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor.size(), 8 );
            EXPECT_EQ( my_holor.length(0), 2 );
            EXPECT_EQ( my_holor.length(1), 2 );
            EXPECT_EQ( my_holor.length(2), 2 );
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
        EXPECT_EQ(h.length(0), 2);
        EXPECT_EQ(h.length(1), 3);
        EXPECT_EQ(my_layout.stride(0), 3);
        EXPECT_EQ(my_layout.stride(1), 1);
        EXPECT_EQ(my_layout.offset(), 0);
        auto my_data = h.data_vector();
        EXPECT_DOUBLE_EQ(my_data[0], 1.1);
        EXPECT_DOUBLE_EQ(my_data[1], 2.2);
        EXPECT_DOUBLE_EQ(my_data[2], 3.3);
        EXPECT_DOUBLE_EQ(my_data[3], 4.4);
        EXPECT_DOUBLE_EQ(my_data[4], 5.5);
        EXPECT_DOUBLE_EQ(my_data[5], 6.6);
    }
    
};


/*=================================================================================
                                Assignment Tests
// =================================================================================*/
TEST(TestHolor, CheckAssignments){
    {
        Holor<int,2> h1{{1,2,3}, {4,5,6}};
        Holor<int,2> h2= h1;
        EXPECT_TRUE((h1==h2));
    }
    {
        Holor<int,2> h1{{1,2,3}, {4,5,6}};
        Holor<int,2> h2 = Holor<int,2> {{1,2,3}, {4,5,6}};
        EXPECT_TRUE((h1==h2));
    }
}



/*=================================================================================
                                Get/Set Tests
=================================================================================*/
TEST(TestHolor, CheckGetSet){
    {
        Holor<int, 3> my_holor{ {{1,2}, {3,4}}, {{5,6}, {7,8}} };
        auto my_layout = my_holor.layout();
        EXPECT_TRUE( (my_layout==Layout<3>{2,2,2}) );
        EXPECT_EQ(my_holor.length(0),2);
        EXPECT_EQ(my_holor.length(1),2);
        EXPECT_EQ(my_holor.length(2),2);
        std::array<size_t,3> tmp_lengths{2,2,2};
        EXPECT_EQ(my_holor.lengths(), tmp_lengths);
        EXPECT_EQ(my_holor.size(), 8);
        std::vector<int> tmp_data{1,2,3,4,5,6,7,8};
        EXPECT_EQ(my_holor.data_vector(), tmp_data);
    }
}


TEST(TestHolor, Resize){
    {
        Holor<int, 3> my_holor;
        my_holor.set_lengths(1,2,3);
        EXPECT_EQ(my_holor.length(0),1);
        EXPECT_EQ(my_holor.length(1),2);
        EXPECT_EQ(my_holor.length(2),3);
        my_holor.set_lengths(std::vector<size_t>{1,1,1});
        EXPECT_EQ(my_holor.length(0),1);
        EXPECT_EQ(my_holor.length(1),1);
        EXPECT_EQ(my_holor.length(2),1);
        my_holor.set_lengths(std::array<size_t,3>{2,2,2});
        EXPECT_EQ(my_holor.length(0),2);
        EXPECT_EQ(my_holor.length(1),2);
        EXPECT_EQ(my_holor.length(2),2);
        my_holor.set_length(0,5);
        EXPECT_EQ(my_holor.length(0),5);
        EXPECT_EQ(my_holor.length(1),2);
        EXPECT_EQ(my_holor.length(2),2);
    }
}




/*=================================================================================
                                Indexing Tests
=================================================================================*/
TEST(TestHolor, CheckIndexing){
    Holor<char, 3> my_holor{ {{'a', 'b'}, {'c', 'd'}},  {{'e', 'f'}, {'g', 'h'}}  };
    EXPECT_EQ(my_holor(0,0,0), 'a');
    EXPECT_EQ(my_holor(0,0,1), 'b');
    EXPECT_EQ(my_holor(0,1,0), 'c');
    EXPECT_EQ(my_holor(0,1,1), 'd');
    EXPECT_EQ(my_holor(1,0,0), 'e');
    EXPECT_EQ(my_holor(1,0,1), 'f');
    EXPECT_EQ(my_holor(1,1,0), 'g');
    EXPECT_EQ(my_holor(1,1,1), 'h');
    my_holor(0,0,0) = 'z';
    EXPECT_EQ(my_holor(0,0,0), 'z');
    auto var = my_holor(0,0,1);
    var = 'u';
    EXPECT_EQ(my_holor(0,0,1), 'b');
}



/*=================================================================================
                                Slicing Tests
=================================================================================*/
TEST(TestHolor, CheckSlicing){
    // normal slicing
    {
        Holor<int,3> h1{ {{1,2,3}, {4,5,6}, {7,8,9}}, {{10,11,12}, {13,14,15}, {16,17,18}}, {{19,20,21}, {22,23,24}, {25,26,27}} };
        auto slice1 = h1(range(0,1), range(1,2), range(0,1));
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 3>>) );
        Holor<int,3> h2{ {{4,5}, {7,8}}, {{13,14}, {16,17}} };
        EXPECT_EQ(slice1.length(0), 2);
        EXPECT_EQ(slice1.length(1), 2);
        EXPECT_EQ(slice1.length(2), 2);
        EXPECT_TRUE( (slice1==h2) );
    }

    {
        Holor<int,3> h1{ {{1,2,3}, {4,5,6}, {7,8,9}}, {{10,11,12}, {13,14,15}, {16,17,18}}, {{19,20,21}, {22,23,24}, {25,26,27}} };
        auto slice1 = h1(2, range(1,2), range(0,1));
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{ {22,23}, {25,26} };
        EXPECT_EQ(slice1.length(0), 2);
        EXPECT_EQ(slice1.length(1), 2);
        EXPECT_TRUE( (slice1==h2) );
    }

    {
        Holor<int,3> h1{ {{1,2,3}, {4,5,6}, {7,8,9}}, {{10,11,12}, {13,14,15}, {16,17,18}}, {{19,20,21}, {22,23,24}, {25,26,27}} };
        auto slice1 = h1(2, 1, range(0,1));
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 1>>) );
        Holor<int,1> h2{22,23};
        EXPECT_EQ(slice1.length(0), 2);
        EXPECT_TRUE( (slice1==h2) );
    }

    // row slicing
    {
        Holor<int,3> h1{ {{1,2,3}, {4,5,6}, {7,8,9}}, {{10,11,12}, {13,14,15}, {16,17,18}}, {{19,20,21}, {22,23,24}, {25,26,27}} };
        auto slice1 = h1.row(0);
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{{1,2,3}, {4,5,6}, {7,8,9}};
        EXPECT_EQ(slice1.length(0), 3);
        EXPECT_EQ(slice1.length(1), 3);
        EXPECT_TRUE( (slice1==h2) );
    }

    {
        Holor<int,3> h1{ {{1,2,3}, {4,5,6}, {7,8,9}}, {{10,11,12}, {13,14,15}, {16,17,18}}, {{19,20,21}, {22,23,24}, {25,26,27}} };
        auto slice1 = h1.row(2);
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{{19,20,21}, {22,23,24}, {25,26,27}};
        EXPECT_EQ(slice1.length(0), 3);
        EXPECT_EQ(slice1.length(1), 3);
        EXPECT_TRUE( (slice1==h2) );
    }

    // col slicing
    {
        Holor<int,3> h1{ {{1,2,3}, {4,5,6}, {7,8,9}}, {{10,11,12}, {13,14,15}, {16,17,18}}, {{19,20,21}, {22,23,24}, {25,26,27}} };
        auto slice1 = h1.col(0);
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{ {1,2,3}, {10,11,12}, {19,20,21} };
        EXPECT_EQ(slice1.length(0), 3);
        EXPECT_EQ(slice1.length(1), 3);
        EXPECT_TRUE( (slice1==h2) );
    }

    {
        Holor<int,3> h1{ {{1,2,3}, {4,5,6}, {7,8,9}}, {{10,11,12}, {13,14,15}, {16,17,18}}, {{19,20,21}, {22,23,24}, {25,26,27}} };
        auto slice1 = h1.col(2);
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{ {7,8,9}, {16,17,18}, {25,26,27} };
        EXPECT_EQ(slice1.length(0), 3);
        EXPECT_EQ(slice1.length(1), 3);
        EXPECT_TRUE( (slice1==h2) );
    }

    // dimension slicing
    {
        Holor<int,3> h1{ {{1,2,3}, {4,5,6}, {7,8,9}}, {{10,11,12}, {13,14,15}, {16,17,18}}, {{19,20,21}, {22,23,24}, {25,26,27}} };
        auto slice1 = h1.slice<1>(0);
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{ {1,2,3}, {10,11,12}, {19,20,21} };
        EXPECT_EQ(slice1.length(0), 3);
        EXPECT_EQ(slice1.length(1), 3);
        EXPECT_TRUE( (slice1==h2) );
    }

    {
        Holor<int,3> h1{ {{1,2,3}, {4,5,6}, {7,8,9}}, {{10,11,12}, {13,14,15}, {16,17,18}}, {{19,20,21}, {22,23,24}, {25,26,27}} };
        auto slice1 = h1.slice<2>(2);
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{ {3, 6, 9}, {12, 15, 18}, {21, 24, 27} };
        EXPECT_EQ(slice1.length(0), 3);
        EXPECT_EQ(slice1.length(1), 3);
        EXPECT_TRUE( (slice1==h2) );
    }
}




int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

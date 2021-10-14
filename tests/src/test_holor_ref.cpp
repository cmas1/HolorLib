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
TEST(TestHolorRef, CheckAliases){
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
TEST(TestHolorRef, CheckConstructors){
    //tests for default constructor
    {
        {
            HolorRef<int,1>my_holor_ref;
            Layout<1> tmp_layout;
            auto my_layout = my_holor_ref.layout();
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor_ref.size(), 0 );
            EXPECT_EQ( my_holor_ref.length(0), 0 );
        }
        {
            HolorRef<int,3>my_holor_ref;
            Layout<3> tmp_layout;
            auto my_layout = my_holor_ref.layout();
            EXPECT_EQ(tmp_layout, my_layout);
            EXPECT_EQ( my_holor_ref.size(), 0 );
            EXPECT_EQ( my_holor_ref.length(0), 0 );
            EXPECT_EQ( my_holor_ref.length(1), 0 );
            EXPECT_EQ( my_holor_ref.length(2), 0 );
        }
    }

    
    //test for constructor from datapointer + Layout
    {   
        std::vector<float> datavec{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
        HolorRef<float, 3> hr( datavec.data(), Layout<3>{2,2,2} );
        auto my_layout = hr.layout();
        Layout<3> tmp_layout{2,2,2};
        EXPECT_EQ(tmp_layout, my_layout);
        EXPECT_EQ( hr.size(), 8 );
        EXPECT_EQ( hr.length(0), 2 );
        EXPECT_EQ( hr.length(1), 2 );
        EXPECT_EQ( hr.length(2), 2 );
        Holor<float,3> h{ {{1.0, 2.0}, {3.0, 4.0}}, {{5.0, 6.0}, {7.0, 8.0}}};
        EXPECT_TRUE( hr==h );
    }
    

    //test for constructor from datapointer + sized container
    {   
        std::vector<float> datavec{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
        HolorRef<float, 3> hr( datavec.data(), std::array<size_t,3>{2,2,2});
        auto my_layout = hr.layout();
        Layout<3> tmp_layout{2,2,2};
        EXPECT_EQ(tmp_layout, my_layout);
        EXPECT_EQ( hr.size(), 8 );
        EXPECT_EQ( hr.length(0), 2 );
        EXPECT_EQ( hr.length(1), 2 );
        EXPECT_EQ( hr.length(2), 2 );
        Holor<float,3> h{ {{1.0, 2.0}, {3.0, 4.0}}, {{5.0, 6.0}, {7.0, 8.0}}};
        EXPECT_TRUE( hr==h );
    }

    //test for constructor from datapointer + resizeable container
    {   
        std::vector<float> datavec{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
        HolorRef<float, 3> hr( datavec.data(), std::vector<size_t>{2,2,2});
        auto my_layout = hr.layout();
        Layout<3> tmp_layout{2,2,2};
        EXPECT_EQ(tmp_layout, my_layout);
        EXPECT_EQ( hr.size(), 8 );
        EXPECT_EQ( hr.length(0), 2 );
        EXPECT_EQ( hr.length(1), 2 );
        EXPECT_EQ( hr.length(2), 2 );
        Holor<float,3> h{ {{1.0, 2.0}, {3.0, 4.0}}, {{5.0, 6.0}, {7.0, 8.0}}};
        EXPECT_TRUE( hr==h );
    }


    //copy/move constructors
    {
        std::vector<int> vec{1,2,3,4,5,6};
        HolorRef<int,2> h1(vec.data(), Layout<2>{2,3});
        HolorRef<int,2> h2(h1);
        EXPECT_TRUE((h1==h2));
    }
    {   
        std::vector<int> vec{1,2,3,4,5,6};
        HolorRef<int,2> h1(vec.data(), Layout<2>{2,3});
        HolorRef<int,2> h2( HolorRef<int,2>(vec.data(), Layout<2>{2,3}) );
        EXPECT_TRUE((h1==h2));
    } 
};


/*=================================================================================
//                                 Assignment Tests
// // =================================================================================*/
TEST(TestHolorRef, CheckAssignments){
    {
        std::vector<int> vec{1,2,3,4,5,6};
        HolorRef<int,2> h1(vec.data(), Layout<2>{2,3});
        HolorRef<int,2> h2 = h1;
        EXPECT_TRUE((h1==h2));
    }
    {   
        std::vector<int> vec{1,2,3,4,5,6};
        HolorRef<int,2> h1(vec.data(), Layout<2>{2,3});
        HolorRef<int,2> h2 = HolorRef<int,2>(vec.data(), Layout<2>{2,3});
        EXPECT_TRUE((h1==h2));
    } 
}



/*=================================================================================
//                                 Get/Set Tests
// =================================================================================*/
TEST(TestHolorRef, CheckGetSet){
    {
        std::vector<int> vec_data{1,2,3,4,5,6,7,8};
        HolorRef<int, 3> hr{ vec_data.data(), Layout<3>{2,2,2} };
        auto my_layout = hr.layout();
        EXPECT_TRUE( (my_layout==Layout<3>{2,2,2}) );
        EXPECT_EQ(hr.length(0),2);
        EXPECT_EQ(hr.length(1),2);
        EXPECT_EQ(hr.length(2),2);
        std::array<size_t,3> tmp_lengths{2,2,2};
        EXPECT_EQ(hr.lengths(), tmp_lengths);
        EXPECT_EQ(hr.size(), 8);
        EXPECT_EQ(hr.dataptr(), vec_data.data());
    }
}




/*=================================================================================
//                                 Indexing Tests
// =================================================================================*/
TEST(TestHolorRef, CheckIndexing){
    std::vector<char> vec{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    HolorRef<char, 3> hr(vec.data(), Layout<3>{2,2,2});
    EXPECT_EQ(hr(0,0,0), 'a');
    EXPECT_EQ(hr(0,0,1), 'b');
    EXPECT_EQ(hr(0,1,0), 'c');
    EXPECT_EQ(hr(0,1,1), 'd');
    EXPECT_EQ(hr(1,0,0), 'e');
    EXPECT_EQ(hr(1,0,1), 'f');
    EXPECT_EQ(hr(1,1,0), 'g');
    EXPECT_EQ(hr(1,1,1), 'h');
    hr(0,0,0) = 'z';
    EXPECT_EQ(hr(0,0,0), 'z');
    EXPECT_EQ(vec[0], 'z');
}



/*=================================================================================
//                                 Slicing Tests
// =================================================================================*/
TEST(TestHolorRef, CheckSlicing){
    // normal slicing
    {
        std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
        HolorRef<int,3> h1{ vec.data(), Layout<3>{3,3,3} };
        auto slice1 = h1(range(0,1), range(1,2), range(0,1));
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 3>>) );
        Holor<int,3> h2{ {{4,5}, {7,8}}, {{13,14}, {16,17}} };
        EXPECT_EQ(slice1.length(0), 2);
        EXPECT_EQ(slice1.length(1), 2);
        EXPECT_EQ(slice1.length(2), 2);
        EXPECT_TRUE( (slice1==h2) );
    }

    {
        std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
        HolorRef<int,3> h1{ vec.data(), Layout<3>{3,3,3} };
        auto slice1 = h1(2, range(1,2), range(0,1));
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{ {22,23}, {25,26} };
        EXPECT_EQ(slice1.length(0), 2);
        EXPECT_EQ(slice1.length(1), 2);
        EXPECT_TRUE( (slice1==h2) );
    }

    {
        std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
        HolorRef<int,3> h1{ vec.data(), Layout<3>{3,3,3} };
        auto slice1 = h1(2, 1, range(0,1));
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 1>>) );
        Holor<int,1> h2{22,23};
        EXPECT_EQ(slice1.length(0), 2);
        EXPECT_TRUE( (slice1==h2) );
    }

    // row slicing
    {
        std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
        HolorRef<int,3> h1{ vec.data(), Layout<3>{3,3,3} };
        auto slice1 = h1.row(0);
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{{1,2,3}, {4,5,6}, {7,8,9}};
        EXPECT_EQ(slice1.length(0), 3);
        EXPECT_EQ(slice1.length(1), 3);
        EXPECT_TRUE( (slice1==h2) );
    }

    {
        std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
        HolorRef<int,3> h1{ vec.data(), Layout<3>{3,3,3} };
        auto slice1 = h1.row(2);
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{{19,20,21}, {22,23,24}, {25,26,27}};
        EXPECT_EQ(slice1.length(0), 3);
        EXPECT_EQ(slice1.length(1), 3);
        EXPECT_TRUE( (slice1==h2) );
    }

    // col slicing
    {
        std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
        HolorRef<int,3> h1{ vec.data(), Layout<3>{3,3,3} };
        auto slice1 = h1.col(0);
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{ {1,2,3}, {10,11,12}, {19,20,21} };
        EXPECT_EQ(slice1.length(0), 3);
        EXPECT_EQ(slice1.length(1), 3);
        EXPECT_TRUE( (slice1==h2) );
    }

    {
        std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
        HolorRef<int,3> h1{ vec.data(), Layout<3>{3,3,3} };
        auto slice1 = h1.col(2);
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{ {7,8,9}, {16,17,18}, {25,26,27} };
        EXPECT_EQ(slice1.length(0), 3);
        EXPECT_EQ(slice1.length(1), 3);
        EXPECT_TRUE( (slice1==h2) );
    }

    // dimension slicing
    {
        std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
        HolorRef<int,3> h1{ vec.data(), Layout<3>{3,3,3} };
        auto slice1 = h1.slice<1>(0);
        EXPECT_TRUE( (std::is_same_v<decltype(slice1), HolorRef<int, 2>>) );
        Holor<int,2> h2{ {1,2,3}, {10,11,12}, {19,20,21} };
        EXPECT_EQ(slice1.length(0), 3);
        EXPECT_EQ(slice1.length(1), 3);
        EXPECT_TRUE( (slice1==h2) );
    }

    {
        std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
        HolorRef<int,3> h1{ vec.data(), Layout<3>{3,3,3} };
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

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



#include <holor/holor_full.h>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

using namespace holor;


int main(){
    /*
     * 1) Construct an holor from a nested list
     * A Holor container can be built directly from a nested initializer list of elements.
     */
    Holor<int, 2> my_holor_01{{1, 2, 3}, {4, 5, 6}};
    std::cout << "\n\033[33m Example 1):\033[0m Constructing holor from nested list of elements\n";
    std::cout << "Creating a holor as  \033[32m Holor<int, 2> my_holor_01{{1, 2, 3}, {4, 5, 6}}; \033[0m \n";
    std::cout << "The created container is my_holor_01 = " << my_holor_01 << "\n\n";


    // 2) Construct an holor from another holor
    auto my_holor_02 = my_holor_01;
    Holor<int, 2> my_holor_03{my_holor_01};
    std::cout << "\n\033[33m Example 2):\033[0m Constructing holor from another holor\n";
    std::cout << "Creating a holor as  \033[32m auto my_holor_02 = my_holor_01; \033[0m \n";
    std::cout << "The created container is my_holor_02 = " << my_holor_02 << "\n\n";
    std::cout << "Creating a holor as  \033[32m Holor<int, 2> my_holor_03{my_holor_01}; \033[0m \n";
    std::cout << "The created container is \033[1m my_holor_03 = " << my_holor_03 << "\033[0m \n\n";


    // 3) Construct an empty holor specifying the length of its dimensions with a  standard container
    std::cout << "\n\033[33m Example 3):\033[0m  Construct an empty holor specifying the length of its dimensions with a  standard container\n";
    std::cout << "The legnths can be specified both using a dynamic-lenght container (e.g., a vector) or a fixed-length container (e.g., an array) provided that they must have the same number of elements as the dimensions of the holor\n";


    std::vector<int> length_01{3,6,2};
    Holor<float, 3> empty_holor_01(length_01);
    std::cout << "  \033[32m std::vector<int> length_01{3,6,2}; \033[0m \n";
    std::cout << "  \033[32m Holor<float, 3> empty_holor_01(length_01); \033[0m \n";
    std::cout << "  The lenghts of \033[1m empty_holor_01 \033[0m  along its dimensions are: [";
    for(auto cnt = 0; cnt<3; cnt++){
        std::cout<< "  " << empty_holor_01.length(cnt);
    }
    std::cout << " ]\n\n";


    std::array<int,4> length_02{3,1,6,2};
    Holor<float, 4> empty_holor_02(length_02);
    std::cout << "  \033[32m std::array<int,4> length_02{3,1,6,2}; \033[0m \n";
    std::cout << "  \033[32m Holor<float, 4> empty_holor_02(length_02); \033[0m \n\n";
    std::cout << "  The lenghts of \033[1m empty_holor_02 \033[0m  along its dimensions are: [";
    for(auto cnt = 0; cnt<4; cnt++){
        std::cout<< "  " << empty_holor_02.length(cnt);
    }
    std::cout << " ]\n\n";

    std::cout << "\033[0;95m Note: \033[0m the container of lenghts shall be passed to the constructor using () and not {}, otherwise it would be interpreted as a nested list and it would not compile.\n";
    std::cout << "\033[0;95m Note: \033[0m the container of lenghts shall have the same number of elements as are the dimensions of the container, otherwise it would not compile.\n\n";



    // 4) Construct an empty holor, without specifying its elements or dimensions
    std::cout << "\n\033[33m Example 4):\033[0m  Construct an empty holor without specifying its lengths or elements\n";
    struct my_object{
        float a;
        int b;
        double c;
    };

    Holor<my_object, 3> empty_holor_03;
    std::cout << "  \033[32m Holor<my_object, 3> empty_holor_03; \033[0m (my_object is a generic type, for example a struct) \n";
    std::cout << "  The lenghts of \033[1m empty_holor_03 \033[0m  along its dimensions are: [";
    for(auto cnt = 0; cnt<3; cnt++){
        std::cout<< "  " << empty_holor_03.length(cnt);
    }
    std::cout << " ]\n\n";


    // 5) Construct an HolorRef
    std::cout << "\n\033[33m Example 5):\033[0m  While an Holor owns the elements it stores, an HolorRef provide a Holor-like view to some elements, without taking ownerships of them. Holor_ref can be constructed by providing a pointer to the memory location of the elements as well as i) the lenghts of the view, or ii) a layout\n\n";

    std::vector<int> myvec01{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    HolorRef<int,2> my_holor_ref_01(myvec01.data(), std::vector<int>{5,2});
    std::cout << "\033[32m std::vector<int> myvec01{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; \033[0m \n";
    std::cout << "\033[32m HolorRef<int,2> my_holor_ref_01(myvec01.data(), std::vector<int>{5,2}); \033[0m \n";
    std::cout << "The created container is my_holor_ref_01 = " << my_holor_ref_01 << "\n\n";

    Layout<2> my_layout(std::vector<int>{2,5});
    HolorRef<int,2> my_holor_ref_02(myvec01.data(), my_layout);
    std::cout << "\033[32m Layout<2> my_layout(std::vector<int>{2,5}); \033[0m \n";
    std::cout << "\033[32m HolorRef<int,2> my_holor_ref_02(myvec01.data(), my_layout); \033[0m \n";
    std::cout << "The created container is my_holor_ref_02 = " << my_holor_ref_02 << "\n\n";

    std::cout << "\033[0;95m Note: \033[0m HolorRef do not own their elements. Therefore, any change to the original memory location will affect also the HolorRef, and viceversa.\n";
    std::cout << "For example, let's do \033[32m myvec01[2] = 99; \033[0m.\n";
    myvec01[2] = 99;
    std::cout << "=> my_holor_ref_01 = " << my_holor_ref_01 << "\n";
    std::cout << "=> my_holor_ref_02 = " << my_holor_ref_02 << "\n\n";


    // 6) Construct an Holor from an HolorRef
    std::cout << "\n\033[33m Example 6):\033[0m  Construct an Holor from an HolorRef\n\n";
    std::vector<double> my_vec2{1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
    HolorRef<double,2> hr(my_vec2.data(), Layout<2>{2,3});
    Holor<double,2> h(hr);
    std::cout << "\033[32m std::vector<double> my_vec2{1.1, 2.2, 3.3, 4.4, 5.5, 6.6}; \033[0m \n";
    std::cout << "\033[32m HolorRef<double,2> hr(my_vec2.data(), Layout<2>{2,3}); \033[0m \n";
    std::cout << "\033[32m Holor<double,2> h(hr); \033[0m \n";
    std::cout << "=> h = " << h << "\n\n";


    // 7) Construct an empty Holor and resize it
    std::cout << "\n\033[33m Example 7):\033[0m  Construct an empty Holor and resize it\n\n";
    Holor<double,2>empty_holor;
    empty_holor.set_lengths(2,3);
    std::ranges::fill(empty_holor, 1.3);
    std::cout << "\033[32m Holor<double,2>empty_holor; \033[0m \n";
    std::cout << "\033[32m empty_holor.set_lengths(2,3); \033[0m \n";
    std::cout << "\033[32m std::ranges::fill(empty_holor, 1.3); \033[0m \n";
    std::cout << "=> empty_holor = " << empty_holor << "\n\n";
    std::cout << "Now, let's resize the dimensions individually.\n";
    empty_holor.set_length(0,5);
    empty_holor.set_length(1,2);
    std::cout << "\033[32m empty_holor.set_length(0,5); \033[0m \n";
    std::cout << "\033[32m empty_holor.set_length(1,2); \033[0m \n";
    std::cout << "=> empty_holor = " << empty_holor << "\n\n";

    return 0;
}
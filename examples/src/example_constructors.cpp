// This file is part of Holor, a C++ template library for multi-dimensional containers

// Copyright 2020 Carlo Masone

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



#include <holor/holor_full.h>
#include <iostream>
#include <vector>
#include <array>

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
    std::cout << "The created container is my_holor_03 = " << my_holor_03 << "\n\n";


    // 3) Construct an empty holor specifying the length of its dimensions with a  standard container
    std::cout << "\n\033[33m Example 3):\033[0m  Construct an empty holor specifying the lenght of its dimensions with a  standard container\n";
    std::cout << "The legnths can be specified both using a dynamic-lenght container (e.g., a vector) or a fixed-length container (e.g., an array) provided that they must have the same number of elements as the dimensions of the holor\n";

    std::vector<int> length_01{3,6,2};
    Holor<float, 3> empty_holor_01(length_01);
    std::cout << "  \033[32m std::vector<int> length_01{3,6,2}; \033[0m \n";
    std::cout << "  \033[32m Holor<float, 3> empty_holor_01(length_01); \033[0m \n\n";


    std::array<int,4> length_02{3,1,6,2};
    Holor<float, 4> empty_holor_02(length_02);
    std::cout << "  \033[32m std::array<int,4> length_02{3,1,6,2}; \033[0m \n";
    std::cout << "  \033[32m Holor<float, 4> empty_holor_02(length_02); \033[0m \n\n";

    std::cout << "Note that in this case, the argument to the constructor should be passed using () and not {}, otherwise it would be interpreted as a nested list and it would not compile.\n";


    // 4) Construct an empty holor specifying the lenght of its dimensions with a  standard container

    return 0;
}
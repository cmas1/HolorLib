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

using namespace holor;


int main(){
    
    Holor<int, 2> A{{1, 2, 3}, {4, 5, 6}};
    std::cout << " \033[32m Holor<int, 3> A{{{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}}, {{12, 13, 14, 15}, {16, 17, 18, 19}, {20, 21, 22, 23}}}; \033[0m \n";
    std::cout << "\033[1m A \033[0m = " << A << "\n\n";

    std::vector<int> temp_vec{11,22};
    std::copy(temp_vec.begin(), temp_vec.end(), A.col(0).rbegin());

    std::cout << " \033[32m std::vector<int> temp_vec{11,22}; \033[0m \n";
    std::cout << " \033[32m std::copy(temp_vec.begin(), temp_vec.end(), A.col(0).rbegin()); \033[0m \n";
    std::cout << "\033[1m A \033[0m = " << A << "\n\n";




    return 0;
}
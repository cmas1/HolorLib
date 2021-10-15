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




#include <holor/holor_full.h>
#include <iostream>
#include <vector>
#include <array>

using namespace holor;


int main(){
    Holor<int, 3> A{{{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}}, {{12, 13, 14, 15}, {16, 17, 18, 19}, {20, 21, 22, 23}}};
    std::cout << " Holor<int, 3> A{{{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}}, {{12, 13, 14, 15}, {16, 17, 18, 19}, {20, 21, 22, 23}}}; \n";
    std::cout << "A = " << A << "\n\n";


    //an Holor can be accessed element by element
    std::cout << "A(0,1,2) = "  << A(0,1,2) << "\n";

    //an Holor can be sliced
    std::cout << "A(range(0,1), range(1,2), range(2,3)) = "<< A(range(0,1), range(1,2), range(2,3)) << "\n";
    std::cout << "A(0, range(0,1), range(2,3)) = "<< A(0, range(0,1), range(2,3)) << "\n\n";


    //there are other functions that allow to slice an Holor
    std::cout << "A.row(0) = "<< A.row(0) << "\n";
    std::cout << "A.col(1) = "<< A.col(1) << "\n";
    std::cout << "A.slice<2>(3) = "<< A.slice<2>(3) << "\n\n";




    return 0;
}
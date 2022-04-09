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
    Holor<int, 2> A;
    A.set_lengths(4,3);
    std::ranges::fill(A,1);
    std::cout << "A = " << A << "\n\n";

    Holor<int, 1> B{1,2,3};
    Holor<int, 1> C{8,7,6,5};
    std::cout << "B = " << B << "\n";
    std::cout << "C = " << C << "\n";
    
    holor::multiply_dim<1>(A, B);
    holor::multiply_dim<0>(A, C);


    std::cout<< "A = " << A << "\n\n";

    return 0;
}
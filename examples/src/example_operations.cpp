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

    std::cout << "sum(A) = " << holor::reduce_all(A, 0, std::plus{}) << "\n";
    std::cout << "mul(A) = " << holor::reduce_all(A, 1, std::multiplies{}) << "\n";
    std::cout << "sum<0>(A) = " <<  holor::reduce<0>(A, Holor<int, 1>{0,0,0}, std::plus{}) << "\n";
    std::cout << "sum<1>(A) = " <<  holor::reduce<1>(A, Holor<int, 1>{0,0,0, 0}, std::plus{}) << "\n\n";

    Holor<int, 1> B{1,2,3};
    Holor<int, 1> C{8,7,6,5};
    std::cout << "B = " << B << "\n";
    std::cout << "C = " << C << "\n";

       
    holor::broadcast<0>(A, B, [](auto& a, auto b){return a*b;});
    holor::broadcast<1>(A, C, std::multiplies{});
    std::cout<< "A = " << A << "\n\n";

    std::cout<< "transpose(A) = " << transpose(A,std::array<size_t,2>{1,0}) << "\n";
    auto AT = transpose_view(A);
    std::cout<< "AT = transpose_view(A) = " << AT << "\n";
    AT(1,2)=1212;
    std::cout<< "AT(1,2) = 1212; A = " << A << "\n\n";

    holor::broadcast_all(A, 2, std::multiplies{});
    std::cout<< "A = " << A << "\n\n";

    holor::apply(A, [](auto& a){return a/2;});
    std::cout<< "A = " << A << "\n\n";

    std::cout<< "shift<0>(A,2) = " << shift<0>(A,2) << "\n";
    std::cout<< "shift<0>(A,-1) = " << shift<0>(A,-1) << "\n\n";



    Holor<int,1> pippo{1,1,1};
    Holor<int,1> minni{2,2,2};
    Holor<int,2> pluto {{2,3}, {2,3}, {2,3}};
    Holor<int,2> paperino{{4,5}, {4,5}, {4,5}};;
    std::cout << "concatenate<0>(pippo,minni) = " << concatenate<0>(pippo,minni) << "\n";
    std::cout << "concatenate<1>(pluto,paperino) = " << concatenate<1>(pluto,paperino) << "\n\n";
    // concatenate<0>(pippo,paperino);
    // std::cout << "concatenate(pippo,pluto) = " << concat<1>(pippo,pluto) << "\n";

    return 0;
}
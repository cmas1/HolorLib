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


#include "../../include/holor/holor.h"
#include "../../include/holor/holor_ref.h"
#include "../../include/holor/slice.h"
#include <iostream>
#include <vector>
#include <ranges>



using namespace holor;


int main(){
    Slice<5> pippo({4, 2, 3, 1, 6}, 5);
    std::cout << "N = " << pippo.dimensions() << "\n";
    std::cout << "lengths = ";
    for (auto x : pippo.lengths()){
        std::cout << x << "   ";
    }
    std::cout <<"\n";
    std::cout << "strides = ";
    for (auto x : pippo.strides()){
        std::cout << x << "   ";
    }

    std::cout << "\nsize = " << pippo.size();
    std::cout << "\noffset = " << pippo.offset();
    std::cout << "\n\n";


    Holor<int, 2> pluto{{1, 2, 3}, {4, 5, 6}};
    // std::cout << "N = " << pluto.descriptor().dimensions() << "\n";
    std::cout << "lengths = ";
    for (auto x : pluto.lengths()){
        std::cout << x << "   ";
    }
    std::cout <<"\n";
    for (auto x : pluto.data_vector()){
        std::cout << x << ",  ";
    }
    std::cout << "\n\n";

    std::cout << "pluto(0,2) = " << pluto(0,2) << "\n";
    std::cout << "pluto(1,1) = " << pluto(1,1) << "\n";
    const int topolino = pluto(0,1);
    std::cout << "pluto(0,1) = " << topolino << "\n\n";




    auto my_slice = pluto.descriptor();
    auto bubu = impl::slice_dim<0>();
    Slice<1> row = bubu(0,my_slice);
    std::cout << "pluto.col(0) = " << pluto.data()[row(0)] << ", " << pluto.data()[row(1)]  << "\n";


    HolorRef<int,2> pluto_ref(pluto.descriptor(), pluto.data());
    std::cout << "pluto_ref(0,2) = " << pluto_ref(0,2) << "\n";
    std::cout << "pluto_ref(1,1) = " << pluto_ref(1,1) << "\n";

    pluto_ref(0,2) = 11;
    std::cout << "pluto(0,2) = " << pluto(0,2) << "\n";

    return 0;
}
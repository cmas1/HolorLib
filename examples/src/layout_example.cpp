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


#include "../../include/holor/layout.h"
#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>



using namespace holor;


int main(){

    Layout<2> A{{3,4},0};
    std::cout << "A:\nlengths = [" << A.lengths()[0] << ", " << A.lengths()[1] << "]\n";
    std::cout << "strides = [" << A.strides()[0] << ", " << A.strides()[1] << "]\n";
    std::cout << "offset = " << A.offset() << "\n\n";

    std::cout << "A(1,0) = "  << A(1,0) <<"\n\n";

    auto B = A.slice_dimension<0>({1,2});
    std::cout << "B:\nlengths = [" << B.lengths()[0] << ", " << B.lengths()[1] << "]\n";
    std::cout << "strides = [" << B.strides()[0] << ", " << B.strides()[1] << "]\n";
    std::cout << "offset = " << B.offset() << "\n\n";

    // auto C = A.slice_dimension(1, {0,1});
    // std::cout << "C:\nlengths = [" << C.lengths()[0] << ", " << C.lengths()[1] << "]\n";
    // std::cout << "strides = [" << C.strides()[0] << ", " << C.strides()[1] << "]\n";
    // std::cout << "offset = " << C.offset() << "\n\n";

    // auto D = A.slice_dimension(0, 2);
    // std::cout << "D:\nlengths = [" << D.lengths()[0] << "]\n";
    // std::cout << "strides = [" << D.strides()[0] << "]\n";
    // std::cout << "offset = " << D.offset() << "\n\n";

    // auto E = A.slice_dimension(1, 1);
    // std::cout << "E:\nlengths = [" << E.lengths()[0] << "]\n";
    // std::cout << "strides = [" << E.strides()[0] << "]\n";
    // std::cout << "offset = " << E.offset() << "\n\n";


    auto F = A(range{0,1}, range{1,2});
    // std::cout << "F:\nlengths = [" << F.lengths()[0] << ", " << F.lengths()[1] << "]\n";
    // std::cout << "strides = [" << F.strides()[0] << ", " << F.strides()[1] << "]\n";
    // std::cout << "offset = " << F.offset() << "\n";
    // std::cout << "dimensions = " << F.dimensions() << "\n\n";


    Layout<1> pluto({5});
    auto res = pluto(1);
    std::cout << "res = " << res <<"\n\n";



    // std::cout << "\n\n TEST SET LENGTH \n\n";
    // std::cout << "A: lengths = [" << A.lengths()[0] << ", " << A.lengths()[1] << "]\n";
    // std::cout << "After setting the length to [5,5]\n";
    // A.set_lengths({5,5});
    // std::cout << "A: lengths = [" << A.lengths()[0] << ", " << A.lengths()[1] << "]\n";


    // std::array<int, 5> pluto {1,2,3,4,5};
    // auto bluto = std::views::all(pluto);
    // std::array<int, 4> minni;
    // std::ranges::copy(std::ranges::subrange(pluto.begin() + 1, pluto.end()), minni.begin());
    // for(auto&& x : minni){
    //     std::cout << x << " ";
    // }
    // std::cout<<"\n\n";


    // auto G = A.slice_layout(0, 1, range{1,2});
    // std::cout << "G:\nlengths = [" << G.lengths()[0] << "]\n";
    // std::cout << "strides = [" << G.strides()[0] << "]\n";
    // std::cout << "offset = " << G.offset() << "\n";
    // std::cout << "dimensions = " << G.dimensions() << "\n\n";

    // auto H = A.slice_layout(0, range{1,2}, 0);
    // std::cout << "H:\nlengths = [" << H.lengths()[0] << "]\n";
    // std::cout << "strides = [" << H.strides()[0] << "]\n";
    // std::cout << "offset = " << H.offset() << "\n";
    // std::cout << "dimensions = " << H.dimensions() << "\n\n";





    // Layout<3> AA{{5,6,3},0};
    // auto BB = AA.slice_layout(0, range{1,3}, range{3,5}, range{0,1});
    // auto CC = AA.slice_layout(0, range{3,4}, range{0,1}, 0);

    // std::cout << "BB:\nlengths = [" << BB.lengths()[0] << ", " << BB.lengths()[1] << ", " << BB.lengths()[2] << "]\n";
    // std::cout << "strides = [" << BB.strides()[0] << ", " << BB.strides()[1] << ", " << BB.strides()[2] << "]\n";
    // std::cout << "offset = " << BB.offset() << "\n";
    // std::cout << "dimensions = " << BB.dimensions() << "\n\n";

    // std::cout << "CC:\nlengths = [" << CC.lengths()[0] << ", " << CC.lengths()[1] << "]\n";
    // std::cout << "strides = [" << CC.strides()[0] << ", " << CC.strides()[1] << "]\n";
    // std::cout << "offset = " << CC.offset() << "\n";
    // std::cout << "dimensions = " << CC.dimensions() << "\n\n";

    return 0;
}
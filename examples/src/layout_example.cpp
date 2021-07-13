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

#include "../../include/common/static_assert.h"


using namespace holor;


int main(){

    Layout<2> A{std::vector<int>{2,3}};
    std::cout << "A:\nlengths = [" << A.lengths()[0] << ", " << A.lengths()[1] << "]\n";
    std::cout << "strides = [" << A.strides()[0] << ", " << A.strides()[1] << "]\n";
    std::cout << "offset = " << A.offset() << "\n\n";

    auto A_row = A.slice_dimension<0>(0);
    std::cout << "A_row:\nlengths = [" << A_row.lengths()[0] << "]\n";
    std::cout << "strides = [" << A_row.strides()[0] << "]\n";
    std::cout << "offset = " << A_row.offset() << "\n\n";
    std::cout << "A_row(0) = " << A_row(0) << "\n";
    std::cout << "A_row(1) = " << A_row(1) << "\n";
    std::cout << "A_row(2) = " << A_row(2) << "\n\n";

    auto A_row_fix = A.slice_unreduced(0, range{0,2});
    std::cout << "A_row_fix:\nlengths = [" << A_row_fix.lengths()[0] << ", " << A_row_fix.lengths()[1] <<"]\n";
    std::cout << "strides = [" << A_row_fix.strides()[0] << ", " << A_row_fix.strides()[1] <<"]\n";
    std::cout << "offset = " << A_row_fix.offset() << "\n\n";
    std::cout << "A_row_fix(0) = " << A_row_fix(0,0) << "\n";
    std::cout << "A_row_fix(1) = " << A_row_fix(0,1) << "\n";
    std::cout << "A_row_fix(2) = " << A_row_fix(0,2) << "\n\n";

    // Layout<2> A{{3,4},0};
    // std::cout << "A:\nlengths = [" << A.lengths()[0] << ", " << A.lengths()[1] << "]\n";
    // std::cout << "strides = [" << A.strides()[0] << ", " << A.strides()[1] << "]\n";
    // std::cout << "offset = " << A.offset() << "\n\n";

    // std::cout << "A(1,0) = "  << A(1,0) <<"\n\n";

    // auto B = A.slice_dimension<0>({1,2});
    // std::cout << "B:\nlengths = [" << B.lengths()[0] << ", " << B.lengths()[1] << "]\n";
    // std::cout << "strides = [" << B.strides()[0] << ", " << B.strides()[1] << "]\n";
    // std::cout << "offset = " << B.offset() << "\n\n";

    // auto C = A.slice_dimension<1>({0,1});
    // std::cout << "C:\nlengths = [" << C.lengths()[0] << ", " << C.lengths()[1] << "]\n";
    // std::cout << "strides = [" << C.strides()[0] << ", " << C.strides()[1] << "]\n";
    // std::cout << "offset = " << C.offset() << "\n\n";

    // auto D = A.slice_dimension<0>(2);
    // std::cout << "D:\nlengths = [" << D.lengths()[0] << "]\n";
    // std::cout << "strides = [" << D.strides()[0] << "]\n";
    // std::cout << "offset = " << D.offset() << "\n\n";

    // auto E = A.slice_dimension<1>(1);
    // std::cout << "E:\nlengths = [" << E.lengths()[0] << "]\n";
    // std::cout << "strides = [" << E.strides()[0] << "]\n";
    // std::cout << "offset = " << E.offset() << "\n\n";


    auto F = A(range{0,1}, range{1,2});
    std::cout << "F:\nlengths = [" << F.lengths()[0] << ", " << F.lengths()[1] << "]\n";
    std::cout << "strides = [" << F.strides()[0] << ", " << F.strides()[1] << "]\n";
    std::cout << "offset = " << F.offset() << "\n";
    std::cout << "dimensions = " << F.dimensions() << "\n\n";



    Layout<2> AAA{std::array<int,2>{2,3}};
    Layout<3> BBB{std::vector<int>{2,3,5}};
    Layout<2> CCC(2,3);
    std::cout << "CCC:\nlengths = [" << CCC.lengths()[0] << ", " << CCC.lengths()[1] << "]\n";
    std::cout << "strides = [" << CCC.strides()[0] << ", " << CCC.strides()[1] << "]\n";
    std::cout << "offset = " << CCC.offset() << "\n\n";

    return 0;
}
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
#include "../../include/holor/holor_extra.h"
#include <iostream>
#include <vector>
#include <ranges>


using namespace holor;


int main(){
    
    Holor<int, 2> pluto{{1, 2, 3}, {4, 5, 6}};

    std::cout << "pluto = " << pluto << "\n\n";
    
    std::cout << "row0 = " << pluto.row(0) << "\n";
    std::cout << "row1 = " << pluto.row(1) << "\n";
    std::cout << "col0 = " << pluto.col(0) << "\n";
    std::cout << "col1 = " << pluto.col(1) << "\n\n";

    std::cout << "pluto(1,2) = " << pluto(1,2) << "\n";
    std::cout << "pluto(1,{1,2}) = " << pluto(1,range(1,2)) << "\n\n";
    std::cout << "pluto({0,1},{1,2}) = " << pluto(range{0,1}, range(1,2)) << "\n\n";

     return 0;
}
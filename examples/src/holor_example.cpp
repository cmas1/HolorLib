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

    std::cout << "pluto.row(0)[0] = " << pluto.row(0)(0) << "\n";
    std::cout << "pluto.row(0)[1] = " << pluto.row(0)(1) << "\n";
    std::cout << "pluto.row(0)[2] = " << pluto.row(0)(2) << "\n";
    
    std::cout << "pluto.lengths()[0] = " << pluto.lengths()[0] << "\n";
    std::cout << "pluto.lengths()[1] = " << pluto.lengths()[1] << "\n";

    // std::cout << "pluto = " << pluto << "\n\n";
    
    // auto pippo = pluto.data_vector();
    // std::cout << "[";
    // for (auto& x:pippo ){
    //     std::cout << x << ", ";
    // }
    // std::cout << "]\n\n";
    // return 0;
}
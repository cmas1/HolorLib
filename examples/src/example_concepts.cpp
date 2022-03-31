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

using namespace holor;


int main(){

    Layout<2> simple_layout;
    std::vector<float> simple_vector;
    std::cout << " \033[32m Layout<2> simple_layout; \033[0m \n";
    std::cout << " \033[32m std::vector<float> simple_vector; \033[0m \n";
    std::cout << "  LayoutType<decltype(simple_layout)> = \033[33m" << LayoutType<decltype(simple_layout)> << "\033[0m\n";
    std::cout << "  LayoutType<decltype(&simple_layout)> = \033[33m" << LayoutType<decltype(&simple_layout)> << "\033[0m\n";
    std::cout << "  DecaysToLayoutType<Layout<2>&)> = \033[33m" << DecaysToLayoutType<Layout<2>&> << "\033[0m\n";
    std::cout << "  LayoutType<decltype(simple_vector)> = \033[33m" << LayoutType<decltype(simple_vector)> << "\033[0m\n\n";


    Holor<int,1> simple_holor1D;
    Holor<int,2> simple_holor2D{{1,2,3},{4,5,6}};
    std::vector<int> datavec{1,2,3,4};
    HolorRef<int,2> simple_HolorRef2D(datavec.data(), Layout<2>{2,2});
    std::cout << " \033[32m Holor<int,1> simple_holor1D; \033[0m \n";
    std::cout << " \033[32m Holor<int,2> simple_holor2D{{1,2,3},{4,5,6}}; \033[0m \n";
    std::cout << " \033[32m std::vector<int> datavec{1,2,3,4}; \033[0m \n";
    std::cout << " \033[32m HolorRef<int,2> simple_HolorRef2D(datavec.data(), Layout<2>{2,2}); \033[0m \n";
    std::cout << "  HolorType<decltype(simple_holor1D)> = \033[33m" << HolorType<decltype(simple_holor1D)> << "\033[0m\n";
    std::cout << "  HolorType<decltype(simple_holor2D)> = \033[33m" << HolorType<decltype(simple_holor2D)> << "\033[0m\n";
    std::cout << "  HolorType<decltype(datavec)> = \033[33m" << HolorType<decltype(datavec)> << "\033[0m\n";
    std::cout << "  HolorType<decltype(simple_HolorRef2D)> = \033[33m" << HolorType<decltype(simple_HolorRef2D)> << "\033[0m\n";
    std::cout << "  HolorType<HolorRef<int,2>> = \033[33m" << HolorType<HolorRef<int,2>> << "\033[0m\n";
    std::cout << "  HolorType<HolorRef<int,2>&> = \033[33m" << HolorType<HolorRef<int,2>&> << "\033[0m\n";
    std::cout << "  DecaysToHolorType<HolorRef<int,2>&> = \033[33m" << DecaysToHolorType<HolorRef<int,2>&> << "\033[0m\n\n";


    return 0;

    
}
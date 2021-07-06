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

#include "../../include/holor/holor_ref_iterator.h"

using namespace holor;


int main(){
    
    Holor<int, 2> pluto{{1, 2, 3}, {4, 5, 6}};
    std::cout << "pluto = " << pluto << "\n\n";
    
    std::vector<int> temp_vec{11,22};

    auto paperino = pluto.col(1);
    HolorRef<int,1> topolino{temp_vec.data(), Layout<1>{{2}} };

    impl::HRef_iterator<int,1> paperino_begin(paperino, impl::begin_iterator_tag{});
    impl::HRef_iterator<int,1> paperino_end(paperino, impl::end_iterator_tag{});
    impl::HRef_iterator<int,1> topolino_begin{topolino, impl::begin_iterator_tag{}};
    impl::HRef_iterator<int,1> topolino_end{topolino, impl::end_iterator_tag{}};

    std::copy(topolino_begin, topolino_end, paperino_begin);

    std::cout << "pluto = " << pluto << "\n\n";

    return 0;
}
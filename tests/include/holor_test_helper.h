// This file is part of BSTools, a C++ template library for B-Splines

// Copyright 2020 Carlo Masone

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



#ifndef TEST_POINT_IMPL_H
#define TEST_POINT_IMPL_H

#include <holor/holor_full.h>

using namespace holor;
namespace test{


    template<typename T, size_t N>
    class TestPointNHelper{
        public:
            // TestPointHelper(){};
 
                     
            // template<typename Container>
            // static bool check_constructor(Point<T, N> point, Container values){
            //     auto data = point.data();

            //     /* if the created polynomial has more coefficiente than the input
            //     * to the constructor, the constructor has failed.
            //     */
            //     if(data.size() != N){
            //         return false;
            //     }

            //     /* if the values of the coefficients of the created polynomial are different
            //     * from those provided to the constructor, the constructor has failed.
            //     */
            //     auto input_it = values.begin();
            //     auto input_end_it = values.end();
            //     for(auto it = data.begin(); it!= data.end(); ++it){
            //         if(*it!= *input_it){
            //             return false;
            //         }
            //         ++input_it;
            //     }

            //     return true;
            // }

    };


    
} // namespace test

#endif

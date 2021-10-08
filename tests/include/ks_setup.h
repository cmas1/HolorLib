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

#ifndef KS_SETUP_H
#define KS_SETUP_H

#include <vector>

namespace bst{
namespace test{



    class KSTestHelper{
        public: 
                     
            template<typename KnotsSequence, typename Knots>
            static bool check_ks_constructor(KnotsSequence ks, Knots test_knots, std::vector<typename KnotsSequence::value_type> test_uknots, std::vector<size_t> test_interval_indices){
                auto knots = ks.knots();
                auto uknots = ks.unique_knots();
                auto interval_indices = ks.interval_indices();
                auto num_intervals = ks.num_intervals();
                auto num_knots = ks.num_knots();

                // check that the knots have the right dimension
                if( (knots.size()!=test_knots.size()) || (knots.size()!= num_knots) ){
                    return false;
                }

                // verify the values of the knots
                {
                    auto input_it = knots.begin();
                    for(auto it = test_knots.begin(); it!= test_knots.end(); ++it){
                        if(*it!= *input_it){
                            return false;
                        }
                        ++input_it;
                    }
                }


                // check that the unique knots have the right dimension
                if( (uknots.size()!=test_uknots.size()) ){
                    return false;
                }

                // verify the values of the unique knots
                {
                    auto input_it = uknots.begin();
                    for(auto it = test_uknots.begin(); it!= test_uknots.end(); ++it){
                        if(*it!= *input_it){
                            return false;
                        }
                        ++input_it;
                    }
                }



                // check that the interval_indices have the right dimension
                if( (interval_indices.size()!=test_interval_indices.size()) || (interval_indices.size()!=num_intervals)){
                    return false;
                }

                // verify the values of the unique knots
                {
                    auto input_it = interval_indices.begin();
                    for(auto it = test_interval_indices.begin(); it!= test_interval_indices.end(); ++it){
                        if(*it!= *input_it){
                            return false;
                        }
                        ++input_it;
                    }
                }

                return true;
            }
    };


    
} // namespace test
} // namespace bst

#endif

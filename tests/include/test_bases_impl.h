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



#ifndef TEST_BASES_IMPL_H
#define TEST_BASES_IMPL_H

#include "../../include/bspline/bs_basis.h"

namespace bst{
namespace test{


    template<typename Base>
    class TestBasesHelper{
        using Poly = typename Base::Poly;
        using value_type = typename Poly::value_type;

        public:
            static bool check_polybase(Poly p1, Poly p2, value_type epsilon){
                auto coeffs1 = p1.coeffs();
                auto coeffs2 = p2.coeffs();

                if(coeffs1.size() != coeffs2.size()){
                    return false;
                }

                for(auto i = 0; i < coeffs1.size(); i++){
                    if( fabs(coeffs1[i]-coeffs2[i])>epsilon ){
                        return false;
                    }
                }
                return true;
            }


            static bool check_evalbase(Base base, value_type k, value_type epsilon){
                auto evaluation = base.eval_active(k);

                value_type tot = 0;
                for (auto elem : evaluation){
                    if (elem.second < 0){
                        return false;
                    }
                    tot += elem.second;
                }

                if(std::fabs(tot-1)>epsilon){
                    return false;
                }

                return true;
            }



            static bool check_periodic_evaluation(Base base, value_type epsilon){
                auto knots = base.knots_sequence();
                auto k_start = knots.knot(0);
                auto k_end = knots.knot(knots.num_knots()-1);
                auto evaluation0 = base.eval_all(k_start);
                auto evaluation1 = base.eval_all(k_end);

                for (auto i = 0; i<=base.degree();  i++){
                    if (std::fabs(evaluation0[i]-evaluation1[i])>epsilon ){
                        return false;
                    }
                }

                return true;
            }


            static bool check_bounded_evaluation(Base base, value_type epsilon){
                auto knots = base.knots_sequence();
                auto k_start = knots.knot(0);
                auto k_end = knots.knot(knots.num_knots()-1);
                auto evaluation0 = base.eval_active(k_start);
                auto evaluation1 = base.eval_active(k_end);

                if (evaluation0[0].second!=1){
                    return false;
                }

                if (evaluation1[base.degree()].second!=1){
                    return false;
                }

                return true;
            }

    };


    
} // namespace test
} // namespace bst

#endif

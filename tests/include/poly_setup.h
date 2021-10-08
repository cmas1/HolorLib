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

#ifndef POLY_SETUP_H
#define POLY_SETUP_H

#include "../../include/polynomials/polynomial.h"

namespace bst{
namespace test{


    template<typename T>
    class PolyTestHelper{
        public:
            // PolyTestHelper(){};
 
                     
            template<typename Coeffs>
            static bool check_poly_constructor(Polynomial<T> poly, Coeffs coeffs){
                auto poly_coeffs = poly.coeffs();
                auto poly_degree = poly.degree();

                /* if the created polynomial has more coefficiente than the input
                * to the constructor, the constructor has failed.
                */
                if(coeffs.size()<poly_coeffs.size()){
                    return false;
                }

                /* if the values of the coefficients of the created polynomial are different
                * from those provided to the constructor, the constructor has failed.
                */
                auto input_it = coeffs.begin();
                auto input_end_it = coeffs.end();
                for(auto it = poly_coeffs.begin(); it!= poly_coeffs.end(); ++it){
                    if(*it!= *input_it){
                        return false;
                    }
                    ++input_it;
                }

                // check that the  degree is ok
                if(poly_coeffs.size()-1!=poly_degree){
                    return false;
                }

                if(coeffs.size()-1 != poly_degree){
                    while(input_it != input_end_it){
                        if(*input_it!=0){
                            return false;
                        }
                        ++input_it;
                    }      
                }

                return true;
            }


            static bool check_dv_constructor(size_t deg, T val){
                Polynomial<T> poly(deg, val);
                
                if(val != 0){
                    /* if the created polynomial has more coefficiente than the input
                    * to the constructor, the constructor has failed.
                    */
                    if(poly.coeffs().size()!=deg+1){
                        return false;
                    }

                    // check that the  degree is ok
                    if(poly.degree()!=deg){
                        return false;
                    }

                    //check the values of the coefficients
                    for (auto coeff : poly.coeffs()){
                        if (coeff != val){
                            return false;
                        }
                    }
                    return true;
                }else{
                    if(poly.coeffs().size() != 1){
                        return false;
                    }
                    if(poly.degree() != 0){
                        return false;
                    }
                    if(poly.coeffs()[0] != 0){
                        return false;
                    }
                    return true;
                }
            }

            static bool check_dv_constructor(size_t deg){
                return check_dv_constructor(deg, 0);
            }


    };


    
} // namespace test
} // namespace bst

#endif

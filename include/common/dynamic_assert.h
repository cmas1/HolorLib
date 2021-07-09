// This file is part of Holor, a C++ template library for multi-dimensional containers

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



#ifndef HOLOR_DYNAMIC_ASSERT_H
#define HOLOR_DYNAMIC_ASSERT_H

/** \file dynamic_assert.h
 * \brief Dynamic assertions.
 *
 * This header contains utils related to dynamic assertions used within BSTools.
 */

#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include "exceptions.h"


namespace holor{

namespace assert{



enum AssertionLevel{
    no_checks = 0,
    release = 1,
    debug = 2
};



// DEFINE_EXCEPTION_LEVEL comes from -Doption
constexpr int current_level = DEFINE_ASSERT_LEVEL;
constexpr int default_level = 1;


constexpr bool assertion_level(int n){
    return (n <= current_level);
}

//TODO: check that there is no computational overhead when cond is false
template<bool cond = assertion_level(default_level), typename Exception = holor::exception::HolorRuntimeError>
void dynamic_assert(bool assertion, const std::string& message = "Dynamic assertion failed."){
    if (assertion){
        return;
    } else {
        throw Exception(message);
    }
}


// If condition is false (the assertion level is higher than the current level chosen) do nothing
template<>
void dynamic_assert<false, holor::exception::HolorRuntimeError>(bool, const std::string&){}


// If condition is false (the assertion level is higher than the current level chosen) do nothing
template<>
void dynamic_assert<false, holor::exception::HolorInvalidArgument>(bool, const std::string&){}



} //namespace holor

} //namespace assert


#endif // HOLOR_DYNAMIC_ASSERT_H
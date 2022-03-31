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



#ifndef RUNTIME_ASSERTIONS_H
#define RUNTIME_ASSERTIONS_H

/** \file runtime_assertions.h
 * \brief This header contains utils related to dynamic assertions.
 */

#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include "exceptions.h"


namespace holor{

namespace assert{


/*!
 * \brief enumeration of the various levels that are used to verify whether a dynamic_asertion check should be performed or not. Namely, a dynamic assertion is performed if its AssertionLevel is less or equal to the current assertion level
 */
enum AssertionLevel{
    no_checks = 0,
    release = 1,
    debug = 2
};

constexpr int current_level = DEFINE_HOLOR_ASSERT_LEVEL; ///< \brief DEFINE_EXCEPTION_LEVEL comes from -Doption and it is specified in the CMakeLists.txt file
constexpr int default_level = 1; ///< \brief By default, assertions are set to the level 1 (i.e., release)


/*!
 * \brief function that checks if an assertion needs to be verified given the current level
 * \param n is the level of the assertion
 * \return true if the assertion should be evaluated
 */
constexpr bool assertion_level(int n){
    return (n <= current_level);
}


/*!
 * \brief function that checks an assertion and throws an exception if it is not verified.
 * \b Example:
 * \verbatim embed:rst:leading-asterisk
 *  .. code::
 *      using namespace holor;
 *      int a = 5; int b = 7;
 *      assert::dynamic_assert( a < b, EXCEPTION_MESSAGE("Invalid assertion, a is not smaller than b.") );
 * \endverbatim
 * \tparam cond is a parameter that is used to conditionally exclude the check. Namely, if the assertion level of the assertion to be verified is less or equal to the current level, the check is performed
 * \tparam Exception is the type of exception that the check would throw.
 * \param assertion is the assertion to be verified
 * \param message is a optional message that will be written when the exception is thrown
 */
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


#endif // RUNTIME_ASSERTIONS_H
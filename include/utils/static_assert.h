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



#ifndef HOLOR_STATIC_ASSERT_H
#define HOLOR_STATIC_ASSERT_H

#include <type_traits>

// TODO: add documentation


/** \file static_assert.h
 * \brief static assertions.
 *
 * This header contains utils related to static assertions used within BSTools.
 */
namespace holor{

namespace assert{
    /****************************************************************
                                ALL
    ****************************************************************/
    constexpr bool all(){
        return true;
    }

    template<typename ... Args>
    constexpr bool all(bool b, Args... args){
        return b && all(args...);
    }


    /****************************************************************
                                SOME
    ****************************************************************/
    constexpr bool some(){
        return false;
    }

    template<typename ... Args>
    constexpr bool some(bool b, Args... args){
        return b || some(args...);
    }


    /****************************************************************
                        HAS_FUNCTIONS
    ****************************************************************/
    struct substitution_failure{};

    template<typename T>
    struct substitution_succeeded: std::true_type {};

    template<>
    struct substitution_succeeded<substitution_failure>: std::false_type {};


    // //----------------     IS PRINTABLE   -------------------------
    // template<typename T>
    // struct get_print{
    //     private:
    //         std::ostream& os;

    //         template<typename X>
    //         static auto check(X const& x) -> decltype(operator<<(os, x));

    //         static substitution_failure check(...);
        
    //     public:
    //         using type = decltype(check(std::declval<T>()));
    // };

    // template<typename T>
    // struct is_printable: substitution_succeeded<typename get_print<T>::type>{};

    // template<typename T>
    // constexpr bool is_printable_v(){
    //     return is_printable<T>::value;
    // }

} //namespace assert

} //namespace holor


#endif // HOLOR_STATIC_ASSERT_H
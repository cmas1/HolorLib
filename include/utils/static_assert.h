//  ____   _____   _____   __       __   __   __   ______       _______   _____   _____   __       _____ 
//  |   \ |   __| |  _  \ |  |     |  | |  \ |  | |  ____|     |__   __| |  _  | |  _  | |  |     |   __|
//  ____/  \  \   | |_| | |  |     |  | |       | |  |__          | |    | | | | | | | | |  |      \  \  
//  |   \  _\  \  |  ___/ |  |___  |  | |  |\   | |  |___         | |    | |_| | | |_| | |  |___   _\  \ 
//  ____/ |_____| |__|    |______| |__| |__| \__| |______|        |_|    |_____| |_____| |______| |_____|

// This file is part of BSTools, a lightweight C++ template library that implements B-Splines

// Copyright 2019 Carlo Masone

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.



#ifndef STATIC_ASSERT_H
#define STATIC_ASSERT_H

#include <type_traits>
#include "../polynomials/polynomial.h"
#include "../polynomials/polynomial_sparse.h"

// TODO: add documentation


/** \file static_assert.h
 * \brief static assertions.
 *
 * This header contains utils related to static assertions used within BSTools.
 */
namespace bst{

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
                        IS_POLYNOMIAL
    ****************************************************************/

    template<typename...>
    struct is_polynomial: std::false_type{};

    template<typename T>
    struct is_polynomial<Polynomial<T>>: std::true_type{};

    template<typename T>
    struct is_polynomial<PolynomialSparse<T>>: std::true_type{};

    template<typename T>
    constexpr bool is_polynomial_v(){
        return is_polynomial<T>::value;
    }

    /****************************************************************
                        HAS_FUNCTIONS
    ****************************************************************/
    struct substitution_failure{};

    template<typename T>
    struct substitution_succeeded: std::true_type {};

    template<>
    struct substitution_succeeded<substitution_failure>: std::false_type {};


    //----------------     IS PRINTABLE   -------------------------
    template<typename T>
    struct get_print{
        private:
            std::ostream& os;

            template<typename X>
            static auto check(X const& x) -> decltype(operator<<(os, x));

            static substitution_failure check(...);
        
        public:
            using type = decltype(check(std::declval<T>()));
    };

    template<typename T>
    struct is_printable: substitution_succeeded<typename get_print<T>::type>{};

    template<typename T>
    constexpr bool is_printable_v(){
        return is_printable<T>::value;
    }

} //namespace assert

} //namespace bst


#endif // STATIC_ASSERT_H
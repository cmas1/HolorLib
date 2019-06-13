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



#ifndef DYNAMIC_ASSERT_H
#define DYNAMIC_ASSERT_H

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


namespace bst{

namespace assert{



enum AssertionLevel{
    release = 0,
    internal = 1,
    debug = 2,
};



// DEFINE_EXCEPTION_LEVEL comes from -Doption
constexpr int current_level = DEFINE_ASSERT_LEVEL;
constexpr int default_level = 2;


constexpr bool assertion_level(int n){
    return (n <= current_level);
}


template<bool cond = assertion_level(default_level), typename Exception = bst::exception::BstInvalidArgument>
void dynamic_assert(bool assertion, const std::string& message = "Dynamic assertion failed."){
    if (assertion){
        return;
    } else {
        throw Exception(message);
    }
}


// If condition is false (the assertion level is higher than the current level chosen) do nothing
template<>
void dynamic_assert<false, bst::exception::BstRuntimeError>(bool, const std::string&){
}


// If condition is false (the assertion level is higher than the current level chosen) do nothing
template<>
void dynamic_assert<false, bst::exception::BstInvalidArgument>(bool, const std::string&){
}


} //namespace bst

} //namespace assert


#endif // DYNAMIC_ASSERT_H
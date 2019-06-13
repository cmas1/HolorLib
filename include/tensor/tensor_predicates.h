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



#ifndef TENSOR_PREDICATES_H
#define TENSOR_PREDICATES_H


#include "../utils/static_assert.h"
#include "tensor_slice.h"


using namespace bst::assert;

/** \file tensor_initializer.h
 * \brief predicates used the implementation of tensors, in static assertions, compile time conditions and runtime assertions
 */

namespace bst{
namespace tensor_impl{

    /*
    * function used to verify that the subscripts used to access the elements of a tensor are actually <tt>size_t</tt>
    * return \p true if all the arguments can be converted to <tt>size_t</tt>
    */
    template<typename... Args>
    constexpr bool requesting_element(){
        return assert::all(std::is_convertible<Args, size_t>()...);
    }


    /*
    * function used to verify that some of the subscripts used to access the elements of a tensor are slices
    * return \p true if some of the arguments are slices
    */
    template<typename... Args>
    constexpr bool requesting_slice(){
        return assert::all((std::is_convertible<Args, size_t>() || std::is_same<Args, slice_request>() || std::is_convertible<Args, slice_request>())...) && assert::some(std::is_same<Args, slice_request>()...);
    }


    /*
    * function used to verify that the subscripts used to access the elements of a tensor are actually <tt>size_t</tt>
    * return \p true if all the arguments can be converted to <tt>size_t</tt>
    */
    template<size_t N, typename... Dims>
    bool check_bounds(const TensorSlice<N>& slice, Dims... dims){
        size_t indexes[N]{size_t(dims)...};
        return equal(indexes, indexes+N, slice.lengths_.begin(), std::less<size_t>{});
    }



} //namespace tensor_impl
} //namespace bst

#endif // TENSOR_PREDICATES_H
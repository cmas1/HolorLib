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



#ifndef HOLOR_PREDICATES_H
#define HOLOR_PREDICATES_H


#include <type_traits>


/** \file tensor_initializer.h
 * \brief predicates used the implementation of tensors, in static assertions, compile time conditions and runtime assertions
 */

namespace holor{
namespace impl{

    /*
    * function used to verify that the subscripts used to access the elements of a tensor are actually <tt>size_t</tt>
    * return \p true if all the arguments can be converted to <tt>size_t</tt>
    */
    template<typename... Args>
    constexpr bool requesting_element(){
        return assert::all(std::is_convertible<Args, size_t>()...);
    }




    /*
    * function used to verify that the subscripts used to access the elements of a tensor are actually <tt>size_t</tt>
    * return \p true if all the arguments can be converted to <tt>size_t</tt>
    */
    // template<size_t N, typename... Dims>
    // bool check_bounds(const TensorSlice<N>& slice, Dims... dims){
    //     size_t indexes[N]{size_t(dims)...};
    //     return equal(indexes, indexes+N, slice.lengths_.begin(), std::less<size_t>{});
    // }

    
    //----------------     IS SEQUENCE CONTAINER   -------------------------

    // template<typename T, typename _ = void>
    // struct is_seq_container : std::false_type {};

    // template<typename T>
    // struct is_seq_container<
    //     T,
    //     std::void_t<
    //             typename T::value_type,
    //             typename T::size_type,
    //             typename T::iterator,
    //             typename T::const_iterator,
    //             decltype(std::declval<T>().size()),
    //             decltype(std::declval<T>().begin()),
    //             decltype(std::declval<T>().end()),
    //             decltype(std::declval<T>().cbegin()),
    //             decltype(std::declval<T>().cend())>
    // > : public std::true_type {};


    // template<typename T>
    // constexpr bool is_seq_container_v(){
    //     return is_seq_container<std::remove_reference_t<T>>::value;
    // }


} //namespace tensor_impl
} //namespace bst

#endif // HOLOR_PREDICATES_H
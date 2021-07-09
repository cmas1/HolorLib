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



#ifndef PREDICATES_H
#define PREDICATES_H


#include <type_traits>


/** \file tensor_initializer.h
 * \brief predicates used the implementation of tensors, in static assertions, compile time conditions and runtime assertions
 */

namespace holor{
namespace impl{


    /*!
    * \brief predicate to select among two different types based on a boolean flag.
    */
    template <bool flag, class IsTrue, class IsFalse>
    struct choose;

    template <class IsTrue, class IsFalse>
    struct choose<true, IsTrue, IsFalse> {
        using type =  IsTrue;
    };

    template <class IsTrue, class IsFalse>
    struct choose<false, IsTrue, IsFalse> {
        using type = IsFalse;
    };



    /*!
     * \brief resizable concept
     */
    template<class T>
    concept Resizable = requires(T container)
    {
        container.resize(std::size_t{0});
    };


    /*!
     * \brief concept for a container
     */
    template<class T>
    concept Container = requires(T container)
    {
        container.begin();
        container.end();
        // container.cbegin();
        // container.cend();
        container.size();
    };


    /*!
     * \brief concept for a fixed length (at compile-time) container
     */
    template<class T>
    concept FixedArray = Container<T> && !Resizable<T>;

    /*!
     * \brief concept for a variable length (not fixed at compile-time) container
     */
    template<class T>
    concept VariableArray = Container<T> && Resizable<T>;
    

} //namespace impl
} //namespace holor

#endif // PREDICATES_H
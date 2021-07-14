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
#include <iostream>

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


    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        Type Selection
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
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



    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        Container Concepts
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*!
     * \brief Type that as a resize function
     */
    template<class T>
    concept Resizable = requires(T container)
    {
        container.resize(std::size_t{0});
    };

    /*!
     * \brief Type that has size, begin and end functions
     */
    template<class T>
    concept Iterable = requires(T container)
    {
        container.begin();
        container.end();
        container.size();
    };

    /*!
     * \brief Type that has size, begin and end functions
     */
    template<class T, class U>
    concept Convertible = std::convertible_to<typename T::value_type,U>;

    /*!
     * \brief Container that has size fixed to N
     */
    template<class T, size_t N>
    concept Sized = (std::tuple_size<T>::value==N);    

    /*!
     * \brief concept for a resizeable container, with elements convertible to U and iterators
     */
    template<class T, class U>
    concept  ResizeableTypedContainer = Iterable<T> && Resizable<T> && Convertible<T,U>;

    /*!
     * \brief concept for a fixed legnth container, with N elements convertible to U and iterators
     */
    template<class T, class U, size_t N>
    concept SizedTypedContainer = Iterable<T> && !Resizable<T> && Convertible<T,U> && Sized<T,N>;



    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            Printable
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*!
     * \brief concept for a fixed legnth container, with N elements convertible to U and iterators
     */
    template<class Type>
    concept Printable = requires (Type object, std::ostream& os){
        (os<<object);
    };


} //namespace assert

} //namespace holor


#endif // HOLOR_STATIC_ASSERT_H
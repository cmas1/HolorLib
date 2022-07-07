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



#ifndef HOLOR_STATIC_ASSERTIONS_H
#define HOLOR_STATIC_ASSERTIONS_H

#include <type_traits>
#include <iostream>


/** \file static_assertions.h
 * \brief This header contains utils related to compile-time assertions
 */
namespace holor{
namespace assert{


    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        Predicate ALL
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*!
     * \brief predicate that returns true if all the arguments are true
     */
    constexpr bool all(){
        return true;
    }

    template<typename ... Args>
    constexpr bool all(bool b, Args... args){
        return b && all(args...);
    }


    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        Predicate SOME
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*!
     * \brief predicate that returns true at least one of the arguments is true
     */
    constexpr bool some(){
        return false;
    }

    template<typename ... Args>
    constexpr bool some(bool b, Args... args){
        return b || some(args...);
    }


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
     * \brief concept used to represent a container that has size, begin and end functions, and a value type
     */
    template<class T>
    concept IterableContainer = requires(T container)
    {
        container.begin();
        container.end();
        container.cbegin();
        container.cend();
        container.size();
        typename std::remove_reference_t<T>::value_type;
    };


    /*!
     * \brief concept used to represent a container that has a resize function
     */
    template<class T>
    concept Resizable = requires(T container)
    {
        container.resize(std::size_t{0});
    };


    /*!
     * \brief concept used to represent a container that has size fixed to N
     */
    template<class T, size_t N>
    concept Sized = (std::tuple_size<T>::value==N); 


    /*!
     * \brief concept used to represent containers whose elements are convertible to a specified type
     */
    template<class T, class U>
    concept ConvertibleElements = std::convertible_to<typename std::remove_reference_t<T>::value_type,U>;

    
    /*!
     * \brief concept representing a container whose elements are of a specific type
     */
    template<class T, class U>
    concept TypedContainer = IterableContainer<T> && ConvertibleElements<T,U>;

    
    /*!
     * \brief concept representing a resizeable container with iterators
     */
    template<class T>
    concept  ResizeableContainer = IterableContainer<T> && Resizable<T>;
       
    /*!
     * \brief concept representing a resizeable container, with elements convertible to U and iterators
     */
    template<class T, class U>
    concept  ResizeableTypedContainer = IterableContainer<T> && Resizable<T> && ConvertibleElements<T,U>;

    /*!
     * \brief concept representing a fixed length container, with N elements and iterators
     */
    template<class T, size_t N>
    concept SizedContainer = IterableContainer<T> && !Resizable<T> && Sized<T,N>;

    /*!
     * \brief concept representing a fixed length container, with N elements convertible to U and iterators
     */
    template<class T, class U, size_t N>
    concept SizedTypedContainer = IterableContainer<T> && !Resizable<T> && ConvertibleElements<T,U> && Sized<T,N>;

    /*!
     * \brief concept representing an iterale container that is either resizeable or it has a fixed length
     */
    template<class T, size_t N>
    concept RSContainer = IterableContainer<T> && (Resizable<T> || Sized<T,N>);

    /*!
     * \brief concept for a typed container that can be resizeable or sized, with N elements and iterators
     */
    template<class T, class U, size_t N>
    concept RSTypedContainer = IterableContainer<T> && ConvertibleElements<T,U> && (Resizable<T> || (!Resizable<T> && Sized<T,N>) );

    

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            Concept Printable
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*!
     * \brief concept for type that supports the operator << for printing on an outputstream
     */
    template<class Type>
    concept Printable = requires (Type object, std::ostream& os){
        (os<<object);
    };


    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        Unary and Binary Functions
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    template <typename return_type, typename arg1_type, typename Func> 
    concept Unaryfunction = requires (Func func, arg1_type arg1){
        { std::invoke(std::forward<Func>(func), arg1) } -> std::same_as<return_type>;
    };

    template <typename return_type, typename arg1_type, typename arg2_type, typename Func> 
    concept Binaryfunction = requires (Func func, arg1_type arg1, arg2_type arg2){
        { std::invoke(std::forward<Func>(func), arg1, arg2) } -> std::same_as<return_type>;
    };


} //namespace assert

} //namespace holor


#endif // HOLOR_STATIC_ASSERTIONS_H
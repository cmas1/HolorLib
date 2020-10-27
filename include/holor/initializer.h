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



#ifndef HOLOR_INITIALIZER_H
#define HOLOR_INITIALIZER_H

#include <cstddef>
#include <initializer_list>
#include <array>
#include <type_traits>

#include <iostream>



/** \file initializer.h
 * \brief Structs and utilities for initializng a Holor container from nested <tt>std::initializer_list</tt>s.
 *
 * This header contains the definitions of the nested <tt>std::initializer_list</tt>s used to initialize a Holor container,
 * as well as several helper functions that are used during such an initialization.
 */

namespace holor{

    namespace impl{

        /// HolorInit struct
        /*!
         *  \brief Helper structure used in the creation of a Holor container from nested initializer_list.
         */
        template<typename T, std::size_t N>
        struct HolorInit{
            using type = std::initializer_list<typename HolorInit<T, N-1>::type>;
        };


        /*!
         *  \brief Specialization of HolorInit when `N=1`, i.e. the most deeply nested list is reached
         */
        template<typename T>
        struct HolorInit<T,1>{
            using type = std::initializer_list<T>;
        };


        /*!
         *  \brief Specialization of HolorInit when `N=0`. This is an error, so the spcialization is not defined
         */
        template<typename T>
        struct HolorInit<T,0>;

        
        /*
         * \brief Helper function that calculates the lengths of a Holor container from nested initializer lists.
         * 
         * \param list list of elements for the initialization
         * 
         * \return a std::array of size `N` containing the lengths of the tensor
         */
        template<std::size_t N, typename List>
        std::array<std::size_t, N> derive_lengths(const List& list);


        /*
         * \brief Helper function used in the computation of the lengths of a Holor container created from nested initializer lists.
         * 
         * \tparam N number of dimensions in the nested list
         * \tparam Iter Iterator type 
         * \tparam List type
         * 
         * \param first iterator at the beginning of the list
         * \param list list of elements for the initialization
         */
        template<std::size_t N, typename Iter, typename List>
        void add_lengths(Iter& first, const List& list);

        
        /*
         * \brief Helper function that checks if all the rows have the same number of elements
         * 
         * \param list list of elements for the initialization 
         * 
         * \return true if all the rows have the same number of elements, false otherwise
         */
        template<std::size_t N, typename List>
        bool check_dimensions_consistency(const List& list);


        /*
         * \brief Helper function that takes the elemens of a nested initializer list and inserts them in the Holor container container as a \p std::vector
         * 
         * \param list list of elements for the initialization 
         * \param vec where the elements are inserted into
         */
        template<typename T, typename Vec>
        void insert_flat(std::initializer_list<T> list, Vec& vec);

        
        /*
         * \brief Helper function that goes through the levels of a nested initializer, recursively, in order to access its elements 
         * 
         * \param first pointer to the first \p std::initializer_list of the nested tree
         * \param last pointer to the last \p std::initializer_list of the nested tree
         * \param vec where the elements are inserted into
         */
        template<typename T, typename Vec>
        void add_list(const std::initializer_list<T>* first, const std::initializer_list<T>* last, Vec& vec);


        /*
         * \brief Helper function that inserts the elements of a <tt>std::initializer_list<\tt> into a vector to store them into a Holor container
         * 
         * \param first pointer to the first element of the list
         * \param last pointer to the last element of the list
         * \param vec where the elements are inserted into
         */
        template<typename T, typename Vec>
        void add_list( const T* first, const T*last, Vec& vec);


    } //namespace impl


    template<typename T, std::size_t N>
    using nested_list = typename impl::HolorInit<T,N>::type;


    /************************************************************
                             DEFINITIONS
    *************************************************************/


    template<std::size_t N, typename List>
    std::array<std::size_t, N> impl::derive_lengths(const List& list){
        std::array<std::size_t, N> lengths;
        auto f = lengths.begin();
        add_lengths<N>(f, list);
        return lengths;
    }


    template<size_t N, typename Iter, typename List>
    void impl::add_lengths(Iter& first, const List& list){
        if constexpr(N>1){
            // TODO: dynamic assert
            // assert::dynamic_assert<assertion_level(AssertionLevel::internal), exception::BstRuntimeError>(check_dimensions_consistency<N>(list), EXCEPTION_MESSAGE("Rows don't have the same number of elements!"));
            *first++ = list.size(); //store this size
            add_lengths<N-1>(first, *list.begin());
        }else{
            *first++ = list.size(); //store this size
        }
    }

    

    // template<std::size_t N, typename List>
    // bool impl::check_dimensions_consistency(const List& list){
    //     auto i = list.begin();
    //     for(auto j = i+1; j!=list.end(); ++j){
    //         if(derive_lengths<N-1>(*i) != derive_lengths<N-1>(*j)){
    //             return false;
    //         }
    //     }
    //     return true;
    // }



    template<typename T, typename Vec>
    void impl::insert_flat(std::initializer_list<T> list, Vec& vec){
        add_list(list.begin(), list.end(), vec);
    }


    //this is for when the insert flat is called on not the innermost list
    template<typename T, typename Vec>
    void impl::add_list(const std::initializer_list<T>* first, const std::initializer_list<T>* last, Vec& vec){
        //TODO: check usage of ranges
        for(; first != last; ++first){
            add_list(first->begin(), first->end(), vec);
        }
    }


    // This is when the insert flat is invoked on the innermost list
    template<typename T, typename Vec>
    void impl::add_list( const T* first, const T*last, Vec& vec){
        //TODO: check usage of ranges
        vec.insert(vec.end(), first, last);
    }


} //namespace bst

#endif // HOLOR_INITIALIZER_H
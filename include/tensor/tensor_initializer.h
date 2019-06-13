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



#ifndef TENSOR_INITIALIZER_H
#define TENSOR_INITIALIZER_H

#include <cstddef>
#include <initializer_list>
#include <array>
#include <type_traits>

#include "../utils/dynamic_assert.h"


using namespace bst::assert;

/** \file tensor_initializer.h
 * \brief Structs and utilities for initializng a Tensor from nested <tt>std::initializer_list</tt>s.
 *
 * This header contains the definitions of the nested <tt>std::initializer_list</tt>s used to initialize a Tensor,
 * as well as several helper functions that are used during such an initialization.
 */

namespace bst{

    namespace tensor_impl{

        /// TensorInit struct
        /*!
        *  \brief Helper structure used in the creation of a Tensor from a tree of <tt>std::initializer_list</tt>.
        */
        template<typename T, std::size_t N>
        struct TensorInit{
            using type = std::initializer_list<typename TensorInit<T, N-1>::type>;
        };


        /*!
        *  \brief Specialization of TensorInit when <tt>N=1</tt>, i.e. the most deeply nested list is reached
        */
        template<typename T>
        struct TensorInit<T,1>{
            using type = std::initializer_list<T>;
        };


        /*!
         *  \brief Specialization of TensorInit when <tt>N=0</tt>. This is an error, so the spcialization is not defined
         */
        template<typename T>
        struct TensorInit<T,0>;

        
        /*
         * \brief Helper function that calculates the lengths of a tensor from a tree of <tt>std::initializer_list</tt>.
         * 
         * \param list list of elements for the initialization
         * 
         * \return a std::array of size \p N containing the lengths of the tensor
         */
        template<std::size_t N, typename List>
        std::array<std::size_t, N> derive_lengths(const List& list);


        /*
         * \brief Helper function used in the computation of the lengths of a Tensor created from a tree of <tt>std::initializer_list</tt>.
         * 
         * This function is used when the <tt>N>1</tt>, i.e. when we haven't reached the deepest level of the tree of <tt>std::initializer_list</tt>.
         * 
         * \param first iterator at the beginning of the list
         * \param list list of elements for the initialization
         */
        template<std::size_t N, typename I, typename List>
        std::enable_if_t<(N>1), void> add_lengths(I& first, const List& list);


        /*
         * \brief Helper function used in the computation of the lengths of a Tensor created from a tree of <tt>std::initializer_list</tt>.
         * 
         * This function is used when the <tt>N==1</tt>, i.e. when we have reached the deepest level of the tree of <tt>std::initializer_list</tt>.
         * 
         * \param first iterator at the beginning of the list
         * \param list list of elements for the initialization
         */
        template<std::size_t N, typename I, typename List>
        std::enable_if_t<(N==1), void> add_lengths(I& first, const List& list);

        
        /*
         * \brief Helper function that checks if all the rows have the same number of elements
         * 
         * \param list list of elements for the initialization 
         * 
         * \return true if all the rows have the same number of elements, false otherwise
         */
        template<std::size_t N, typename List>
        bool check_non_jagged(const List& list);


        /*
         * \brief Helper function that takes the elemens of a nested initializer list and inserts them in the Tensor container as a \p std::vector
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
         * \brief Helper function that inserts the elements of a <tt>std::initializer_list<\tt> into a vector to store them into a Tensor
         * 
         * \param first pointer to the first element of the list
         * \param last pointer to the last element of the list
         * \param vec where the elements are inserted into
         */
        template<typename T, typename Vec>
        void add_list( const T* first, const T*last, Vec& vec);


    } //namespace tensor_impl


    template<typename T, std::size_t N>
    using initializer_tensor = typename tensor_impl::TensorInit<T,N>::type;


    /************************************************************
                             DEFINITIONS
    *************************************************************/


    template<std::size_t N, typename List>
    std::array<std::size_t, N> tensor_impl::derive_lengths(const List& list){
        std::array<std::size_t, N> lengths;
        auto f = lengths.begin();
        add_lengths<N>(f, list);
        return lengths;
    }



    template<std::size_t N, typename I, typename List>
    std::enable_if_t<(N>1), void> tensor_impl::add_lengths(I& first, const List& list){
        assert::dynamic_assert<assertion_level(AssertionLevel::internal), exception::BstRuntimeError>(check_non_jagged<N>(list), EXCEPTION_MESSAGE("Rows don't have the same number of elements!"));
        *first++ = list.size(); //store this size
        add_lengths<N-1>(first, *list.begin());
    }



    template<std::size_t N, typename I, typename List>
    std::enable_if_t<(N==1), void> tensor_impl::add_lengths(I& first, const List& list){
        *first++ = list.size(); //store this size
    }

    

    template<std::size_t N, typename List>
    bool tensor_impl::check_non_jagged(const List& list){
        auto i = list.begin();
        for(auto j = i+1; j!=list.end(); ++j){
            if(derive_lengths<N-1>(*i) != derive_lengths<N-1>(*j)){
                return false;
            }
        }
        return true;
    }



    template<typename T, typename Vec>
    void tensor_impl::insert_flat(std::initializer_list<T> list, Vec& vec){
        add_list(list.begin(), list.end(), vec);
    }



    template<typename T, typename Vec>
    void tensor_impl::add_list(const std::initializer_list<T>* first, const std::initializer_list<T>* last, Vec& vec){
        for(; first != last; ++first){
            add_list(first->begin(), first->end(), vec);
        }
    }



    template<typename T, typename Vec>
    void tensor_impl::add_list( const T* first, const T*last, Vec& vec){
        vec.insert(vec.end(), first, last);
    }


} //namespace bst

#endif // TENSOR_INITIALIZER_H
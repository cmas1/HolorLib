// This file is part of Holor, a C++ template library for multi-dimensional containers

// Copyright 2020-2021 Carlo Masone

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or suholorantial portions of the Software.

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


/*! \file initializer.h
 * \brief Structs and utilities for initializng a Holor container from nested `std::initializer_list`s.
 *
 * This header contains the definitions of the nested `std::initializer_list`s used to initialize a Holor container,
 * as well as several helper functions that are used during such an initialization.
 */

namespace holor{

    namespace impl{

        /*=======================================================================
                                    HolorInit Struct
        =======================================================================*/
        /*!
         *  \brief Helper structure used in the creation of a Holor container from nested initializer_list.
         *  \tparam T is the type of the elements in the nested lists
         *  \tparam N is the number of nested levels
         */
        template<typename T, std::size_t N>
        struct HolorInit{
            using type = std::initializer_list<typename HolorInit<T, N-1>::type>;
        };

        /*!
         *  \brief Specialization of HolorInit when `N=1`, i.e., the deepest list is reached
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



        /*=======================================================================
                                Functions: Declarations
        =======================================================================*/        
        /*!
         * \brief Function that calculates the lengths of a Holor container from nested initializer lists.
         * \param list list of elements for the initialization
         * \return a std::array of size `N` containing the lengths of the tensor
         */
        template<std::size_t N, typename List>
        std::array<std::size_t, N> derive_lengths(const List& list);

        /*!
         * \brief Helper function used in `derive_lengths`.
         */
        template<std::size_t N, typename Iter, typename List>
        void add_lengths(Iter& first, const List& list);

        

        /*!
         * \brief Function that checks if all the levels in the nested lists have consisten dimensions
         * \param list list of elements for the initialization 
         * \return true if all the rows have the same number of elements, false otherwise
         */
        template<std::size_t N, typename List>
        bool is_consistent(const List& list);

        /*!
         * \brief Helper function used in `is_consistent`.
         */
        template<std::size_t N, typename List>
        bool check_dimension_consistency(const List& list);



        /*!
         * \brief Function that takes the elements of a nested initializer list and inserts them in the Holor container container as a `std::vector`
         * \param list list of elements for the initialization 
         * \param vec where the elements are inserted into
         */
        template<typename T, typename Vec>
        void insert_flat(std::initializer_list<T> list, Vec& vec);
        
        /*!
         * \brief Helper function  used in `insert_flat`
         */
        template<typename T, typename Vec>
        void add_list(const std::initializer_list<T>* first, const std::initializer_list<T>* last, Vec& vec);

        template<typename T, typename Vec>
        void add_list( const T* first, const T*last, Vec& vec);

    } //namespace impl


    /*=======================================================================
                        Functions: Definitions
    =======================================================================*/
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
            assert::dynamic_assert(is_consistent<N>(list), EXCEPTION_MESSAGE("The nested list does has inconsistent dimensions."));
            *first++ = list.size(); //store this size
            add_lengths<N-1>(first, *list.begin());
        }else{ // innermost list
            *first++ = list.size(); 
        }
    }

    
    template<std::size_t N, typename List>
    bool impl::is_consistent(const List& list){
        return check_dimension_consistency<N>(list);
    }


    template<std::size_t N, typename List>
    bool impl::check_dimension_consistency(const List& list){
        // verify that all lists at the curent level have the same size. If yes, proceed to the next level, otherwise return false (check failed)
        if constexpr (N>1){
            auto dim = list.begin()->size();
            for(auto iter_current = list.begin()+1; iter_current!=list.end(); ++iter_current){
                if(iter_current->size() != dim){
                    return false;
                }
            }
            return check_dimension_consistency<N-1>(*list.begin());
        }else { 
            return true; //all checks have succeded. return true.
        }
    }



    template<typename T, typename Vec>
    void impl::insert_flat(std::initializer_list<T> list, Vec& vec){
        add_list(list.begin(), list.end(), vec);
    }


    //this is for when the insert flat is not called on the innermost list
    template<typename T, typename Vec>
    void impl::add_list(const std::initializer_list<T>* first, const std::initializer_list<T>* last, Vec& vec){
        for(; first != last; ++first){
            add_list(first->begin(), first->end(), vec);
        }
    }

    // This is when the insert flat is invoked on the innermost list
    template<typename T, typename Vec>
    void impl::add_list( const T* first, const T*last, Vec& vec){
        vec.insert(vec.end(), first, last);
    }



    /*=======================================================================
                                Alias
    =======================================================================*/
    /*!
     * \brief alias to refer to the type of nested initializer lists
     */
    template<typename T, std::size_t N>
    using nested_list = typename impl::HolorInit<T,N>::type;



} //namespace holor

#endif // HOLOR_INITIALIZER_H
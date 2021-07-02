// This file is part of HolorRef, a C++ template library for multi-dimensional containers

// Copyright 2020 Carlo Masone

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



#ifndef HOLOR_REF_ITERATOR_H
#define HOLOR_REF_ITERATOR_H

#include <cstddef>
#include <iterator>

#include "layout.h"

#include <iostream>

namespace holor{

namespace impl{


/*================================================================================================
                                    HolorRef Iterator
================================================================================================*/
/*!
 * \brief class that implements a random-acces iterator for the Holor_Ref view container.
 * For a brief description of the properties of random-access iterators refer to https://www.cplusplus.com/reference/iterator/RandomAccessIterator/
 * or to https://en.cppreference.com/w/cpp/iterator/random_access_iterator
 */
template<typename T, size_t N>
class HRef_iterator {
    public:
        // using iterator_concept = std::random_access_iterator; //NOTE: this does not compile...
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        constructors/destructors/assignments
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        template<typename Container> //TODO: this should be a concept of a Holor_Ref. Or else, the whole iterator class may be moved inside of Holor_Ref
        explicit HRef_iterator(Container& container){
            start_ptr_ = container.dataptr();
            iter_ptr_ = container.dataptr();
            layout_ptr_ = &(container.layout());
            coordinates_.fill(0);
        }


        // Function used for geeting the end iterator.
        template<typename Container> //TODO: this should be a concept of a Holor_Ref. Or else, the whole iterator class may be moved inside of Holor_Ref
        explicit HRef_iterator(Container& container, bool is_end){
            start_ptr_ = container.dataptr();
            layout_ptr_ = &(container.layout());
            for (auto cnt = 0; cnt < (N-1) ; cnt++){
                coordinates_[cnt] = layout_ptr_->length(cnt) -1;
            }
            coordinates_[N-1] = layout_ptr_->length(N-1); //coordinates to one past the last element of the container
            iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
        }

        // default constructible
        HRef_iterator() = default;

        //copy constructible
        HRef_iterator(const HRef_iterator& a) = default;

        //copy-assignable
        HRef_iterator& operator=(const HRef_iterator& a) = default;

        //destructile
        ~HRef_iterator() = default;

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        reference/dereference operators
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief dereference operator as an rvalue or lvalue (if in a dereferenceable state)
         */
        reference operator*() const {
            return *iter_ptr_;
        } 

        /*!
         * \brief dereference operator as an rvalue (if in a dereferenceable state)
         */
        pointer operator->() const {
            return &(*iter_ptr_);
        }

        /*!
         * \brief offset dereference operator
         */
        reference operator[](difference_type n) const {
            return *((this + n)->iter_ptr_);
        } 

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        increment operators 
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief prefix ++
         */
        HRef_iterator& operator++(){
            step_forward<N>();
            iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
            return *this;
        }

        /*!
         * \brief postfix ++
         */
        HRef_iterator operator++(int){
            HRef_iterator retval = *this;
            ++(*this);
            return retval;
        }

        /*!
         * \brief prefix --
         */
        HRef_iterator& operator--(){
            step_back<N>();
            iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
            return *this;
        }

        /*!
         * \brief postfix --
         */
        HRef_iterator operator--(int){
            HRef_iterator retval = *this;
            --(*this);
            return retval;
        }

        /*!
         * \brief advances the iterator by n positions
         */
        HRef_iterator& operator+=(difference_type n){
            for (auto cnt = 0; cnt < n; cnt++){
                step_forward<N>();
            }
            iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
            return *this;
        }
        
        /*!
         * \brief decreases the iterator by n positions
         */
        HRef_iterator& operator-=(difference_type n){
            for (auto cnt = 0; cnt < n; cnt++){
                step_back<N>();
            }
            iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
            return *this;
        }

        /*!
         * \brief given the iterator a, it implements the operation a + n
         */
        HRef_iterator operator+(difference_type n) const{
            HRef_iterator retval = *this;
            retval += n;
            return retval;
        }

        /*!
         * \brief given the iterator a, it implements the operation a - n
         */
        HRef_iterator operator-(difference_type n) const{
            HRef_iterator retval = *this;
            retval -= n;
            return retval;
        }


        /*!
         * \brief implements the operation n + a
         */
        friend HRef_iterator operator+(difference_type n, const HRef_iterator& a) {
            HRef_iterator retval = a;
            retval += n;
            return retval;
        }


        /*!
         * \brief given two iterators a and b, it implements the difference a - b
         */
        friend difference_type operator-(const HRef_iterator& a, const HRef_iterator&b) {
            difference_type result = 0;
            for (auto cnt = 0; cnt<N; cnt++){
                result += a.coordinates_[cnt]*a.layout_ptr_->strides()[cnt] - b.coordinates_[cnt]*b.layout_ptr_->strides()[cnt];
            }
            return result;
        }



        /*~~~~~~~~~~~~~~~~~~~~~~~~
        equality operators
        ~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief equality operations to compare two iterators. For example, needed to test iter == end()
         */
        bool operator==(const HRef_iterator& rhs) const{
            return iter_ptr_ == rhs.iter_ptr_;
        }  

        /*!
         * \brief equality operations to compare two iterators. For example, needed to test iter != end()
         */
        bool operator!=(const HRef_iterator& rhs) const{
            return iter_ptr_ != rhs.iter_ptr_;
        }

        /*!
         * \brief defualt three-way comparison operator
         */
        friend auto operator<=>(HRef_iterator, HRef_iterator) = default;


        //TODO: we should consider implementing sentinel operations to support c++20 ranges functions (https://en.cppreference.com/w/cpp/ranges). First, I should study better ranges... !See https://www.foonathan.net/2020/03/iterator-sentinel/ for more info on how to implement these sentinels
        // 
        // //sentinel operators
        // 
        // bool operator==(HRef_iterator, Href_sentinel);
        // bool operator!=(HRef_iterator, Href_sentinel);
        // friend bool operator==(Href_sentinel, HRef_iterator);
        // friend bool operator!=(Href_sentinel, HRef_iterator);
        // friend difference_type operator-(Href_sentinel, HRef_iterator); // Not required, but useful

    private:
        pointer start_ptr_; //<-! \brief pointer to initial memory location addressed by the Holor_Ref that the iterator refers to. This is needed because the elements are not stored contiguously in memory
        
        pointer iter_ptr_; //<-! \brief pointer to current memory location addressed by the iterator
        
        const Layout<N>* layout_ptr_; //<-! \brief pointer to the layout of the Holor_Ref container that the iterator refers to. This is needed because the elements are not stored contiguously in memory
        
        std::array<difference_type, N> coordinates_; //<-! \brief coordinates of the current iterator from the starting pointer. This is needed because the elements are not stored contiguously in memory

        /*!
         * \brief helper function to implement the ++ operator
         */
        template<size_t Coord>
        void step_forward(){
            if ( coordinates_[Coord] < (layout_ptr_->length(Coord) -1) ){
                coordinates_[Coord] += 1;
            } else{
                coordinates_[Coord] = 0;
                step_forward<Coord-1>();
            }
        }

        /*!
         * \brief helper function to implement the ++ operator
         */
        template<>
        void step_forward<1>(){
            if ( coordinates_[0] < (layout_ptr_->length(0) -1) ){
                coordinates_[0] += 1;
            } else{ //end of the container
                for (auto cnt = 0; cnt < (N-1) ; cnt++){
                    coordinates_[cnt] = layout_ptr_->length(cnt) -1;
                }
                coordinates_[N-1] = layout_ptr_->length(N-1); //coordinates to one past the last element of the container
            }
        }

        /*!
         * \brief helper function to implement the -- operator
         */
        template<size_t Coord>
        void step_back(){
            if ( coordinates_[Coord] > 0 ){
                coordinates_[Coord] -= 1;
            } else{
                coordinates_[Coord] = layout_ptr_->length(Coord) -1;
                step_back<Coord-1>();
            }
        }

        /*!
         * \brief helper function to implement the -- operator
         */
        template<>
        void step_back<1>(){
            if ( coordinates_[0] > 0 ){
                coordinates_[0] -= 1;
            } else{ //beginning of the container
                for (auto& coord : coordinates_){
                    coord = 0;
                }
            }
        }
};



//TODO: just a reminder, but the implementation of the iterator and const_iterator should be done without code duplication
// template<typename T, size_t N>
// class HolorRef_const_iterator {
// public:
//     typedef typename A::difference_type difference_type;
//     typedef typename A::value_type value_type;
//     typedef typename const A::reference reference;
//     typedef typename const A::pointer pointer;
//     typedef std::random_access_iterator_tag iterator_category; //or another tag

//     const_iterator ();
//     const_iterator (const const_iterator&);
//     const_iterator (const iterator&);
//     ~const_iterator();

//     const_iterator& operator=(const const_iterator&);
//     bool operator==(const const_iterator&) const;
//     bool operator!=(const const_iterator&) const;
//     bool operator<(const const_iterator&) const; //optional
//     bool operator>(const const_iterator&) const; //optional
//     bool operator<=(const const_iterator&) const; //optional
//     bool operator>=(const const_iterator&) const; //optional

//     const_iterator& operator++();
//     const_iterator operator++(int); //optional
//     const_iterator& operator--(); //optional
//     const_iterator operator--(int); //optional
//     const_iterator& operator+=(size_type); //optional
//     const_iterator operator+(size_type) const; //optional
//     friend const_iterator operator+(size_type, const const_iterator&); //optional
//     const_iterator& operator-=(size_type); //optional            
//     const_iterator operator-(size_type) const; //optional
//     difference_type operator-(const_iterator) const; //optional

//     reference operator*() const;
//     pointer operator->() const;
//     reference operator[](size_type) const; //optional
// };




} //namespace impl

} //namespace holor

#endif // HOLOR_REF_ITERATOR_H
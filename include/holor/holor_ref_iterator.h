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



namespace holor{

/*================================================================================================
                                    HolorRef Iterator
================================================================================================*/
/*!
 * \brief class that implements an iterator for the Holor_Ref view container.
 */
template<typename T, size_t N>
class HRef_iterator {
    public:
        using iterator_concept = std::random_access_iterator;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        constructors/destructors/assignments
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/ //TODO: after implementing the increment/decrement operations, to see what is needed beside the pointer
        explicit HRef_iterator(pointer ptr, layout lt) : iter_ptr(ptr), iter_layout(lt) {}; 
        HRef_iterator();
        HRef_iterator(const HRef_iterator& a);
        HRef_iterator& operator=(const HRef_iterator& a);
        ~HRef_iterator();

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        reference/dereference operators
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/ //TODO
        reference operator*() const {
            return *iter_ptr;
        } 

        pointer operator->() const {
            return &(*iter_ptr);
        }

        reference operator[](difference_type n) const{
            return *(iter_ptr + n); //NOTE: the increment must be done according to the layout
        } 

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        increment operators 
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/ //WIP: start from this
        /*!
         * \brief prefix ++
         */
        HRef_iterator& HRef_iterator::operator++(){
            step_forward<N>();
            iter_ptr_ = start_ptr_ + layout_(coordinates_);
            return *this;
        }

        /*!
         * \brief postfix ++
         */
        HRef_iterator HRef_iterator::operator++(int){
            HRef_iterator retval = *this;
            ++(*this);
            return retval;
        }

        /*!
         * \brief prefix --
         */
        HRef_iterator& HRef_iterator::operator--(){
            step_back<N>();
            iter_ptr_ = start_ptr_ + layout_(coordinates_);
            return *this;
        }

        /*!
         * \brief postfix --
         */
        HRef_iterator HRef_iterator::operator--(int){
            HRef_iterator retval = *this;
            --(*this);
            return retval;
        }

        /*!
         * \brief advances the iterator by n positions
         */
        HRef_iterator& HRef_iterator::operator+=(difference_type n){
            for (auto cnt = 0; cnt < n; cnt++){
                step_forward<N>();
            }
            iter_ptr_ = start_ptr_ + layout_(coordinates_);
            return *this;
        }
        
        /*!
         * \brief decreases the iterator by n positions
         */
        HRef_iterator& HRef_iterator::operator-=(difference_type n){
            for (auto cnt = 0; cnt < n; cnt++){
                step_back<N>();
            }
            iter_ptr_ = start_ptr_ + layout_(coordinates_);
            return *this;
        }

        /*!
         * \brief advances the iterator by n positions
         */
        HRef_iterator HRef_iterator::operator+(difference_type n){
            HRef_iterator retval = *this;
            retval += n;
            return retval;
        }

        /*!
         * \brief decreases the iterator by n positions
         */
        HRef_iterator HRef_iterator::operator+(difference_type n){
            HRef_iterator retval = *this;
            retval = n;
            return retval;
        }


        friend HRef_iterator operator+(difference_type, const HRef_iterator&); const
        HRef_iterator HRef_iterator::operator-(difference_type) const;
        difference_type HRef_iterator::operator-(const HRef_iterator&) const;

        /*~~~~~~~~~~~~~~~~~~~~~~~~
        equality operators
        ~~~~~~~~~~~~~~~~~~~~~~~*/  //TODO: requires the comparisons of layouts? Probably not
        friend bool operator==(HRef_iterator, HRef_iterator);  // required
        friend bool operator!=(HRef_iterator, HRef_iterator);  // [note]
        friend bool operator<(HRef_iterator, HRef_iterator);   // [note]
        friend bool operator<=(HRef_iterator, HRef_iterator);  // [note]
        friend bool operator>(HRef_iterator, HRef_iterator);   // [note]
        friend bool operator>=(HRef_iterator, HRef_iterator);  // [note]

        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;
        bool operator<(const iterator&) const; //optional
        bool operator>(const iterator&) const; //optional
        bool operator<=(const iterator&) const; //optional
        bool operator>=(const iterator&) const; //optional

        /*~~~~~~~~~~~~~~~~~
        sentinel operators
        ~~~~~~~~~~~~~~~~~*/ //TODO: let's keep this for last
        bool operator==(HRef_iterator, sentinel);
        bool operator!=(HRef_iterator, sentinel);
        bool operator==(sentinel, HRef_iterator);
        bool operator!=(sentinel, HRef_iterator);
        difference_type operator-(sentinel, HRef_iterator); // Not required, but useful

        // HRef_iterator& operator++() { m_ptr++; return *this; } 
        // HRef_iterator operator++(int) { HRef_iterator tmp = *this; ++(*this); return tmp; }
        // friend bool operator== (const HRef_iterator& a, const HRef_iterator& b) { return a.m_ptr == b.m_ptr; };
        // friend bool operator!= (const HRef_iterator& a, const HRef_iterator& b) { return a.m_ptr != b.m_ptr; };  //TODO: requires the comparisons of layouts as well, which is not implemented

    private:
        pointer start_ptr_;
        pointer iter_ptr_;
        Layout<N> iter_layout_; //NOTE: should this be a pointer rather than a copy of the layout? perhaps using a smart pointer
        std::array<difference_type, N> coordinates_;

        /*!
         * \brief helper function to implement the ++ operator
         */
        template<size_t Coord>
        void step_forward(){
            if ( coordinates_[Coord] < (iter_layout_.length(Coord) -1) ){
                coordinates_[Coord] += 1;
            } else{
                coordinates_[Coord] = 0;
                step_forward<Coord-1>()
            }
        }

        /*!
         * \brief helper function to implement the ++ operator
         */
        template<>
        void step_forward<0>(){
            if ( coordinates_[0] < (iter_layout_.length(0) -1) ){
                coordinates_[0] += 1;
            } else{ //end of the container
                for (auto cnt = 0; cnt < (N-1) ; cnt++){
                    coordinates_ = iter_layout_.length(cnt) -1;
                }
                coordinates_(N-1) = iter_layout_.length(N-1); //coordinates to one past the last element of the container
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
                coordinates_[Coord] = iter_layout_.length(Coord) -1;
                step_back<Coord-1>()
            }
        }

        /*!
         * \brief helper function to implement the -- operator
         */
        template<>
        void step_back<0>(){
            if ( coordinates_[0] > 0 ){
                coordinates_[0] -= 1;
            } else{ //beginning of the container
                for (auto& coord : coordinates_){
                    coord = 0;
                }
            }
        }
};


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





} //namespace holor

#endif // HOLOR_REF_ITERATOR_H
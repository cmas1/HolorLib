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



#ifndef HOLOR_REF_H
#define HOLOR_REF_H

#include <cstddef>
#include <vector>
#include <type_traits>
#include <iterator>

#include "layout.h"
#include "initializer.h"
#include "../common/common_predicates.h"


namespace holor{

/*================================================================================================
                                    HolorRef Class
================================================================================================*/
/// HolorRef class
/*!
 * Class providing a dense implementation of a general n-dimensional tensor container.
 * Since the purpose of this class is to provide a container object, tensor arithmetic operations are not supported.
 * 
 * It uses a row-major representation, i.e. the elements of rows are contiguous and the elements of a columne are separated by a fixed number of elements (a stride)
 * 
 * //TODO: explain better what is a tensor (number of dimensions, extensions, slicing, access)
 */
template<typename T, size_t N>
class HolorRef{   

    public:


        /*============================================================
                            CUSTOM ITERATOR
        =============================================================*/
        template<bool IsConst>
        friend class Iterator;

        struct begin_iterator_tag{};
        struct end_iterator_tag{};

        /*!
        * \brief class that implements a random-acces iterator for the Holor_Ref view container.
        * For a brief description of the properties of random-access iterators refer to https://www.cplusplus.com/reference/iterator/RandomAccessIterator/
        * or to https://en.cppreference.com/w/cpp/iterator/random_access_iterator
        */
        template<bool IsConst>
        class Iterator {
            public:
                using iterator_category = std::random_access_iterator_tag;
                using difference_type = std::ptrdiff_t;
                using value_type = T;
                using pointer = typename impl::choose<IsConst, const T*, T*>::type;
                using reference = typename impl::choose<IsConst, const T&, T&>::type;
                using holor_pointer = typename impl::choose<IsConst, const HolorRef<T,N>*, HolorRef<T,N>*>::type;

                /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                constructors/destructors/assignments
                ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
                explicit Iterator(holor_pointer holor, begin_iterator_tag){
                    start_ptr_ = holor->dataptr_;
                    layout_ptr_ = &(holor->layout_);
                    coordinates_.fill(0);
                    iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
                }


                // Function used for geeting the end iterator.
                explicit Iterator(holor_pointer holor, end_iterator_tag){
                    start_ptr_ = holor->dataptr_;
                    layout_ptr_ = &(holor->layout_);
                    for (auto cnt = 0; cnt < (N-1) ; cnt++){
                        coordinates_[cnt] = layout_ptr_->length(cnt) -1;
                    }
                    coordinates_[N-1] = layout_ptr_->length(N-1); //coordinates to one past the last element of the container
                    iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
                }


                //copy constructor of  const_iterator from iterator
                template<bool IsConst_ = IsConst, class = std::enable_if_t<IsConst_>>
                Iterator(const Iterator<false>& rhs): start_ptr_(rhs.start_ptr_), iter_ptr_(rhs.iter_ptr_), layout_ptr_(rhs.layout_ptr_), coordinates_(rhs.coordinates_){};

                Iterator() = default; // default constructible
                Iterator(const Iterator&) = default; //copy constructible
                Iterator& operator=(const Iterator&) = default; //copy-assignable
                ~Iterator() = default; //destructible

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
                Iterator& operator++(){
                    step_forward<N>();
                    iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
                    return *this;
                }

                /*!
                * \brief postfix ++
                */
                Iterator operator++(int){
                    Iterator retval = *this;
                    ++(*this);
                    return retval;
                }

                /*!
                * \brief prefix --
                */
                Iterator& operator--(){
                    step_back<N>();
                    iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
                    return *this;
                }

                /*!
                * \brief postfix --
                */
                Iterator operator--(int){
                    Iterator retval = *this;
                    --(*this);
                    return retval;
                }

                /*!
                * \brief advances the iterator by n positions
                */
                Iterator& operator+=(difference_type n){
                    for (auto cnt = 0; cnt < n; cnt++){
                        step_forward<N>();
                    }
                    iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
                    return *this;
                }
                
                /*!
                * \brief decreases the iterator by n positions
                */
                Iterator& operator-=(difference_type n){
                    for (auto cnt = 0; cnt < n; cnt++){
                        step_back<N>();
                    }
                    iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
                    return *this;
                }

                /*!
                * \brief given the iterator a, it implements the operation a + n
                */
                Iterator operator+(difference_type n) const{
                    Iterator retval = *this;
                    retval += n;
                    return retval;
                }

                /*!
                * \brief given the iterator a, it implements the operation a - n
                */
                Iterator operator-(difference_type n) const{
                    Iterator retval = *this;
                    retval -= n;
                    return retval;
                }


                /*!
                * \brief implements the operation n + a
                */
                friend Iterator operator+(difference_type n, const Iterator& a) {
                    Iterator retval = a;
                    retval += n;
                    return retval;
                }


                /*!
                * \brief given two iterators a and b, it implements the difference a - b
                */
                friend difference_type operator-(const Iterator& a, const Iterator&b) {
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
                bool operator==(const Iterator& rhs) const{
                    return iter_ptr_ == rhs.iter_ptr_;
                }  

                /*!
                * \brief equality operations to compare two iterators. For example, needed to test iter != end()
                */
                bool operator!=(const Iterator& rhs) const{
                    return iter_ptr_ != rhs.iter_ptr_;
                }

                /*!
                * \brief defualt three-way comparison operator
                */
                friend auto operator<=>(Iterator, Iterator) = default;


                //TODO: we should consider implementing sentinel operations to support c++20 ranges functions (https://en.cppreference.com/w/cpp/ranges). First, I should study better ranges... !See https://www.foonathan.net/2020/03/iterator-sentinel/ for more info on how to implement these sentinels

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
        /*==================================================================================================================
                                            End of custom iterator
        ==================================================================================================================*/



        


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                    ALIASES
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        static constexpr size_t dimensions = N; ///! \brief number of dimensions in the container 

        using value_type = T; ///! type of the values in the container
        using iterator = typename HolorRef<T,N>::Iterator<false>; ///! iterator type for the underlying data storage
        using const_iterator = typename HolorRef<T,N>::Iterator<true>; ///! iterator type for the underlying data storage
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                CONSTRUCTORS, ASSIGNMENTS AND DESTRUCTOR
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Default constructor.
         * 
         * \return A HolorRef with zero elements on each dimension
         */
        HolorRef() = default;


        /*!
         * \brief Default move constructor.
         * 
         * \return A HolorRef equal to the argument
         */
        HolorRef(HolorRef&&) = default;


        /*!
         * \brief Default copy constructor.
         * 
         * \return A HolorRef equal to the argument
         */
        HolorRef(const HolorRef&) = default;


        /*!
         * \brief Default move assignement.
         * 
         * \return A HolorRef equal to the argument
         */
        HolorRef& operator=(HolorRef&&) = default;


        /*!
         * \brief Default copy assignement.
         * 
         * \return A HolorRef equal to the argument
         */
        HolorRef& operator=(const HolorRef&) = default;


        /*!
         *  \brief Default destructor.
         */
        ~HolorRef() = default;


        /*!
         * \brief Constructor that creates a HolorRef by specifying a data pointer and a layout
         * \param dataptr pointer to the location where the data is hosted
         * \param layout layout that indicates how the elements stored in the location pointer by dataptr can be indexed
         * \return a HolorRef
         */
        HolorRef(T* dataptr, const Layout<N>& layout): layout_{layout}, dataptr_{dataptr}{}


        //TODO:  Should add a constructor from a datapointer plus lengths. Can we implement it using ranges?
        
        

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            GET/SET FUNCTIONS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Function that returns the Layout used by the HolorRef to store and index the data
         * 
         * \return Layout
         */
        const Layout<N>& layout() const{
            return layout_;
        }

        /*!
         * \brief Function that returns the number of elements along each of the container's dimensions
         * 
         * \return the lengths of each dimension of the HolorRef container 
         */
        auto lengths() const{
            return layout_.lengths();
        }

        /*!
         * \brief Function that returns the total number of elements in the container
         * 
         * \return the total number of elements in the container
         */
        size_t size() const{
            return layout_.size();
        }

        /*!
         * \brief Function that provides a flat access to the data contained in the container
         * 
         * \return a pointer to the data stored in the container
         */
        T* dataptr(){
            return dataptr_;
        }
        
        /*!
         * \brief Function that provides a flat access to the data contained in the container
         * 
         * \return a const pointer to the data stored in the container
         */
        const T* dataptr() const{
            return dataptr_;
        }


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            ITERATORS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        auto begin(){
            return iterator(this, begin_iterator_tag{});
        }

        auto end(){
            return iterator(this, end_iterator_tag{});
        }

        auto cbegin() const{
            return const_iterator(this, begin_iterator_tag{});
        }

        auto cend() const{
            return const_iterator(this, end_iterator_tag{});
        }

        auto rbegin(){
            return reverse_iterator(end());
        }

        auto rend(){
            return reverse_iterator(begin());
        }

        auto crbegin(){
            return reverse_iterator(cend());
        }

        auto crend(){
            return reverse_iterator(cbegin());
        }

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            ACCESS FUNCTIONS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Access tensor element subscripting with integers without range check
         * \param dims pack of indices, one per dimension of the HolorRef container
         * \return the value of the HolorRef stored at the position indexed by the indices
         */
        template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
        T& operator()(Dims... dims){
            return *(dataptr_ + layout_(dims...));
        }


        /*!
         * \brief Access tensor element subscripting with integers without range check
         * \param dims pack of indices, one per dimension of the HolorRef container
         * \return the value of the HolorRef stored at the position indexed by the indices
         */
        template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
        const T operator()(Dims... dims) const{
            return *(dataptr_ + layout_(dims...));
        }


        /*!
         * \brief Access tensor slice by subscripting with integers without range check
         * \param dims pack of indices, one per dimension of the HolorRef container
         * \return the value of the HolorRef stored at the position indexed by the indices
         */
        template<typename... Args> requires (impl::range_indexing<Args...>() && (sizeof...(Args)==N) )
        auto operator()(Args... args) {
            //TODO: the size of the HolorRef depends on the size of the Layout. We need to extract the size of the layout
            auto sliced_layout = layout_(args...);
            return HolorRef<T, decltype(sliced_layout)::order>(dataptr_, sliced_layout);
        };


        /*!
         * \brief function that returns the `i-th` row of the tensor
         * \param i index of the row to be indexed
         * \return a reference container to the row 
         */
        HolorRef<T, N-1> row(size_t i){
            return HolorRef<T, N-1>(dataptr_, layout_.template slice_dimension<0>(i));
        }


        /*!
         * \brief function that returns the `i-th` row of the tensor
         * \param i index of the row to be indexed
         * \return a reference container to the row 
         */
        const HolorRef<T, N-1> row(size_t i) const{
            return HolorRef<T, N-1>(dataptr_, layout_.template slice_dimension<0>(i));
        }

        
        /*!
         * \brief function that returns the `i-th` column of the tensor
         * \param i index of the column to be indexed
         * \return a reference container to the column 
         */
        HolorRef<T, N-1> col(size_t i){
            return HolorRef<T, N-1>(dataptr_, layout_.template slice_dimension<1>(i));
        }

      
        /*!
         * \brief function that returns the `i-th` column of the tensor
         * \param i index of the column to be indexed
         * \return a reference container to the column 
         */
        const HolorRef<T, N-1> col(size_t i) const{
            return HolorRef<T, N-1>(dataptr_, layout_.template slice_dimension<1>(i));
        }


        /*!
         * \brief function that returns the `i-th` slice of a single dimension
         * \tparam M is the dimension to be sliced. 0 is a row, 1 is a column, ...
         * \param i index of the slice alonge the `M-th` dimension
         * \return a reference container to the slice 
         */
        template<size_t M>
        HolorRef<T, N-1> slice(size_t i){
            return HolorRef<T, N-1>(dataptr_, layout_.template slice_dimension<M>(i));
        }

     
        /*!
         * \brief function that returns the `i-th` slice of a single dimension
         * \tparam M is the dimension to be sliced. 0 is a row, 1 is a column, ...
         * \param i index of the slice alonge the `M-th` dimension
         * \return a reference container to the slice 
         */
        template<size_t M>
        const HolorRef<T, N-1> slice(size_t i) const{
            return HolorRef<T, N-1>(dataptr_, layout_.template slice_dimension<M>(i));
        }


    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        PRIVATE MEMBERS AND FUNCTIONS
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    private:
        Layout<N> layout_; ///\brief The Layout of how the elements of the container are stored in memory
        T* dataptr_; ///! \brief Pointer to the memory location where the data is stored

};


} //namespace holor

#endif // HOLOR_REF_H
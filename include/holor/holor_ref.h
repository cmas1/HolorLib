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



#ifndef HOLOR_REF_H
#define HOLOR_REF_H

#include <cstddef>
#include <vector>

#include "layout.h"
#include "initializer.h"
#include "../common/static_assertions.h"


namespace holor{

/*================================================================================================
                                    HolorRef Class
================================================================================================*/
/*!
 * \brief Class implementing a general N-dimensional container which does not own its elements, but rather provides a multi-dimensional view of the data stored in a certain memory location.
 * 
 * A HolorRef is intended as a general `N`-dimensional container without ownership, whose elements need not to be numerical types, but can be of a generic type `T`. 
 * HolorRefs use a row-major representation, i.e., the elements of last dimension of the container are contiguous. For more information on row-major ordering please refer to https://en.wikipedia.org/wiki/Row-_and_column-major_order.
 * 
 * \tparam N the number of dimensions of the container. For example, for a matrix-like container it is `N-2`.
 * \tparam T the type of the elements stored in the container.
 */
template<typename T, size_t N>
class HolorRef{   

    public:


        /*============================================================
                            CUSTOM ITERATOR
        =============================================================*/
        //Forward declaration of the iterator class for HolorRef containers
        template<bool IsConst>
        friend class Iterator;

        struct begin_iterator_tag{}; //empty structure used to tag the begin iterator
        struct end_iterator_tag{}; //empty structure used to tag the end iterator

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
                using pointer = typename assert::choose<IsConst, const T*, T*>::type;
                using reference = typename assert::choose<IsConst, const T&, T&>::type;
                using holor_pointer = typename assert::choose<IsConst, const HolorRef<T,N>*, HolorRef<T,N>*>::type;


                /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                constructors/destructors/assignments
                ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
                //! \brief construct an iterator at the beginning of the container
                explicit Iterator(holor_pointer holor, begin_iterator_tag){
                    start_ptr_ = holor->dataptr_;
                    layout_ptr_ = &(holor->layout_);
                    coordinates_.fill(0);
                    iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
                    compute_iterator_strides();
                }


                //! \brief construct an iterator at the end of the container
                explicit Iterator(holor_pointer holor, end_iterator_tag){
                    start_ptr_ = holor->dataptr_;
                    layout_ptr_ = &(holor->layout_);
                    for (auto cnt = 0; cnt < (N-1) ; cnt++){
                        coordinates_[cnt] = layout_ptr_->length(cnt) - 1;
                    }
                    coordinates_[N-1] = layout_ptr_->length(N-1); //one element past the last in the container
                    iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_); 
                    compute_iterator_strides();
                }


                //! \brief copy constructor of  const_iterator from iterator
                template<bool IsConst_ = IsConst, class = std::enable_if_t<IsConst_>>
                Iterator(const Iterator<false>& rhs): start_ptr_(rhs.start_ptr_), iter_ptr_(rhs.iter_ptr_), layout_ptr_(rhs.layout_ptr_), coordinates_(rhs.coordinates_), iterator_strides_(rhs.iterator_strides_){};

                Iterator() = default;                           ///< \brief default constructible
                Iterator(const Iterator&) = default;            ///< \brief copy constructible
                Iterator& operator=(const Iterator&) = default; ///< \brief copy-assignable
                ~Iterator() = default;                          ///< \brief destructible



                /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                reference/dereference operators
                ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
                //! \brief dereference operator as an rvalue or lvalue (if in a dereferenceable state)
                reference operator*() const {
                    return *iter_ptr_;
                }
                
                //! \brief dereference operator as an rvalue (if in a dereferenceable state)
                pointer operator->() const {
                    return &(*iter_ptr_);
                }

                //! \brief offset dereference operator
                reference operator[](difference_type n) const {
                    return *((this + n)->iter_ptr_);
                } 

                /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                increment operators 
                ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
                //! \brief prefix ++
                Iterator& operator++(){
                    step_forward<N-1>();
                    iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
                    return *this;
                }

                //! \brief postfix ++
                Iterator operator++(int){
                    Iterator retval = *this;
                    ++(*this);
                    return retval;
                }

                //! \brief prefix --
                Iterator& operator--(){
                    step_back<N-1>();
                    iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
                    return *this;
                }

                //! \brief postfix --
                Iterator operator--(int){
                    Iterator retval = *this;
                    --(*this);
                    return retval;
                }

                //! \brief advances the iterator by n positions
                Iterator& operator+=(difference_type n){
                    for (auto cnt = 0; cnt < n; cnt++){
                        step_forward<N-1>();
                    }
                    iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
                    return *this;
                }
                
                //! \brief decreases the iterator by n positions
                Iterator& operator-=(difference_type n){
                    for (auto cnt = 0; cnt < n; cnt++){
                        step_back<N-1>();
                    }
                    iter_ptr_ = start_ptr_ + layout_ptr_->operator()(coordinates_);
                    return *this;
                }

                //! \brief given the iterator a, it implements the operation a + n
                Iterator operator+(difference_type n) const{
                    Iterator retval = *this;
                    retval += n;
                    return retval;
                }

                //! \brief given an iterator a, it implements the operation a - n
                Iterator operator-(difference_type n) const{
                    Iterator retval = *this;
                    retval -= n;
                    return retval;
                }

                //!\brief implements the operation n + a
                friend Iterator operator+(difference_type n, const Iterator& a) {
                    Iterator retval = a;
                    retval += n;
                    return retval;
                }

                //! \brief given two iterators a and b, it implements the difference a - b
                friend difference_type operator-(const Iterator& a, const Iterator&b) {
                    difference_type result = 0;
                    for (auto cnt = 0; cnt<N; cnt++){
                        result += a.coordinates_[cnt]*a.iterator_strides_[cnt] - b.coordinates_[cnt]*b.iterator_strides_[cnt];
                    }
                    return result;
                }

                /*~~~~~~~~~~~~~~~~~~~~~~~~
                equality operators
                ~~~~~~~~~~~~~~~~~~~~~~~*/
                //! \brief equality operations to compare two iterators. For example, needed to test iter == end()
                bool operator==(const Iterator& rhs) const{
                    return iter_ptr_ == rhs.iter_ptr_;
                }  

                //! \brief equality operations to compare two iterators. For example, needed to test iter != end()
                bool operator!=(const Iterator& rhs) const{
                    return iter_ptr_ != rhs.iter_ptr_;
                }

                //! \brief defualt three-way comparison operator
                friend auto operator<=>(Iterator, Iterator) = default;


            private:
                pointer start_ptr_;                             ///< \brief pointer to initial memory location addressed by the Holor_Ref that the iterator refers to. This is needed because the elements are not stored contiguously in memory                
                pointer iter_ptr_;                              ///< \brief pointer to current memory location addressed by the iterator
                const Layout<N>* layout_ptr_;                   ///< \brief pointer to the layout of the Holor_Ref container that the iterator refers to. This is needed because the elements are not stored contiguously in memory
                std::array<difference_type, N> coordinates_;    ///< \brief coordinates of the current iterator from the starting pointer. This is needed because the elements are not stored contiguously in memory
                std::array<size_t, N> iterator_strides_;        ///< \brief internal strides of the iterator (they are different from the ones in the layout) which are used to compute the distance between two iterators, i.e., the nubmer of increments to go from an iterator to another one


                /*!
                * \brief Computes inner strides, needed to correctly implement the distance between two iterators
                */
                void compute_iterator_strides(){
                    size_t tmp = 1;
                    for(int i = N-1; i>=0; --i){
                        iterator_strides_[i] = tmp;
                        tmp *= layout_ptr_->length(i);
                    }
                }


                //! \brief helper function to implement the ++ operator
                template<size_t Coord>
                void step_forward(){
                    if ( coordinates_[Coord] < (layout_ptr_->length(Coord) -1) ){
                        coordinates_[Coord] += 1;
                    } else{
                        coordinates_[Coord] = 0;
                        step_forward<Coord-1>();
                    }
                }

                template<>
                void step_forward<0>(){
                    if ( coordinates_[0] < (layout_ptr_->length(0) -1) ){
                        coordinates_[0] += 1;
                    } else{ //end of the container
                        for (auto cnt = 0; cnt < (N-1) ; cnt++){
                            coordinates_[cnt] = layout_ptr_->length(cnt) -1;
                        }
                        coordinates_[N-1] = layout_ptr_->length(N-1); //coordinates to one past the last element of the container
                    }
                }

                //! \brief helper function to implement the -- operator
                template<size_t Coord>
                void step_back(){
                    if ( coordinates_[Coord] > 0 ){
                        coordinates_[Coord] -= 1;
                    } else{
                        coordinates_[Coord] = layout_ptr_->length(Coord) -1;
                        step_back<Coord-1>();
                    }
                }

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
        /*==================================================================================================================
                                            End of custom iterator
        ==================================================================================================================*/

      


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                    ALIASES
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        static constexpr size_t dimensions = N;                                 ///< \brief number of dimensions in the container 
        using value_type = T;                                                   ///< \brief type of the values in the container
        using iterator = typename HolorRef<T,N>::Iterator<false>;               ///< \brief type of the iterator for the container
        using const_iterator = typename HolorRef<T,N>::Iterator<true>;          ///< \brief type of the const_iterator for the container
        using reverse_iterator = std::reverse_iterator<iterator>;               ///< \brief type of the reverse_iterator for the container
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;   ///< \brief type of the const_reverse_iterator for the container



        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                CONSTRUCTORS, ASSIGNMENTS AND DESTRUCTOR
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        HolorRef() = default;                           ///< \brief Default constructor
        HolorRef(HolorRef<T, N>&& holor_ref) = default;                 ///< \brief Default move constructor.
        HolorRef(const HolorRef<T, N>& holor_ref) = default;            ///< \brief Default copy constructor.
        HolorRef& operator=(HolorRef<T, N>&& holor_ref) = default;      ///< \brief Default move assignement.
        HolorRef& operator=(const HolorRef<T, N>& holor_ref) = default; ///< \brief Default copy assignement.
        ~HolorRef() = default;                          ///< \brief Default destructor.

        /*!
         * \brief Constructor that creates a HolorRef by specifying a data pointer and a layout
         * \param dataptr pointer to the location where the data is hosted
         * \param layout layout that indicates how the elements stored in the location pointer by dataptr can be indexed
         * \return a HolorRef
         */
        HolorRef(T* dataptr, const Layout<N>& layout): layout_{layout}, dataptr_{dataptr}{}
        HolorRef(T* dataptr, Layout<N>&& layout): layout_{layout}, dataptr_{dataptr}{}


        /*!
         * \brief Constructor that creates a HolorRef by specifying a data pointer and a container of lengths with compile-time size equal to `N` (e.g., `std::array<size_t,N>`).
         * \param dataptr pointer to the location where the data is hosted
         * \param lengths container of the number of elements along each dimension of the layout
         * \return a HolorRef
         */
        template <class Container> requires assert::SizedTypedContainer<Container, size_t, N>
        explicit HolorRef(T* dataptr, const Container& lengths): layout_{lengths}, dataptr_{dataptr}{}
        
        
        /*!
         * \brief Constructor that creates a HolorRef by specifying a data pointer and a container of lengths with run-time size equal to `N` (e.g., `std::vector<size_t>`).
         * \param dataptr pointer to the location where the data is hosted
         * \param lengths container of the number of elements along each dimension of the layout
         * \return a HolorRef
         */
        template <class Container> requires assert::ResizeableTypedContainer<Container, size_t>
        explicit HolorRef(T* dataptr, const Container& lengths): layout_{lengths}, dataptr_{dataptr}{}        

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            GET/SET FUNCTIONS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Get the Layout used by the HolorRef to index the data in memory
         * \return Layout
         */
        const Layout<N>& layout() const{
            return layout_;
        }

        /*!
         * \brief Get the number of elements along each of the container's dimensions
         * \return the lengths of each dimension of the HolorRef container 
         */
        auto lengths() const{
            return layout_.lengths();
        }

        /*!
         * \brief Get the number of elements along a specific dimension of the container
         * \param dim dimension ti inquire for its number of elements
         * \return a single length
         */
        auto length(size_t dim) const{
            return layout_.length(dim);
        }

        /*!
         * \brief Get the total number of elements in the container
         * \return the total number of elements in the container
         */
        size_t size() const{
            return layout_.size();
        }

        /*!
         * \brief Get a flat access to the data contained in the container
         * \return a pointer to the data stored in the container
         */
        T* dataptr(){
            return dataptr_;
        }
        
        const T* dataptr() const{
            return dataptr_;
        }


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            ITERATORS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        auto begin(){ return iterator(this, begin_iterator_tag{}); } ///< \brief returns an iterator to the beginning
        auto end(){ return iterator(this, end_iterator_tag{}); } ///< \brief returns an iterator to the end

        auto cbegin() const{ return const_iterator(this, begin_iterator_tag{}); } ///< \brief returns a constant iterator to the beginning
        auto cend() const{ return const_iterator(this, end_iterator_tag{}); } ///< \brief returns a constant iterator to the end

        auto rbegin(){ return reverse_iterator(end()); } ///< \brief returns a reverse iterator to the beginning
        auto rend(){ return reverse_iterator(begin()); } ///< \brief returns a reverse iterator to the end

        auto crbegin() const{ return reverse_iterator(cend()); } ///< \brief returns a constant reverse iterator to the beginning
        auto crend() const{ return reverse_iterator(cbegin()); } ///< \brief returns a constant reverse iterator to the end


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            ACCESS FUNCTIONS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Access a single element in the container
         * \param dims pack of indices, one per dimension of the Holor container
         * \return the value of the Holor stored at the position indexed by the indices
         */
        template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
        T& operator()(Dims&&... dims){
            return *(dataptr_ + layout_(std::forward<Dims>(dims)...));
        }

        template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
        const T operator()(Dims&&... dims) const{
            return *(dataptr_ + layout_(std::forward<Dims>(dims)...));
        }


        /*!
         * \brief Access a slice of the container by providing a single index or a range of indices for each dimension
         * \param dims pack of indices, one per dimension of the container
         * \return the value of the Holor stored at the position indexed by the indices
         */
        template<typename... Args> requires (impl::ranged_index_pack<Args...>() && (sizeof...(Args)==N) )
        auto operator()(Args&&... args) {
            auto sliced_layout = layout_(std::forward<Args>(args)...);
            return HolorRef<T, decltype(sliced_layout)::order>(dataptr_, sliced_layout);
        };


        /*!
         * \brief Access the `i-th` row of the container
         * \param i index of the row to be indexed
         * \return a reference container to the row 
         */
        HolorRef<T, N-1> row(size_t i){
            return HolorRef<T, N-1>(dataptr_, layout_.template slice_dimension<0>(i));
        }

        const HolorRef<T, N-1> row(size_t i) const{
            return HolorRef<T, N-1>(dataptr_, layout_.template slice_dimension<0>(i));
        }

        
        /*!
         * \brief Access the `i-th` column of the container
         * \param i index of the column to be indexed
         * \return a reference container to the column 
         */
        HolorRef<T, N-1> col(size_t i){
            return HolorRef<T, N-1>(dataptr_, layout_.template slice_dimension<1>(i));
        }

        const HolorRef<T, N-1> col(size_t i) const{
            return HolorRef<T, N-1>(dataptr_, layout_.template slice_dimension<1>(i));
        }


        /*!
         * \brief Access the `i-th` slice of a single dimension
         * \tparam M is the dimension to be sliced. 0 is a row, 1 is a column, ...
         * \param i index of the slice alonge the `M-th` dimension
         * \return a reference container to the slice 
         */
        template<size_t M>
        HolorRef<T, N-1> slice(size_t i){
            return HolorRef<T, N-1>(dataptr_, layout_.template slice_dimension<M>(i));
        }

        template<size_t M>
        const HolorRef<T, N-1> slice(size_t i) const{
            return HolorRef<T, N-1>(dataptr_, layout_.template slice_dimension<M>(i));
        }



    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        ELEMENT ASSIGNMENT FUNCTIONS
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*!
     * \brief assign the elements in the HolorRef container from another HolorRef container with the same type of elements, dimensions and extents
     * \param rhs HolorRef containr from where the values are copied
     */
    void substitute(const HolorRef<T,N>& rhs){
        assert::dynamic_assert(this->layout_.lengths() == rhs.lengths(), EXCEPTION_MESSAGE("Incompatible dimensions."));
        std::copy(rhs.cbegin(), rhs.cend(), this->begin());
    }

    /*!
     * \brief assign the elements in the HolorRef container from an Holor container with the same type of elements, dimensions and extents
     * \tparam HolorContainer template parameter used to refer to type of the Holor container
     * \param rhs HolorRef containr from where the values are copied
     */
    //TODO: use a proper concept for HolorContainer
    template<typename HolorContainer> requires ( (HolorContainer::dimensions == N) && (std::is_same_v<typename HolorContainer::value_type, T>) )
    void substitute(const HolorContainer& rhs){
        assert::dynamic_assert(this->layout_.lengths() == rhs.lengths(), EXCEPTION_MESSAGE("Incompatible dimensions."));
        std::copy(rhs.cbegin(), rhs.cend(), this->begin());
    }

    template<typename HolorContainer> requires ( (HolorContainer::dimensions == N) && (std::is_same_v<typename HolorContainer::value_type, T>) )
    void substitute(HolorContainer&& rhs){
        assert::dynamic_assert(this->layout_.lengths() == rhs.lengths(), EXCEPTION_MESSAGE("Incompatible dimensions."));
        std::move(rhs.begin(), rhs.end(), this->begin());
    }


private:
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        PRIVATE MEMBERS AND FUNCTIONS
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/    
        Layout<N> layout_;  ///< \brief The Layout of how the elements of the container are stored in memory
        T* dataptr_;        ///< \brief Pointer to the memory location where the data is stored

};


} //namespace holor

#endif // HOLOR_REF_H
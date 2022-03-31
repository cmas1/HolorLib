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


#ifndef HOLOR_H
#define HOLOR_H


#include <cstddef>
#include <vector>

#include "holor_ref.h"
#include "holor_types.h"
#include "layout.h"
#include "initializer.h"



namespace holor{


/*================================================================================================
                                    Holor Class
================================================================================================*/
/*!
 * \brief Class implementing a general N-dimensional container with contiguous storage in memory.
 * 
 * A Holor is intended as a general `N`-dimensional container, whose elements need not to be numerical types, but can be of a generic type `T`. 
 * Holors are implemented with a row-major representation, i.e., the elements of last dimension of the container are contiguous. For more information on row-major ordering please refer to https://en.wikipedia.org/wiki/Row-_and_column-major_order.
 * 
 * \tparam N the number of dimensions of the container. For example, for a matrix-like container it is `N-2`.
 * \tparam T the type of the elements stored in the container.
 */
template<typename T, size_t N> requires (N>0)
class Holor{   

    public:
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                    ALIASES
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        static constexpr size_t dimensions = N;                                         ///< \brief number of dimensions in the container 
        using value_type = T;                                                           ///< \brief type of the values in the container
        using iterator = typename std::vector<T>::iterator;                             ///< \brief type of the iterator for the container
        using const_iterator = typename std::vector<T>::const_iterator;                 ///< \brief type of the const_iterator for the container
        using reverse_iterator = typename std::vector<T>::reverse_iterator;             ///< \brief type of the reverse_iterator for the container
        using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator; ///< \brief type of the const_reverse_iterator for the container
        using holor_type = holor::impl::HolorOwningTypeTag;                             ///< \brief tags a Holor type with ownership over its data


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                CONSTRUCTORS, ASSIGNMENTS AND DESTRUCTOR
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        Holor() = default;                                      ///< \brief default constructor with zero elements in every dimension
        Holor(Holor<T, N>&& holor) = default;                   ///< \brief default move constructor
        Holor(const Holor<T, N>& holor) = default;              ///< \brief default copy constructor
        Holor& operator=(Holor<T, N>&& holor) = default;        ///< \brief default move assignment
        Holor& operator=(const Holor<T, N>& holor) = default;   ///< \brief default copy assignment
        ~Holor() = default;                                     ///< \brief default destructor
    
        /*!
         * \brief Constructor that creates a Holor by specifying the length of each dimension
         * \param lengths fixed length container with `N` lengths (e.g., a std::array)
         * \return a Holor with specified lenghts but without initialization of its elements
         */
        template <class Container> requires assert::SizedTypedContainer<Container, size_t, N>
        explicit Holor(const Container& lengths): layout_{lengths}{
            data_.resize(layout_.size());
        }

        /*!
         * \brief Constructor that creates a Holor by specifying the length of each dimension
         * \param lengths variable length container with `N` lengths (e.g., a std::vector)
         * \return a Holor with specified lenghts but without initialization of its elements
         */
        template <class Container> requires assert::ResizeableTypedContainer<Container, size_t>
        explicit Holor(const Container& lengths): layout_{lengths}{
            data_.resize(layout_.size());
        }


        /*!
         * \brief Constructor from a HolorRef object. Only copy is allowed, because the HolorRef does not own the objects it contains.
         * \param ref a HolorRef object
         * \return a Holor
         */
        template<typename U> requires (std::convertible_to<U, T>)
        Holor(const HolorRef<U,N>& ref) {
            layout_ = Layout<N>(ref.layout().lengths());
            data_.resize(layout_.size());
            std::copy(ref.cbegin(), ref.cend(), data_.begin());
        }


        /*!
         * \brief Constructor from a nested list of elements
         * \param init nested list of the elements to be inserted in the container
         * \return a Holor containing the elements in the list
         */
        template<typename U> //requires (std::convertible_to<U, T>)
        Holor(holor::nested_list<U,N> init) {
            layout_ = Layout<N>(impl::derive_lengths<N>(init));
            data_.reserve(layout_.size());
            impl::insert_flat(init, data_);
        }

        Holor(holor::nested_list<T,N> init) {
            layout_ = Layout<N>(impl::derive_lengths<N>(init));
            data_.reserve(layout_.size());
            impl::insert_flat(init, data_);
        }


        /*!
         * \brief Assignment from a nested list of elements
         * \param init nested list of the elements to be inserted in the container
         * \return a reference to a Holor containing the elements in the list
         */
        template<typename U> //requires (std::convertible_to<U, T>)
        Holor& operator=(holor::nested_list<U,N> init) {
            layout_ = Layout<N>{impl::derive_lengths<N>(init)};
            data_.reserve(layout_.size());
            impl::insert_flat(init, data_);
            return *this;
        }

        Holor& operator=(holor::nested_list<T,N> init) {
            layout_ = Layout<N>{impl::derive_lengths<N>(init)};
            data_.reserve(layout_.size());
            impl::insert_flat(init, data_);
            return *this;
        }


        /*!
         * \brief Remove the constructor and assignment from a std::initializer_list, in order to allow using the constructor from holor::nested_list
         */
        // template<typename U>
        // Holor(std::initializer_list<U>) = delete;

        // template<typename U>
        // Holor& operator=(std::initializer_list<U>) = delete;

        
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            ITERATORS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        auto begin(){ return data_.begin(); } ///< \brief returns an iterator to the beginning
        auto end(){ return data_.end(); } ///< \brief returns an iterator to the end

        auto cbegin() const{ return data_.cbegin(); } ///< \brief returns a constant iterator to the beginning
        auto cend() const{ return data_.cend(); } ///< \brief returns a constant iterator to the end

        auto rbegin(){ return data_.rbegin(); } ///< \brief returns a reverse iterator to the beginning
        auto rend(){ return data_.rend(); } ///< \brief returns a reverse iterator to the end

        auto crbegin() const{ return data_.crbegin(); } ///< \brief returns a constant reverse iterator to the beginning
        auto crend() const{ return data_.crend(); } ///< \brief returns a constant reverse iterator to the end



        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            GET/SET FUNCTIONS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Function that returns the Layout used by the Holor to store and index the data
         * \return Layout
         */
        const Layout<N>& layout() const{
            return layout_;
        }

        /*!
         * \brief Function that returns the number of elements along each of the container's dimensions
         * \return the lengths of each dimension of the Holor container 
         */
        auto lengths() const{
            return layout_.lengths();
        }


        /*!
         * \brief Function changes the number of elements along each of the container's dimensions. This operation may destroy some elements or create new elements with unspecified values
         * \param lengths the lengths of each dimension of the Holor container 
         */
        template<typename... Lengths> requires ((sizeof...(Lengths)==N) && (assert::all(std::is_convertible_v<Lengths,size_t>...)) )
        void set_lengths(Lengths&&... lengths) {
            layout_.set_lengths(std::forward<Lengths>(lengths)...);
            data_.resize(layout_.size());
        }

        template <class Container> requires assert::SizedTypedContainer<Container, size_t, N>
        void set_lengths(const Container& lengths){
            layout_.set_lengths(lengths);
            data_.resize(layout_.size());
        }

        template <class Container> requires assert::ResizeableTypedContainer<Container, size_t>
        void set_lengths(const Container& lengths){
            layout_.set_lengths(lengths);
            data_.resize(layout_.size());
        }


        /*!
         * \brief Function that returns the number of elements along a specific dimension of the container
         * \param dim the dimension to be inquired for its length
         * \return the length of the selected dimension
         */
        auto length(size_t dim) const{
            return layout_.length(dim);
        }


        /*!
         * \brief Function changes the number of elements along a single dimension of the container. This operation may destroy some elements or create new elements with unspecified values
         * \param dim the dimension to modify
         * \param length the new length of the dimension
         */
        void set_length(size_t dim, size_t length){
            layout_.set_length(dim, length);
            data_.resize(layout_.size());
        }

        /*!
         * \brief Function that returns the total number of elements in the container
         * \return the total number of elements in the container
         */
        size_t size() const{
            return layout_.size();
        }

        /*!
         * \brief Function that provides a flat access to the data contained in the container
         * \return a pointer to the data stored in the container
         */
        T* data(){
            return data_.data();
        }
        
        /*!
         * \brief Function that provides a flat access to the data contained in the container
         * \return a const pointer to the data stored in the container
         */
        const T* data() const{
            return data_.data();
        }

        /*!
         * \brief Function that returns a copy of the container's data vector
         * \return the data as a vector
         */
        auto data_vector() const{
            return data_;
        }


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
            return *(data() + layout_(std::forward<Dims>(dims)...));
        }

        template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
        const T operator()(Dims&&... dims) const{
            return data_[layout_(std::forward<Dims>(dims)...)];
        }


        /*!
         * \brief Access a slice of the container by providing a single index or a range of indices for each dimension
         * \param dims pack of indices, one per dimension of the Holor container
         * \return the value of the Holor stored at the position indexed by the indices
         */
        template<typename... Args> requires (impl::ranged_index_pack<Args...>() && (sizeof...(Args)==N) )
        auto operator()(Args&&... args) {
            auto sliced_layout = layout_(std::forward<Args>(args)...);
            return HolorRef<T, decltype(sliced_layout)::order>(data_.data(), sliced_layout);
        };


        /*!
         * \brief Access the `i-th` row of the container
         * \param i index of the row to be indexed
         * \return a reference container to the row 
         */
        auto row(size_t i){
            return HolorRef<T, N-1>(data_.data(), layout_.template slice_dimension<0>(i));
        }

        const auto row(size_t i) const{
            return HolorRef<T, N-1>(data_.data(), layout_.template slice_dimension<0>(i));
        }

        
        /*!
         * \brief Access the `i-th` column of the container
         * \param i index of the column to be indexed
         * \return a reference container to the column 
         */
        auto col(size_t i){
            return HolorRef<T, N-1>(data_.data(), layout_.template slice_dimension<1>(i));
        }

        const auto col(size_t i) const{
            return HolorRef<T, N-1>(data_.data(), layout_.template slice_dimension<1>(i));
        }


        /*!
         * \brief Access the `i-th` slice of a single dimension (e.g., the fifth row or the second column)
         * \tparam M is the dimension to be sliced. 0 is a row, 1 is a column, ...
         * \param i index of the slice alonge the `M-th` dimension
         * \return a reference container to the slice 
         */
        template<size_t M> requires (M<N)
        auto slice(size_t i){
            return HolorRef<T, N-1>(data_.data(), layout_.template slice_dimension<M>(i));
        }

        template<size_t M> requires (M<N)
        const auto slice(size_t i) const{
            return HolorRef<T, N-1>(data_.data(), layout_.template slice_dimension<M>(i));
        }


    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        PRIVATE MEMBERS AND FUNCTIONS
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    private:
        Layout<N> layout_;      ///< \brief The Layout of how the elements of the container are stored in memory
        std::vector<T> data_;   ///< \brief Vector storing the actual data
};


} //namespace holor

#endif // HOLOR_H
// This file is part of Holor, a C++ template library for multi-dimensional containers

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



#ifndef HOLOR_H
#define HOLOR_H


//TODO: check all includes
#include <cstddef>
#include <vector>
#include <type_traits>

#include "holor_ref.h"
#include "layout.h"
#include "initializer.h"



namespace holor{


/*================================================================================================
                                    Holor Class
================================================================================================*/
/// Holor class
/*!
 * \brief Class implementing a general N-dimensional container with contiguous storage in memory.
 * 
 * A Holor is intended as a general `N`-dimensional container, whose elements need not to be numerical types, but can be of a generic type `T`. 
 * Holors are implemented with a row-major representation, i.e., the elements of last dimension of the container are contiguous. For more information on row-major ordering please refer to https://en.wikipedia.org/wiki/Row-_and_column-major_order.
 * 
 * \tparam N the number of dimensions of the container. For example, for a matrix-like container it is `N-2`.
 * \tparam T the type of the elements stored in the container.
 */
template<typename T, size_t N>
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


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                CONSTRUCTORS, ASSIGNMENTS AND DESTRUCTOR
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        Holor() = default;                          ///< \brief default constructor with zero elements in every dimension
        Holor(Holor&&) = default;                   ///< \brief default move constructor
        Holor(const Holor&) = default;              ///< \brief default copy constructor
        Holor& operator=(Holor&&) = default;        ///< \brief default move assignment
        Holor& operator=(const Holor&) = default;   ///< \brief default copy assignment
        ~Holor() = default;                         ///< \brief default destructor
    
        //TODO:  Do we need a constructor from lengths, perhaps with default initialization? Can we implement it using ranges?
        /*!
         * \brief Constructor that creates a Holor by specifying the length of each dimension
         * \param lengths array of `N` lengths
         * \return a Holor
         */
        Holor(const std::array<size_t,N>& lengths): layout_{lengths}{
            data_.reserve(layout_.size_);
        }

        /*!
         * \brief Constructor that creates a Holor by specifying the length of each dimension
         * \param lengths array of `N` lengths
         * \return a Holor
         */
        Holor(std::array<size_t,N>&& lengths): layout_{lengths}{
            data_.reserve(layout_.size_);
        }


        /*!
         * \brief Constructor from a HolorRef object. Only copy is allowed, because the HolorRef does not own the objects it contains.
         * \param ref a HolorRef object
         * \return a Holor
         */
        template<typename U> requires (std::convertible_to<U, T>)
        Holor(const HolorRef<U,N>& ref) {
            layout_.lengths_ = ref.layout_.lengths_;
            layout_.offset_ = 0;
            layout_.compute_strides();
            data_.reserve(layout_.size());
            this->push_ref_elements(ref.dataptr_, ref.layout_);
        }


        /*!
         * \brief Assignment from a HolorRef object
         * \param ref a HolorRef object
         * \return a Holor
         */
        template<typename U> requires (std::convertible_to<U, T>)
        Holor& operator=(const HolorRef<U,N>& ref){
            layout_.lengths_ = ref.layout_.lengths_;
            layout_.offset_ = 0;
            layout_.compute_strides();
            data_.reserve(ref.layout_.size());
            this->push_ref_elements(ref.dataptr_, ref.layout_);
            return *this;
        }

        
        /*!
         * \brief Constructor from a nested list of elements
         * \param init nested list of the elements to be inserted in the container
         * \return a Holor containing the elements in the list
         */
        Holor(holor::nested_list<T,N> init){
            layout_ = Layout<N>(impl::derive_lengths<N>(init));
            // layout_.set_offset(0);
            // layout_.set_lengths(impl::derive_lengths<N>(init));
            data_.reserve(layout_.size());
            impl::insert_flat(init, data_);
            // TODO: dynamic check that the  number of elements in the container matches the extents?
        }


        /*!
         * \brief Assign from a nested list of elements
         * \param init nested list of the elements to be inserted in the container
         * \return a reference to Holor
         */
        Holor& operator=(holor::nested_list<T,N> init){
            layout_.set_offset(0);
            layout_.set_lengths(impl::derive_lengths<N>(init));
            data_.reserve(layout_.size());
            impl::insert_flat(init, data_);
            // TODO: dynamic check that the  number of elements in the container matches the extents?
            return *this;
        }


        /*!
         * \brief Remove the constructor from a std::initializer_list, in order to allow using the constructor from holor::nested_list
         */
        template<typename U>
        Holor(std::initializer_list<U>) = delete;

        
        /*!
         * \brief Remove the assignment from a std::initializer_list, in order to allow using the assignment from holor::nested_list
         */
        template<typename U>
        Holor& operator=(std::initializer_list<U>) = delete;


        
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            ITERATORS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        auto begin(){
            return data_.begin();
        }

        auto end(){
            return data_.end();
        }

        auto cbegin() const{
            return data_.cbegin();
        }

        auto cend() const{
            return data_.cend();
        }

        auto rbegin(){
            return data_.rbegin();
        }

        auto rend(){
            return data_.rend();
        }

        auto crbegin(){
            return data_.crbegin();
        }

        auto crend(){
            return data_.crend();
        }



        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            GET/SET FUNCTIONS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Function that returns the Layout used by the Holor to store and index the data
         * 
         * \return Layout
         */
        const Layout<N>& layout() const{
            return layout_;
        }

        /*!
         * \brief Function that returns the number of elements along each of the container's dimensions
         * 
         * \return the lengths of each dimension of the Holor container 
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
        T* data(){
            return data_.data();
        }
        
        /*!
         * \brief Function that provides a flat access to the data contained in the container
         * 
         * \return a const pointer to the data stored in the container
         */
        const T* data() const{
            return data_.data();
        }

        /*
         * \brief Function that returns copy of the container's data vector
         * 
         * \return a const vector of data
         */
        auto data_vector() const{
            return data_;
        }


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            ACCESS FUNCTIONS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Access tensor element subscripting with integers without range check
         * \param dims pack of indices, one per dimension of the Holor container
         * \return the value of the Holor stored at the position indexed by the indices
         */
        template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
        T& operator()(Dims... dims){
            return *(data() + layout_(dims...));
        }


        /*!
         * \brief Access tensor element subscripting with integers without range check
         * \param dims pack of indices, one per dimension of the Holor container
         * \return the value of the Holor stored at the position indexed by the indices
         */
        template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
        const T operator()(Dims... dims) const{
            return data_[layout_(dims...)];
        }


        /*!
         * \brief Access tensor slice by subscripting with integers without range check
         * \param dims pack of indices, one per dimension of the Holor container
         * \return the value of the Holor stored at the position indexed by the indices
         */
        template<typename... Args> requires (impl::range_indexing<Args...>() && (sizeof...(Args)==N) )
        auto operator()(Args&&... args) {
            //TODO: the size of the HolorRef depends on the size of the Layout. We need to extract the size of the layout
            auto sliced_layout = layout_(std::forward<Args>(args)...);
            return HolorRef<T, decltype(sliced_layout)::order>(data_.data(), sliced_layout);
        };


        /*!
         * \brief function that returns the `i-th` row of the tensor
         * \param i index of the row to be indexed
         * \return a reference container to the row 
         */
        HolorRef<T, N-1> row(size_t i){
            return HolorRef<T, N-1>(data_.data(), layout_.template slice_dimension<0>(i));
        }

        /*!
         * \brief function that returns the `i-th` row of the tensor
         * \param i index of the row to be indexed
         * \return a reference container to the row 
         */
        const HolorRef<T, N-1> row(size_t i) const{
            return HolorRef<T, N-1>(data_.data(), layout_.template slice_dimension<0>(i));
        }

        
        /*!
         * \brief function that returns the `i-th` column of the tensor
         * \param i index of the column to be indexed
         * \return a reference container to the column 
         */
        HolorRef<T, N-1> col(size_t i){
            return HolorRef<T, N-1>(data_.data(), layout_.template slice_dimension<1>(i));
        }

        /*!
         * \brief function that returns the `i-th` column of the tensor
         * \param i index of the column to be indexed
         * \return a reference container to the column 
         */
        const HolorRef<T, N-1> col(size_t i) const{
            return HolorRef<T, N-1>(data_.data(), layout_.template slice_dimension<1>(i));
        }


        /*!
         * \brief function that returns the `i-th` slice of a single dimension
         * \tparam M is the dimension to be sliced. 0 is a row, 1 is a column, ...
         * \param i index of the slice alonge the `M-th` dimension
         * \return a reference container to the slice 
         */
        template<size_t M>
        HolorRef<T, N-1> slice(size_t i){
            return HolorRef<T, N-1>(data_.data(), layout_.template slice_dimension<M>(i));
        }

        /*!
         * \brief function that returns the `i-th` slice of a single dimension
         * \tparam M is the dimension to be sliced. 0 is a row, 1 is a column, ...
         * \param i index of the slice alonge the `M-th` dimension
         * \return a reference container to the slice 
         */
        template<size_t M>
        const HolorRef<T, N-1> slice(size_t i) const{
            return HolorRef<T, N-1>(data_.data(), layout_.template slice_dimension<M>(i));
        }


    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        PRIVATE MEMBERS AND FUNCTIONS
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    private:
        Layout<N> layout_; ///\brief The Layout of how the elements of the container are stored in memory
        std::vector<T> data_; ///! \brief Vector storing the actual data


        /*!
         * \brief helper function that copies the valid data from a HolorRef to a Holor
         */
        template<typename U, size_t M>
        void push_ref_elements( const U* data_ptr, const Layout<M>& ref_layout){
            for (auto i = 0; i<ref_layout.lengths()[0]; i++){
                push_ref_elements(data_ptr, ref_layout.slice_dimension<0>(i));
            }
        }


        /*!
         * \brief helper function that copies the valid data from a HolorRef to a Holor
         */
        template<typename U>
        void push_ref_elements( const U* data_ptr, const Layout<1>& ref_layout){
            for (auto i = 0; i<ref_layout.lengths()[0]; i++){
                data_.push_back(*(data_ptr + ref_layout(i)));
            }
        }

};


} //namespace holor

#endif // HOLOR_H
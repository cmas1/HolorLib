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

#include <cstddef>
#include <vector>
#include <type_traits>

// #include "holor_ref.h"
#include "initializer.h"



namespace holor{


/*================================================================================================
                                    Holor Class
================================================================================================*/
/// Holor class
/*!
 * Class providing a dense implementation of a general n-dimensional tensor container.
 * Since the purpose of this class is to provide a container object, tensor arithmetic operations are not supported.
 * 
 * It uses a row-major representation, i.e. the elements of rows are contiguous and the elements of a columne are separated by a fixed number of elements (a stride)
 * 
 * //TODO: explain better what is a tensor (number of dimensions, extensions, slicing, access)
 */
template<typename T, size_t N>
class Holor{   

    public:
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                    ALIASES
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        static constexpr size_t order = N; ///! \brief number of dimensions in the container 

        using value_type = T; ///! type of the values in the container
        using iterator = typename std::vector<T>::iterator; ///! iterator type for the underlying data storage
        using const_iterator = typename std::vector<T>::const_iterator; ///! iterator type for the underlying data storage



        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                CONSTRUCTORS, ASSIGNMENTS AND DESTRUCTOR
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Default constructor.
         * 
         * \return A Holor with zero elements on each dimension
         */
        Holor() = default;


        /*!
         * \brief Default move constructor.
         * 
         * \return A Holor equal to the argument
         */
        Holor(Holor&&) = default;


        /*!
         * \brief Default copy constructor.
         * 
         * \return A Holor equal to the argument
         */
        Holor(const Holor&) = default;


        /*!
         * \brief Default move assignement.
         * 
         * \return A Holor equal to the argument
         */
        Holor& operator=(Holor&&) = default;


        /*!
         * \brief Default copy assignement.
         * 
         * \return A Holor equal to the argument
         */
        Holor& operator=(const Holor&) = default;


        /*!
         *  \brief Default destructor.
         */
        ~Holor() = default;


        //TODO:  Do we need a constructor from lengths, perhaps with default initialization? Can we implement it using ranges?
        /*!
         * \brief Constructor that creates a Holor by specifying the length of each dimension
         * \param lengths array of `N` lengths
         * \return a Holor
         */
        Holor(const std::array<size_t,N>& lengths): layout_{lengths...}{
            data_.reserve(layout_.size_);
        }

        /*!
         * \brief Constructor that creates a Holor by specifying the length of each dimension
         * \param lengths array of `N` lengths
         * \return a Holor
         */
        Holor(std::array<size_t,N>&& lengths): layout_{lengths...}{
            data_.reserve(layout_.size_);
        }


        /*!
         * \brief Constructor from a HolorRef object
         * \param ref a HolorRef object
         * \return a Holor
         */
        template<typename U> requires (std::convertible_to<U, T>)
        Holor(const HolorRef<U,N>& ref) {
            layout_.lengths_ = ref.layout_.lengths_;
            layout_.offset_ = 0;
            layout_.compute_strides();
            data_.reserve(layout_.size());
            this->push_ref_elements(x.dataptr_, x.layout_);
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
            layout_.set_offset(0);
            layout_.set_lengths(impl::derive_lengths<N>(init));
            data_.reserve(layout_.size());
            impl::insert_flat(init, data_);
            // TODO: dynamic check that the  number of elements in the container matches the extents?
        }


        /*!
         * \brief Assign from a nested list of elements
         * \param init nested list of the elements to be inserted in the container
         * \return a reference to Holor
         */
        Holor& operator=(Holor_initializer<T,N> init){
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
        auto operator()(Args... args) {
            //TODO: the size of the HolorRef depends on the size of the Layout. We need to extract the size of the layout
            auto sliced_layout = layout_(args...);
            return HolorRef<decltype(sliced_layout)::order>(data_.data(), sliced_layout);
        };


        /*!
         * \brief function that returns the `i-th` row of the tensor
         * \param i index of the row to be indexed
         * \return a reference container to the row 
         */
        HolorRef<T, N-1> row(size_t i){
            return HolorRef(data_.data(), layout_.slice_dimension<0>());
        }

        
        /*!
         * \brief function that returns the `i-th` column of the tensor
         * \param i index of the column to be indexed
         * \return a reference container to the column 
         */
        HolorRef<T, N-1> col(size_t i){
            return HolorRef(data_.data(), layout_.slice_dimension<1>());
        }


        /*!
         * \brief function that returns the `i-th` slice of a single dimension
         * \tparam M is the dimension to be sliced. 0 is a row, 1 is a column, ...
         * \param i index of the slice alonge the `M-th` dimension
         * \return a reference container to the slice 
         */
        template<size_t M>
        HolorRef<T, N-1> slice(size_t i){
            return HolorRef(data_.data(), layout_.slice_dimension<M>());
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
            for (auto i = 0; i<ref_layout.lengths_[0]; i++){
                push_ref_elements(data_ptr, ref_layout.slice_dimension<0>(i));
            }
        }


        /*!
         * \brief helper function that copies the valid data from a HolorRef to a Holor
         */
        template<typename U>
        void push_ref_elements( const U* data_ptr, const Layout<1>& ref_layout){
            for (auto i = 0; i<ref_layout.lengths_[0]; i++){
                data_.push_back(*(data_ptr + ref_layout(i)));
            }
        }

};


} //namespace holor

#endif // HOLOR_H
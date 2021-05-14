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

#include "layout.h"
#include "initializer.h"



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
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                    ALIASES
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        static constexpr size_t dimensions = N; ///! \brief number of dimensions in the container 

        using value_type = T; ///! type of the values in the container
        using iterator = typename std::vector<T>::iterator; ///! iterator type for the underlying data storage
        using const_iterator = typename std::vector<T>::const_iterator; ///! iterator type for the underlying data storage



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
        HolorRef(T* dataptr, Layout<N> layout): layout_{layout}, dataptr_{dataptr}{}


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
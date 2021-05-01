// This file is part of Holor, a C++ template library for multi-dimensional containers

// Copyright 2020 Carlo Masone

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

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
// #include <type_traits>

// #include "tensor_initializer.h"
#include "layout.h"
// #include "tensor_predicates.h"
// #include "tensor_utils.h"



namespace holor{

/// HolorRef class
/*!
 * Class providing a dense implementation of a general n-dimensional tensor container that does not own its elements.
 * Whereas a Holor owns the memory that stores its elements, a Holor_ref does not. A Holor_ref provides a tensor-like view to a contiguous memory that stores some elements, but it does not own the memory.
 * 
 * It uses a row-major representation, i.e. the elements of rows are contiguous and the elements of a column are separated by a fixed number of elements (a stride)
 * 
 * //TODO: explain better what is a tensor (number of dimensions, extensions, slicing, access)
 */
template<typename T, size_t N>
class HolorRef{
    public:
        /****************************************************************
                                ALIASES
        ****************************************************************/
        static constexpr size_t order = N; /*! number of dimensions of the tensor view on the data*/
        using value_type = T; /*! type of the data on which create a tensor view*/


        /****************************************************************
                CONSTRUCTORS, ASSIGNEMENTS AND DESTRUCTOR
        ****************************************************************/
        /*! 
         * \brief HolorRef constructor.
         * 
         * \param layout Layout describing how the data is stored in memory
         * \param ptr pointer to the stored elements
         * 
         * \return A Tensor whose elements have the default value for their type.
         */
        HolorRef(const Layout<N>& layout, T* ptr): layout_{layout}, dataptr_{ptr} {};

      

        // /****************************************************************
        //                     GET AND SET
        // ****************************************************************/
        
        /*
        * Function that provides a pointer to the location of the first stored element
        * 
        * \return a pointer to the first stored element
        */
        T* begin(){
            return dataptr_;
        }


        /*
        * Function that provides a pointer to the location of the first stored element
        * 
        * \return a pointer to the first stored element
        */
        const T* begin() const{
            return dataptr_;
        }

        
        /*
        * Function that provides a pointer to the location after the last stored element
        * 
        * \return a pointer to the location after the last stored element
        */
        T* end(){
            return dataptr_ + layout_.size_;
        }


        /*
        * Function that provides a pointer to the location after the last stored element
        * 
        * \return a pointer to the location after the last stored element
        */
        const T* end() const{
            return dataptr_ + layout_.size_;
        }

        // /*
        // * Function that returns the number of elements along the \p n dimension
        // * 
        // * \param n dimension of the tensor along which count the number of elements
        // * 
        // * \return the number of elements along the \p n-th dimension
        // */
        // size_t length(size_t n) const{
        //     return layout_.lenghts_[n];
        // }


        // /*
        // * Function that returns the total number of elements in the tensor
        // * 
        // * \return the total number of elements in the tensor
        // */
        // size_t size() const{
        //     return layout_.size_;
        // }


        // /*
        // * Function that returns the TensorLayout defining the extensions of the Tensor in its \p N dimensions
        // * 
        // * \return TensorLayout defining subscripting
        // */
        // const TensorLayout<N>& descriptor() const{
        //     return layout_;
        // }


        // /*
        // * Function that provides a flat access to the data contained in the tensor
        // * 
        // * \return a pointer to the data stored in the tensor
        // */
        // T* data(){
        //     return data_.data();
        // }

        
        // /* 
        // * \return a const pointer to the data stored in the tensor
        // */
        // const T* data() const{
        //     return data_.data();
        // }


        /****************************************************************
                                    ACCESS
        ****************************************************************/
        /*
        * Access tensor element subscripting with integers without range check
        */
        template<typename... Args>
        std::enable_if_t<impl::requesting_element<Args...>(), T&> operator()(Args... args){
            return *(dataptr_ + layout_(args...));
        }


        //TODO: implement const version
        /*
        * ccess tensor element subscripting with integers without range check
        */
        template<typename... Args>
        std::enable_if_t<impl::requesting_element<Args...>(), const T> operator()(Args... args) const{
            return dataptr_[layout_(args...)];
        }


        // /*
        // * Access tensor element subscripting with integers with range check
        // */
        // template<typename... Args>
        // std::enable_if_t<impl::requesting_element<Args...>(), T&> at(Args... args){
        //     dynamic_assert<assertion_level(AssertionLevel::release), bst::exception::BstRuntimeError>( tensor_impl::check_bounds(layout_, args...), \
        //         EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
        //     return *(dataptr_ + layout_(args...));
        // }


        /****************************************************************
                                OPERATORS
        ****************************************************************/

    private:
        Layout<N> layout_; /*! Layout of the tensor representation of the data */
        T* dataptr_; /*! Pointer to the data */

};

} //namespace holor

#endif // HOLOR_REF_H
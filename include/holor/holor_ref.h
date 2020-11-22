//  ____   _____   _____   __       __   __   __   ______       _______   _____   _____   __       _____ 
//  |   \ |   __| |  _  \ |  |     |  | |  \ |  | |  ____|     |__   __| |  _  | |  _  | |  |     |   __|
//  ____/  \  \   | |_| | |  |     |  | |       | |  |__          | |    | | | | | | | | |  |      \  \  
//  |   \  _\  \  |  ___/ |  |___  |  | |  |\   | |  |___         | |    | |_| | | |_| | |  |___   _\  \ 
//  ____/ |_____| |__|    |______| |__| |__| \__| |______|        |_|    |_____| |_____| |______| |_____|

// This file is part of BSTools, a lightweight C++ template library that implements B-Splines

// Copyright 2019 Carlo Masone

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.



#ifndef HOLOR_REF_H
#define HOLOR_REF_H

#include <cstddef>
// #include <type_traits>

// #include "tensor_initializer.h"
#include "slice.h"
// #include "tensor_predicates.h"
// #include "tensor_utils.h"



namespace holor{

/// HolorRef class
/*!
 * Class providing a dense implementation of a reference general n-dimensional tensor container.
 * Unlike a Tensor object, a HolorRef object does not own its elements. In practice, whereas a Tensor has a vector containing its elements,
 * a HolorRef contains a pointer to the elements of a Tensor objects.
 * The application of HolorRef is to represent subtensors of a Tensor.
 */
template<typename T, size_t N>
class HolorRef{
    public:
        /****************************************************************
                                ALIASES
        ****************************************************************/
        // number of dimensions
        static constexpr size_t order = N;
        using value_type = T;


        /****************************************************************
                CONSTRUCTORS, ASSIGNEMENTS AND DESTRUCTOR
        ****************************************************************/
        /*!
        *   HolorRef constructor.
        * 
        * \param slice Tensorslice describing how the data is stored in memory
        * \param ptr pointer to the stored elements
        * 
        * \return A Tensor whose elements have the default value for their type.
        */
        HolorRef(const Slice<N>& slice, T* ptr): layout_{slice}, dataptr_{ptr} {};

      

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
        // * Function that returns the TensorSlice defining the extensions of the Tensor in its \p N dimensions
        // * 
        // * \return TensorSlice defining subscripting
        // */
        // const TensorSlice<N>& descriptor() const{
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


        // TODO: these should be private
        /*
        * Slice that describes the memory layout of the Tensor, i.e. the number of elements along each dimension,
        * the total number of elements, the offset and the strides for iterating through the vector containing the actual data
        */
        Slice<N> layout_;


        /*
        * Pointer to the data
        */
        T* dataptr_;

    private:
        // template<typename X, size_t NN>
        // friend std::ostream& operator<<(std::ostream& os, const HolorRef<X,NN>& t);

        // template<typename X, size_t NN>
        // friend std::ostream& print(std::ostream& os, const HolorRef<X,NN>& t);
};





/****************************************************************
                        OPERATORS
****************************************************************/
// template<typename T, size_t N>
// std::ostream& print(std::ostream& os, const T* ptr, const TensorSlice<N>& ts){
//     os << "[";
//     for (auto i = 0; i<ts.lengths_[0]-1; i++){
//         auto tmp = tensor_impl::slice_dim<0>();
//         TensorSlice<N-1> row = tmp(i, ts);
//         print(os, ptr, row );
//         os << ", ";
//     }
//     auto tmp = tensor_impl::slice_dim<0>();
//     TensorSlice<N-1> row = tmp(ts.lengths_[0]-1, ts);
//     print(os, ptr, row );
//     os << "]";
//     return os;
// }


// template<typename T>
// std::ostream& print(std::ostream& os, const T* ptr, const TensorSlice<1>& ts){
//     os << "[";
//     for (auto i = 0; i<ts.lengths_[0]-1; i++){
//         os << *(ptr + ts(i)) << ", ";
//     }
//     os << *(ptr + ts(ts.lengths_[0]-1) ) ;
//     os << "]";
//     return os;
// }



// template<typename T, size_t N>
// std::ostream& operator<<(std::ostream& os, const HolorRef<T,N>& t){
//     // static_assert(is_printable_v<T>, "operator<<: element of the tensor are not printable.");
//     return print(os, t.dataptr_, t.layout_);
// }


} //namespace bst

#endif // HOLOR_REF_H
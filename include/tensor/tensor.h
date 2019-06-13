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



#ifndef TENSOR_H
#define TENSOR_H

#include <cstddef>
#include <vector>
#include <type_traits>

#include "tensor_initializer.h"
#include "tensor_ref.h"
#include "tensor_slice.h"
#include "tensor_predicates.h"
#include "tensor_utils.h"
#include "../utils/dynamic_assert.h"
#include "../utils/static_assert.h"



namespace bst{

/// Tensor class
/*!
 * Class providing a dense implementation of a general n-dimensional tensor container.
 * Since the purpose of this class is to provide a container object, tensor arithmetic operations are not supported.
 * 
 * It uses a row-major representation, i.e. the elements of rows are contiguous and the elements of a columne are separated by a fixed number of elements (a stride)
 * 
 * //TODO: explain better what is a tensor (number of dimensions, extensions, slicing, access)
 */
template<typename T, size_t N>
class Tensor{
    private:
        template<typename U, size_t M>
        void push_element( const U* ptr, const TensorSlice<M>& ts){
            for (auto i = 0; i<ts.lengths_[0]; i++){
                auto tmp = tensor_impl::slice_dim<0>();
                TensorSlice<N-1> row = tmp(i, ts);
                push_element(ptr, row);
            }
        }

        template<typename U>
        void push_element( const U* ptr, const TensorSlice<1>& ts){
            for (auto i = 0; i<ts.lengths_[0]; i++){
                data_.push_back(*(ptr + ts(i)));
            }
        }

    public:
        /****************************************************************
                                ALIASES
        ****************************************************************/
        // number of dimensions
        static constexpr size_t order = N;

        using value_type = T;
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;


        /****************************************************************
                CONSTRUCTORS, ASSIGNEMENTS AND DESTRUCTOR
        ****************************************************************/
        /*!
        *   Default constructor.
        * 
        * \return A Tensor whose elements have the default value for their type.
        */
        Tensor() = default;


        /*!
        *   Default move constructor.
        * 
        * \return A Tensor
        */
        Tensor(Tensor&&) = default;


        /*!
        *   Default move assignement.
        * 
        * \return A Tensor
        */
        Tensor& operator=(Tensor&&) = default;


        /*!
        *   Default copy constructor.
        * 
        * \return A Tensor
        */
        Tensor(const Tensor&) = default;


        /*!
        *   Default copy assignement.
        * 
        * \return A Tensor
        */
        Tensor& operator=(const Tensor&) = default;


        /*!
        *   Default destructor.
        */
        ~Tensor() = default;


        /*!
        * Constructor from a TensorRef object
        *
        * /return a Tensor
        */
        // template<typename U>
        // Tensor(const TensorRef<U,N>& x): layout_{x.layout_}, data_{x.begin(), x.end()} {
        //     static_assert(std::is_convertible<U,T>(), "Tensor constructor: incompatible element types");
        // }
        template<typename U>
        Tensor(const TensorRef<U,N>& x) {
            static_assert(std::is_convertible<U,T>(), "Tensor constructor: incompatible element types");
            layout_.lengths_ = x.layout_.lengths_;
            layout_.offset_ = 0;
            layout_.compute_strides();
            data_.reserve(x.layout_.size());
            this->push_element(x.dataptr_, x.layout_);
        }


        /*!
        * Assignment operator from a TensorRef object
        * 
        * /return a reference to Tensor
        */
        template<typename U>
        Tensor& operator=(const TensorRef<U,N>& x){
            static_assert(std::is_convertible<U,T>(), "Tensor constructor: incompatible element types");
            layout_.lengths_ = x.layout_.lengths_;
            layout_.offset_ = 0;
            layout_.compute_strides();
            data_.reserve(x.layout_.size());
            this->push_element(x.dataptr_, x.layout_);
            return *this;
        }


        /*!
        * Constructor that creates a Tensor by specifying the length of each dimension of the Tensor
        * 
        * /return a Tensor
        */
        template<typename... Lengths>
        Tensor(Lengths... lengths): layout_{lengths...}, data_(layout_.size_) {}

        
        /*
        * Constructor from a list
        *
        * /return a Tensor
        */
        Tensor(initializer_tensor<T,N> init){
            layout_.offset_ = 0;
            layout_.lengths_ = tensor_impl::derive_lengths<N>(init);
            layout_.compute_strides();
            data_.reserve(layout_.size_);
            tensor_impl::insert_flat(init, data_);
            dynamic_assert<assertion_level(AssertionLevel::release), bst::exception::BstRuntimeError>( data_.size() == layout_.size(), \
                EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
        }


        // /*
        // * Assign from a list
        // *
        // * /return a reference to Tensor
        // */
        // Tensor& operator=(Tensor_initializer<T,N> init){
        //     // TODO: to implement
        //     return *this;
        // }


        /*
        * Remove constructor from a list
        */
        template<typename U>
        Tensor(std::initializer_list<U>) = delete;

        /*
        * Remove assignment from a list
        */
        template<typename U>
        Tensor& operator=(std::initializer_list<U>) = delete;

        

        // /****************************************************************
        //                     GET AND SET
        // ****************************************************************/
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


        /*
        * Function that provides a flat access to the data contained in the tensor
        * 
        * \return a pointer to the data stored in the tensor
        */
        T* data(){
            return data_.data();
        }

        
        /* 
        * \return a const pointer to the data stored in the tensor
        */
        const T* data() const{
            return data_.data();
        }


        /****************************************************************
                                    ACCESS
        ****************************************************************/
        /*
        * Access tensor element subscripting with integers without range check
        */
        template<typename... Args>
        std::enable_if_t<tensor_impl::requesting_element<Args...>(), T&> operator()(Args... args){
            return *(data() + layout_(args...));
        }


        //TODO: implement const version
        // /*
        // * Access tensor element subscripting with integers without range check
        // */
        // template<typename... Args>
        // std::enable_if_t<tensor_impl::requesting_element<Args...>(), const T> operator()(Args... args) const{
        //     std::cout<<"pippo ";
        //     return data_[layout_(args...)];
        // }


        /*
        * Access tensor element subscripting with integers with range check
        */
        template<typename... Args>
        std::enable_if_t<tensor_impl::requesting_element<Args...>(), T&> at(Args... args){
            dynamic_assert<assertion_level(AssertionLevel::release), bst::exception::BstRuntimeError>( tensor_impl::check_bounds(layout_, args...), \
                EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
            return *(data() + layout_(args...));
        }


        // TODO implement const version
        // /*
        // * Access tensor element subscripting with integers with range check
        // */
        // template<typename... Args>
        // std::enable_if_t<tensor_impl::requesting_element<Args...>(), T&> at(Args... args){
        //     dynamic_assert<assertion_level(AssertionLevel::release), bst::exception::BstRuntimeError>( tensor_impl::check_bounds(layout_, args...), \
        //         EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
        //     return *(data() + layout_(args...));
        // }

        
        /*
        * 
        */
        //TODO it cannot work like this, the return type cannot be deduced dynamically
        template<typename... Args>
        auto operator()(Args... args) -> enable_if_t<tensor_impl::requesting_slice<Args...>(), int> {
            const size_t n_slices = count_slices(args...);
            std::cout << n_slices <<  "    SIIIIIIIIIIIIIIIII\n";
            // TensorRef<T,n_slices> res;
            return 1;
        };

        
        // /*
        // * 
        // */
        // template<typename... Args>
        // enable_if_t<tensor_impl::Requesting_slice<Args...>(), TensorRef<const T,N>> operator()(const Args&... args) const;

        
        /*
        * Access \f$i$-th\f row
        * 
        * \param index of the row to be accessed
        * 
        * \return the \a i-th row of the tensor
        */
        TensorRef<T, N-1> operator[](size_t i){
            return row(i);
        }


        /*
        * Access \f$i$-th\f row
        * 
        * \param index of the row to be accessed
        * 
        * \return the \a i-th row of the tensor
        */
        TensorRef<const T, N-1> operator[](size_t i) const{
            return row(i);
        }


        /*
        *
        */
        TensorRef<T, N-1> row(size_t n){
            // dynamic_assert<assertion_level(AssertionLevel::internal), bst::exception::BstInvalidArgument>(n<rows(), \
            //     EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
            auto myslice = tensor_impl::slice_dim<0>();
            TensorSlice<N-1> row = myslice(n, layout_);
            return {row, data()};
        }


        /*
        *
        */
        TensorRef<const T, N-1> row(size_t n) const{
            // dynamic_assert<assertion_level(AssertionLevel::internal), bst::exception::BstInvalidArgument>(n<rows(), \
            //     EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
            auto myslice = tensor_impl::slice_dim<0>();
            TensorSlice<N-1> row = myslice(n, layout_);
            return {row, data()};
        }


        /*
        *
        */
        TensorRef<T, N-1> col(size_t n){
            // dynamic_assert<assertion_level(AssertionLevel::internal), bst::exception::BstInvalidArgument>(n<rows(), \
            //     EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
            auto myslice = tensor_impl::slice_dim<1>();
            TensorSlice<N-1> col = myslice(n, layout_);
            return {col, data()};
        }


        /*
        *
        */
        TensorRef<const T, N-1> col(size_t n) const{
            // dynamic_assert<assertion_level(AssertionLevel::internal), bst::exception::BstInvalidArgument>(n<rows(), \
            //     EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
            auto myslice = tensor_impl::slice_dim<1>();
            TensorSlice<N-1> col = myslice(n, layout_);
            return {col, data()};
        }


        // TODO add a general function, like row and col, to get the n-th dimension slice
        template<size_t M>
        TensorRef<const T, N-1> dim_slice(size_t n) const{
            // dynamic_assert<assertion_level(AssertionLevel::internal), bst::exception::BstInvalidArgument>(n<rows(), \
            //     EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
            auto myslice = tensor_impl::slice_dim<M>();
            TensorSlice<N-1> slice = myslice(n, layout_);
            return {slice, data()};
        }


        /****************************************************************
                                OPERATORS
        ****************************************************************/
        void print_details(std::string title = ""){
            std::cout<< "\n";
            std::cout << "=========================================\n";
            std::cout <<"| " << title << std::endl;
            std::cout << "=========================================\n";
            std::cout << "| LAYOUT:\n";
            std::cout << "|      size = " << layout_.size_ << std::endl;
            std::cout << "|      offset = " << layout_.offset_ << std::endl;
            std::cout << "|      lengths = [ ";
            for(auto x:layout_.lengths_){
                std::cout << x << " ";
            }
            std::cout <<"]\n";
            std::cout << "| DATA:\n";
            std::cout << "|      [ ";
            for(auto x:data_){
                std::cout << x << " ";
            }
            std::cout <<"]\n";
            std::cout << "=========================================\n\n";
        }

        template<typename X, size_t NN>
        friend std::ostream& operator<<(std::ostream& os, const Tensor<X,NN>& t);

    private:
        /*
        * Slice that describes the memory layout of the tensor, i.e. the number of elements along each dimension,
        * the total number of elements, the offset and the strides for iterating through the vector containing the actual data
        */
        TensorSlice<N> layout_;


        /*
        * Vector storing the actual data
        */
        std::vector<T> data_;
};




/****************************************************************
                        OPERATORS
****************************************************************/
template<typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const Tensor<T,N>& t){
    // static_assert(is_printable_v<T>, "operator<<: element of the tensor are not printable.");
    return print(os, t.data(), t.layout_);
}


} //namespace bst

#endif // TENSOR_H
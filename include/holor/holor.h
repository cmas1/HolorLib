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

// #include "tensor_predicates.h"
// #include "tensor_utils.h"
// #include "../common/static_assert.h"



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
         * \brief Constructor that creates a Holor by specifying the length of each dimension of the Holor
         * 
         * \return a Holor
         */
        // template<typename... Lengths>
        // Holor(Lengths... lengths): layout_{lengths...}, data_(layout_.size_) {}


        //TODO: Constructor from HolorRef
        // /*!
        // * Constructor from a Slice object
        // *
        // * /return a Holor
        // */
        // // template<typename U>
        // // Holor(const Slice<U,N>& x): layout_{x.layout_}, data_{x.begin(), x.end()} {
        // //     static_assert(std::is_convertible<U,T>(), "Holor constructor: incompatible element types");
        // // }
        // template<typename U>
        // Holor(const Slice<U,N>& x) {
        //     static_assert(std::is_convertible<U,T>(), "Holor constructor: incompatible element types");
        //     layout_.lengths_ = x.layout_.lengths_;
        //     layout_.offset_ = 0;
        //     layout_.compute_strides();
        //     data_.reserve(x.layout_.size());
        //     this->push_element(x.dataptr_, x.layout_);
        // }


        //TODO: assignment from HolorRef
        // /*!
        // * Assignment operator from a Slice object
        // * 
        // * /return a reference to Holor
        // */
        // template<typename U>
        // Holor& operator=(const Slice<U,N>& x){
        //     static_assert(std::is_convertible<U,T>(), "Holor constructor: incompatible element types");
        //     layout_.lengths_ = x.layout_.lengths_;
        //     layout_.offset_ = 0;
        //     layout_.compute_strides();
        //     data_.reserve(x.layout_.size());
        //     this->push_element(x.dataptr_, x.layout_);
        //     return *this;
        // }

        
        /*!
         * \brief Constructor from a nested list of elements
         * 
         * \param init nested list of the elements to be inserted in the container
         *
         * \return a Holor containing the elements in the list
         */
        Holor(holor::nested_list<T,N> init){
            layout_.set_offset(0);
            layout_.set_lengths(impl::derive_lengths<N>(init));
            data_.reserve(layout_.size());
            impl::insert_flat(init, data_);
            // TODO: dynamic check that the  number of elements in the container matches the extents?
        }


        // /*
        // * Assign from a list
        // *
        // * /return a reference to Holor
        // */
        // Holor& operator=(Holor_initializer<T,N> init){
        //     // TODO: to implement
        //     return *this;
        // }


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
        const Layout<N>& descriptor() const{
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
        /*
        * Access tensor element subscripting with integers without range check
        */
        template<typename... Args>
        std::enable_if_t<impl::requesting_element<Args...>(), T&> operator()(Args... args){
            return *(data() + layout_(args...));
        }


        //TODO: implement const version
        // /*
        // * Access tensor element subscripting with integers without range check
        // */
        template<typename... Args>
        std::enable_if_t<impl::requesting_element<Args...>(), const T> operator()(Args... args) const{
            return data_[layout_(args...)];
        }


        /*
        * Access tensor element subscripting with integers with range check
        */
        // template<typename... Args>
        // std::enable_if_t<holor_impl::requesting_element<Args...>(), T&> at(Args... args){
        //     dynamic_assert<assertion_level(AssertionLevel::release), holor::exception::BstRuntimeError>( holor_impl::check_bounds(layout_, args...), \
        //         EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
        //     return *(data() + layout_(args...));
        // }


        // TODO implement const version
        // /*
        // * Access tensor element subscripting with integers with range check
        // */
        // template<typename... Args>
        // std::enable_if_t<holor_impl::requesting_element<Args...>(), const T> at(Args... args){
        //     dynamic_assert<assertion_level(AssertionLevel::release), holor::exception::BstRuntimeError>( holor_impl::check_bounds(layout_, args...), \
        //         EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
        //     return data_[layout_(args...)];
        // }

        
        /*
        * 
        */
        template<typename... Args>
        auto operator()(Args... args) -> std::enable_if_t<holor_impl::requesting_slice<Args...>(), int> {
            // Slice<T,n_slices> res;
        };






        
        // /*
        // * 
        // */
        // template<typename... Args>
        // enable_if_t<holor_impl::Requesting_slice<Args...>(), Slice<const T,N>> operator()(const Args&... args) const;

        
        /*
        * Access \f$i$-th\f row
        * 
        * \param index of the row to be accessed
        * 
        * \return the \a i-th row of the tensor
        */
        // Slice<T, N-1> operator[](size_t i){
        //     return row(i);
        // }


        /*
        * Access \f$i$-th\f row
        * 
        * \param index of the row to be accessed
        * 
        * \return the \a i-th row of the tensor
        */
        // Slice<const T, N-1> operator[](size_t i) const{
        //     return row(i);
        // }


        /*
        *
        */
        // Slice<T, N-1> row(size_t n){
        //     // dynamic_assert<assertion_level(AssertionLevel::internal), holor::exception::BstInvalidArgument>(n<rows(), \
        //     //     EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
        //     auto myslice = holor_impl::slice_dim<0>();
        //     HolorLayout<N-1> row = myslice(n, layout_);
        //     return {row, data()};
        // }


        /*
        *
        */
        // Slice<const T, N-1> row(size_t n) const{
        //     // dynamic_assert<assertion_level(AssertionLevel::internal), holor::exception::BstInvalidArgument>(n<rows(), \
        //     //     EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
        //     auto myslice = holor_impl::slice_dim<0>();
        //     HolorLayout<N-1> row = myslice(n, layout_);
        //     return {row, data()};
        // }


        /*
        *
        */
        // Slice<T, N-1> col(size_t n){
        //     // dynamic_assert<assertion_level(AssertionLevel::internal), holor::exception::BstInvalidArgument>(n<rows(), \
        //     //     EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
        //     auto myslice = holor_impl::slice_dim<1>();
        //     HolorLayout<N-1> col = myslice(n, layout_);
        //     return {col, data()};
        // }


        /*
        *
        */
        // Slice<const T, N-1> col(size_t n) const{
        //     // dynamic_assert<assertion_level(AssertionLevel::internal), holor::exception::BstInvalidArgument>(n<rows(), \
        //     //     EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
        //     auto myslice = holor_impl::slice_dim<1>();
        //     HolorLayout<N-1> col = myslice(n, layout_);
        //     return {col, data()};
        // }


        // // TODO add a general function, like row and col, to get the n-th dimension slice
        // template<size_t M>
        // Slice<const T, N-1> dim_slice(size_t n) const{
        //     // dynamic_assert<assertion_level(AssertionLevel::internal), holor::exception::BstInvalidArgument>(n<rows(), \
        //     //     EXCEPTION_MESSAGE("The number of elements in the tensor does not match the extents of the tensor") );
        //     auto myslice = holor_impl::slice_dim<M>();
        //     HolorLayout<N-1> slice = myslice(n, layout_);
        //     return {slice, data()};
        // }


        /****************************************************************
                                OPERATORS
        ****************************************************************/
        // void print_details(std::string title = ""){
        //     std::cout<< "\n";
        //     std::cout << "=========================================\n";
        //     std::cout <<"| " << title << std::endl;
        //     std::cout << "=========================================\n";
        //     std::cout << "| LAYOUT:\n";
        //     std::cout << "|      size = " << layout_.size_ << std::endl;
        //     std::cout << "|      offset = " << layout_.offset_ << std::endl;
        //     std::cout << "|      lengths = [ ";
        //     for(auto x:layout_.lengths_){
        //         std::cout << x << " ";
        //     }
        //     std::cout <<"]\n";
        //     std::cout << "| DATA:\n";
        //     std::cout << "|      [ ";
        //     for(auto x:data_){
        //         std::cout << x << " ";
        //     }
        //     std::cout <<"]\n";
        //     std::cout << "=========================================\n\n";
        // }

        // template<typename X, size_t NN>
        // friend std::ostream& operator<<(std::ostream& os, const Holor<X,NN>& t);


    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        PRIVATE MEMBERS AND FUNCTIONS
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    private:
        /*
         * \brief Layout that describes the memory layout of the container, i.e. the number of elements along each dimension,
         * the total number of elements, the offset and the strides for iterating through the vector containing the actual data
         */
        Layout<N> layout_;


        /*
         * \brief Vector storing the actual data
         */
        std::vector<T> data_;


        // template<typename U, size_t M>
        // void push_element( const U* ptr, const HolorLayout<M>& ts){
        //     for (auto i = 0; i<ts.lengths_[0]; i++){
        //         auto tmp = holor_impl::slice_dim<0>();
        //         HolorLayout<N-1> row = tmp(i, ts);
        //         push_element(ptr, row);
        //     }
        // }


        // template<typename U>
        // void push_element( const U* ptr, const HolorLayout<1>& ts){
        //     for (auto i = 0; i<ts.lengths_[0]; i++){
        //         data_.push_back(*(ptr + ts(i)));
        //     }
        // }
};




/*================================================================================================
                            Holor Out-of-Class Functions
================================================================================================*/
// template<typename T, size_t N>
// std::ostream& operator<<(std::ostream& os, const Holor<T,N>& t){
//     // static_assert(is_printable_v<T>, "operator<<: element of the tensor are not printable.");
//     return print(os, t.data(), t.layout_);
// }



} //namespace holor

#endif // HOLOR_H
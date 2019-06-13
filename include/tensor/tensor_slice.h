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

//TODO add or modify the constructors according to how slices will be used in Tensor.h (for exampl for accessing elements)


#ifndef TENSOR_SLICE_H
#define TENSOR_SLICE_H

#include <cstddef>
#include <initializer_list>
#include <array>
#include <type_traits>
#include <numeric>

#include "../utils/static_assert.h"


namespace bst{


/// TensorSlice class
/*!
 *
 *  \brief  Class defining multi-dimensional subset of a \ref Tensor.
 *
 *  The TensorSlice class represents a map from a set of subscripts to a multi-dimensional subset of a \ref Tensor. 
 *  It uses the  idea of generalized slices from the standard library, i.e. it is based on the fact that the elements of a Tensor
 *  are stored as an array (or rather a std::vector) following a row-major representation.
 *  A TensorSlice is specified by three parameter sets: <em>starting offset</em>, <em>length array</em>, and <em>stride array</em>. 
 *      - The <em>starting offset</em> is the index of the first element of the original Tensor that is part of the subset.  
 *      - The <em>length array</em> contains the number of elements in that for every dimension.
 *      - The <em>stride array</em> is the distance in the array between successive elements in that dimension.
 *  For a TensorSlice with \p N dimensions, both the length array and the stride array must be size \p N.
 * 
 * Given a <tt>Tensor<T,N> t</tt> whose data is stored in the array (or vector) \p t.elements and a <tt>TensorSlice<N> s</tt> with offset
 * \p s.offset and an array with \p N strides \p s.strides, the element <tt>t.elements[i]</tt> corresponding to <tt>s[j_0, j_2, ..., j_{n-1}]</tt> is found
 * according to the formula
 * \f[
 *  i = s.offset + \sum_{j=0}^{N-1} j \cdot s.strides[j]
 * \f]
 * 
 * \verbatim embed:rst:leading-asterisk
 *  .. note::
 *      The strides depend on the lengths of the original tensor.
 * \endverbatim
 *
 * \b Example:
 * \n To illustrate how slices work, we consider a 3x3x3 tensor whose elements have the same value as their index,
 * 
 * \image html tensorslice_1.jpg
 * 
 * Let's take the  2x2x3 slice containing the elements <tt>[12, 13, 14, 15, 16, 17, 21, 22, 23, 24, 25, 26]</tt>.
 * \image html tensorslice_2.jpg
 * 
 * The \b dimensions are <tt>s.lengths = [2,2,3]</tt>.
 * - \b strides:
 *  - the first stride is the distance between two consecutive elements along the vertical dimension (rows), i.e. \p 21 and \p 12.
 *     Therefore, <tt>s.strides[0] = 9</tt>.
 *  - the second stride is the distance between two consecutive elements along the horizontal dimension (columns), i.e. \p 12 and \p 15.
 *     Therefore, <tt>s.strides[1] = 3</tt>.
 *  - the third stride is the distance between two consecutive elements along the depth dimension (depth), i.e. \p 12 and \p 13.
 *     Therefore, <tt>s.strides[2] = 1</tt>.
 * 
 * The \b offset of the slice is the number of elements preceeding the first element of the slice, i.e. <tt>s.offset = 12 </tt>.
 */
template<size_t N>
class TensorSlice{
    public:

        /****************************************************************
                CONSTRUCTORS, ASSIGNEMENTS AND DESTRUCTOR
        ****************************************************************/    
        /*!
        * Default constructor that creates an empty slice with no elements
        */
        TensorSlice() = default;

        
        /*!
        * Constructor from a TensorSlice
        * 
        * \param lengths number of elements along each dimension of the slice
        * 
        * \return a TensorSlice
        */
        TensorSlice(const TensorSlice<N>& ts) = default;

        /*!
        * Constructor from a TensorSlice
        * 
        * \param lengths number of elements along each dimension of the slice
        * 
        * \return a TensorSlice
        */
        TensorSlice<N>& operator=(const TensorSlice<N>& ts) = default;

        /*!
        * Constructor from a TensorSlice
        * 
        * \param lengths number of elements along each dimension of the slice
        * 
        * \return a TensorSlice
        */
        TensorSlice(TensorSlice<N>&& ts) = default;

        /*!
        * Constructor from a TensorSlice
        * 
        * \param lengths number of elements along each dimension of the slice
        * 
        * \return a TensorSlice
        */
        TensorSlice<N>& operator=(TensorSlice<N>&& ts) = default;

        /*!
        * Constructor of a slice from an offset and a list of lengths
        * 
        * \param offset index from where to sart the slice
        * \param lengths number of elements along each dimension of the slice
        * 
        * \return a TensorSlice
        */
        TensorSlice(size_t offset, std::initializer_list<size_t> lengths): offset_{offset}{
            std::copy(lengths.begin(),lengths.end(),lengths_.begin());
        };


        /*!
        * Constructor of a slice from an offset and an array of lengths
        * 
        * \param offset index from where to sart the slice
        * \param lengths array containing the number of elements along each dimension of the slice
        * 
        * \return a TensorSlice
        */
        TensorSlice(size_t offset, std::array<size_t,N> lengths): offset_{offset}, lengths_{lengths} {};


        /*!
        * Constructor of a slice from an offset, a list of lengths and a list of strides
        * 
        * \param offset index from where to start the slice
        * \param lengths number of elements along each dimension of the slice
        * \param strides strides of the slice
        * 
        * \return a TensorSlice
        */
        TensorSlice(size_t offset, std::initializer_list<size_t> lengths, std::initializer_list<size_t> strides): offset_{offset}{
            std::copy(lengths.begin(),lengths.end(),lengths_.begin());
            std::copy(strides.begin(),strides.end(),strides_.begin());
            size_ = std::accumulate(lengths.begin(), lengths.end(), 1, std::multiplies<size_t>());
        }


        /*!
        * Constructor of a slice from an offset, a list of lengths and a list of strides
        * 
        * \param offset index from where to start the slice
        * \param lengths number of elements along each dimension of the slice
        * \param strides strides of the slice
        * 
        * \return a TensorSlice
        */
        TensorSlice(size_t offset, std::array<size_t,N> lengths, std::array<size_t,N> strides): 
            offset_{offset}, lengths_{lengths}, strides_{strides}{
        }


        /*!
        * Constructor from a list of lengths
        * 
        * \param lengths number of elements along each dimension of the slice
        * 
        * \return a TensorSlice
        */
        template<typename... Lengths>
        TensorSlice(Lengths... lengths){
            std::array<size_t,N> tmp{size_t(lengths)...};
            lengths_ = std::move(tmp);
            offset_ = 0;
            this->compute_strides();
        }

        
        /****************************************************************
                                OPERATORS
        ****************************************************************/
        /*!
        * Given a set of subscripts referring to a TensorSlice, it returns the index of the corresponding element in the Tensor
        * 
        * \param dims parameters pack containing the subscripts
        * 
        * \return the index of the subscripted element in the Tensor
        */
        template<typename... Dims, typename = enable_if_t<assert::all(std::is_convertible_v<Dims,size_t>...)>>
        size_t operator()(Dims... dims) const{
            static_assert(sizeof...(Dims)==N, "TensorSlice<N>::operator(): dimension mismatch");
            size_t args[N]{size_t(dims)... }; //OPTIMIZE: inefficient copy of subscripts from parameters pack into an array.
            return offset_ + std::inner_product(args, args+N, strides_.begin(), size_t{0});
        }

        
        /***********************************************************************
        *                           GET FUNCTIONS
        ***********************************************************************/
        /*!
        * Get the size of the slice
        * \return the size (total number of elements) of the slice
        */
        size_t size() const{
            return size_;
        }

        /*!
        * Get the offset of the slice
        * \return the offset (with respect to the sliced Tensor) of the slice
        */
        size_t offset() const{
            return offset_;
        }

        /*!
        * Get the lengths of the slice
        * \return the lengths (number of elements per dimension) of the slice
        */
        std::array<size_t,N> lengths() const{
            return lengths_;
        }

        /*!
        * Get the strides of the slice
        * \return the strides of the slice
        */
        std::array<size_t,N> strides() const{
            return strides_;
        }



        /***********************************************************************
        *                           SET FUNCTIONS
        ***********************************************************************/
        /*!
         * Computes and sets the strides and total size of the TensorSlice given the lengths of the Tensor
         */
        void compute_strides(){
            size_ = 1;
            for(int i = N-1; i>=0; --i){
                strides_[i] = size_;
                size_ *= lengths_[i];
            }
        }

        void set_lengths(std::array<size_t,N> lengths){
            lengths_ = lengths;
        }


        //TODO: make these arguments private
        /*!
        * number of elements in each dimension
        */
        std::array<size_t,N> lengths_;

        /*!
        * total number of elements of the slice
        */
        size_t size_;

        /*!
        * offset from the beginning of the array of elements of the tensor where the slice starts
        */
        size_t offset_;

        /*!
        * distance between consecutive elements in each dimension
        */
        std::array<size_t,N> strides_;

        // // TODO: is it correct to use NN instead of N?
        // template<typename X, size_t NN>
        // friend class Tensor;
    private:
        

};



/****************************************************************
*                   SPECIALIZATIONS FOR N = 1
****************************************************************/
/*!
* Specialization of operator TensorSlice() for <tt>N=1</tt> and for \p int subscripts
* 
* \param i subscript along first dimension (rows)
* 
* \return the index of the subscripted element in the Tensor
*/
template<>
template<>
inline size_t TensorSlice<1>::operator()(int i) const{
    return offset_ + i*strides_[0];
}

/*!
* Specialization of operator TensorSlice() for <tt>N=1</tt> and for <tt>unsigned int</tt> subscripts
* 
* \param i subscript along first dimension (rows)
* 
* \return the index of the subscripted element in the Tensor
*/
template<>
template<>
inline size_t TensorSlice<1>::operator()(unsigned int i) const{
    return offset_ + i*strides_[0];
}

/*!
* Specialization of operator TensorSlice() for <tt>N=1</tt> and for \p long subscripts
* 
* \param i subscript along first dimension (rows)
* 
* \return the index of the subscripted element in the Tensor
*/
template<>
template<>
inline size_t TensorSlice<1>::operator()(long i) const{
    return offset_ + i*strides_[0];
}

/*!
* Specialization of operator TensorSlice() for <tt>N=1</tt> and for <tt>unsigned long</tt>> subscripts
* 
* \param i subscript along first dimension (rows)
* 
* \return the index of the subscripted element in the Tensor
*/
template<>
template<>
inline size_t TensorSlice<1>::operator()(unsigned long i) const{
    return offset_ + i*strides_[0];
}


/****************************************************************
*                   SPECIALIZATIONS FOR N = 2
****************************************************************/
/*!
* Specialization of operator TensorSlice() for <tt>N=2</tt> and for \p int subscripts
* 
* \param i subscript along first dimension (rows)
* \param j subscript along second dimension (columns)
* 
* \return the index of the subscripted element in the Tensor
*/
template<>
template<>
inline size_t TensorSlice<2>::operator()(int i, int j) const{
    return offset_ + i*strides_[0] + j*strides_[1];
}

/*!
* Specialization of operator TensorSlice() for <tt>N=2</tt> and for <tt>unsigned int</tt> subscripts
* 
* \param i subscript along first dimension (rows)
* \param j subscript along second dimension (columns)
* 
* \return the index of the subscripted element in the Tensor
*/
template<>
template<>
inline size_t TensorSlice<2>::operator()(unsigned int i, unsigned int j) const{
    return offset_ + i*strides_[0] + j*strides_[1];
}

/*!
* Specialization of operator TensorSlice() for <tt>N=2</tt> and for \p long subscripts
* 
* \param i subscript along first dimension (rows)
* \param j subscript along second dimension (columns)
* 
* \return the index of the subscripted element in the Tensor
*/
template<>
template<>
inline size_t TensorSlice<2>::operator()(long i, long j) const{
    return offset_ + i*strides_[0] + j*strides_[1];
}

/*!
* Specialization of operator TensorSlice() for <tt>N=2</tt> and for <tt>unsigned long</tt>> subscripts
* 
* \param i subscript along first dimension (rows)
* \param j subscript along second dimension (columns)
* 
* \return the index of the subscripted element in the Tensor
*/
template<>
template<>
inline size_t TensorSlice<2>::operator()(unsigned long i, unsigned long j) const{
    return offset_ + i*strides_[0] + j*strides_[1];
}



/****************************************************************
*                   SPECIALIZATIONS FOR N = 3
****************************************************************/
/*!
* Specialization of operator TensorSlice() for <tt>N=3</tt> and for \p int subscripts
* 
* \param i subscript along first dimension (rows)
* \param j subscript along second dimension (columns)
* \param k subscript along third dimension (depth)
* 
* \return the index of the subscripted element in the Tensor
*/
template<>
template<>
inline size_t TensorSlice<3>::operator()(int i, int j, int k) const{
    return offset_ + i*strides_[0] + j*strides_[1] + k*strides_[2];
}

/*!
* Specialization of operator TensorSlice() for <tt>N=3</tt> and for <tt>unsigned int</tt> subscripts
* 
* \param i subscript along first dimension (rows)
* \param j subscript along second dimension (columns)
* \param k subscript along third dimension (depth)
* 
* \return the index of the subscripted element in the Tensor
*/
template<>
template<>
inline size_t TensorSlice<3>::operator()(unsigned int i, unsigned int j, unsigned int k) const{
    return offset_ + i*strides_[0] + j*strides_[1] + k*strides_[2];
}

/*!
* Specialization of operator TensorSlice() for <tt>N=3</tt> and for \p long subscripts
* 
* \param i subscript along first dimension (rows)
* \param j subscript along second dimension (columns)
* \param k subscript along third dimension (depth)
* 
* \return the index of the subscripted element in the Tensor
*/
template<>
template<>
inline size_t TensorSlice<3>::operator()(long i, long j, long k) const{
    return offset_ + i*strides_[0] + j*strides_[1] + k*strides_[2];
}

/*!
* Specialization of operator TensorSlice() for <tt>N=3</tt> and for <tt>unsigned long</tt>> subscripts
* 
* \param i subscript along first dimension (rows)
* \param j subscript along second dimension (columns)
* \param k subscript along third dimension (depth)
* 
* \return the index of the subscripted element in the Tensor
*/
template<>
template<>
inline size_t TensorSlice<3>::operator()(unsigned long i, unsigned long j, unsigned long k) const{
    return offset_ + i*strides_[0] + j*strides_[1] + k*strides_[2];
}





struct slice_request{
    size_t start_;
    size_t end_;
    size_t step_;

    slice_request(size_t start, size_t end, size_t step=1): start_{start}, end_{end}, step_{step}{
        dynamic_assert<assertion_level(AssertionLevel::release), bst::exception::BstInvalidArgument>( end>start, \
                EXCEPTION_MESSAGE("slice_request: invalid argument. end<start") );
        dynamic_assert<assertion_level(AssertionLevel::release), bst::exception::BstInvalidArgument>( step>0, \
                EXCEPTION_MESSAGE("slice_request: invalid argument. step>0") );
    }
};

constexpr size_t count_slices(){
    return 0;
}

template<typename X>
constexpr size_t count_slices(X x){
    return  (std::is_same<X, slice_request>())? 1:0;
}

template<typename X, typename... Args>
constexpr size_t count_slices(X x, Args... args){
    return count_slices(x) + count_slices(args...);
}



} //namespace bst

#endif // TENSOR_SLICE_H;
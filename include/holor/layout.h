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


#ifndef HOLOR_LAYOUT_H
#define HOLOR_LAYOUT_H

#include <cstddef>
#include <array>
#include <numeric>
#include <type_traits>

#include "../utils/static_assert.h"
#include "./predicates.h"






namespace holor{


struct slice_range{
    size_t start_;
    size_t end_;
    size_t step_;

    slice_range(size_t start, size_t end, size_t step=1): start_{start}, end_{end}, step_{step}{
        // it must be end > start, start >=0, end <= length[dim]
        // check also step?
    }        
};

/* function used to verify that some of the subscripts used to access the elements of a tensor are layouts
* return \p true if some of the arguments are layouts
*/
template<typename... Args>
constexpr bool requesting_slice(){
    return assert::all((std::is_convertible<Args, size_t>() || std::is_same<Args, slice_range>() || std::is_convertible<Args, slice_range>())...) && assert::some(std::is_same<Args, slice_range>()...);
}












/*!
 * \brief Class that represents the layout of a Holor container or a Slice of a Holor, i.e., how the elements are stored in memory.
 *
 * The Layout class contains the information for indexing a multi-dimensional subset of a Holor container. 
 * It uses the  idea of generalized layouts from the standard library, i.e. it is based on the fact that the elements of a Holor
 * are stored as a 1D data sequence following a row-major representation.
 * 
 * Indexing a layout of a Holor container is done via three parameters: 
 *      - The \b offset is the index of the first element of the container that is part of the layout.  
 *      - The \b lengths are the numbers of elements for every dimension of the layout.
 *      - The \b strides are the distances in the 1D data sequence between successive elements in individual dimensions of the layout.
 *  For a Layout with `N` dimensions, both the length array and the stride array must be size `N`.
 * 
 * Given a Holor container with `N` dimensions, the elements of a layout are indexed from the underlying 1D data sequence according to the following formula
 * \f[
 *  index = offset + \sum_{j=0}^{N-1} j \cdot strides[j]
 * \f]
 * 
 * \verbatim embed:rst:leading-asterisk
 *  .. note::
 *      The strides depend on the lengths of the original tensor.
 * \endverbatim
 */


// //Forward declaration to make this a friend class of Layout
// namespace impl{
//     template<size_t M>
//     struct layout_dim;
// }





template<size_t N>
class Layout{
    
    friend class Layout<N+1>;
    public:

        /****************************************************************
                CONSTRUCTORS, ASSIGNEMENTS AND DESTRUCTOR
        ****************************************************************/    
        /*!
         * \brief Default constructor that creates an empty layout with no elements
         */
        Layout():size_{0}, offset_{0}{};


        /*!
         * \brief Copy constructor
         * 
         * \param a Layout
         * 
         * \return a copy of the input Layout
         */
        Layout(const Layout<N>& layout) = default;


        /*!
         * \brief Copy assignment
         * 
         * \param a Layout
         * 
         * \return a Layout copied from the input
         */
        Layout<N>& operator=(const Layout<N>& layout) = default;


        /*!
         * \brief Move constructor
         * 
         * \param a Layout
         * 
         * \return a Layout moved from the input
         */
        Layout(Layout<N>&& layout) = default;


        /*!
         * \brief Move assignment
         * 
         * \param a Layout
         * 
         * \return a Layout moved from the input
         */
        Layout<N>& operator=(Layout<N>&& layout) = default;


        /*!
         * \brief Constructor of a layout from an offset and an array of lengths
         * 
         * \param lengths array containing the number of elements along each dimension of the layout
         * \param offset index of the first element of the layout in the original Holor container
         * 
         * \return a Layout
         */
        Layout(const std::array<size_t,N>& lengths, size_t offset=0): offset_{offset}, lengths_{lengths} {
            compute_strides();
        };


        /*!
         * \brief Constructor of a layout from an offset and an array of lengths
         * 
         * \param lengths array containing the number of elements along each dimension of the layout
         * \param offset index of the first element of the layout in the original Holor container
         * 
         * \return a Layout
         */
        Layout(std::array<size_t,N>&& lengths, size_t offset): offset_{offset}, lengths_{lengths} {
            compute_strides();
        };


        // /*!
        //  * Constructor from a list of lengths
        //  * 
        //  * \tparam lengths number of elements along each dimension of the layout
        //  * 
        //  * \return a Layout
        //  */
        // //TODO: do we need this constructor? Needs Improvements!!! Do we need a simple constructor from an array of lengths, without offset?
        // template<typename... Lengths>
        // Layout(Lengths... lengths){
        //     static_assert(sizeof...(Lengths)==N, "Layout constructor dimension mismatch");
        //     std::array<size_t,N> tmp{size_t(lengths)...};
        //     lengths_ = std::move(tmp);
        //     offset_ = 0;
        //     this->compute_strides();
        // }


        
        /***********************************************************************
        *                           GET/SET FUNCTIONS
        ***********************************************************************/
        /*!
         * \brief Get the number of dimensions of the layout
         * 
         * \return the number `N` of dimensions of the layout.
         */
        constexpr size_t dimensions(){
            return N;
        }

        /*!
         * \brief Get the size of the layout
         * 
         * \return the size (total number of elements) of the layout
         */
        size_t size() const{
            return size_;
        }

        /*!
         * \brief Get the offset of the layout
         * 
         * \return the offset (with respect to the layoutd Holor container) of the layout
         */
        size_t offset() const{
            return offset_;
        }

        /*!
         * \brief Get the lengths of the layout
         * 
         * \return the lengths (number of elements per dimension) of the layout
         */
        std::array<size_t,N> lengths() const{
            return lengths_;
        }

        /*!
         * \brief Get the strides of the layout
         * 
         * \return the strides of the layout
         */
        std::array<size_t,N> strides() const{
            return strides_;
        }

        /*!
         * \brief Set the lengths of the layout
         * 
         * \param the lengths to be set
         */
        void set_lengths(std::array<size_t,N> lengths) {
            lengths_ = lengths;
            compute_strides();
        }

        /*!
         * \brief Set the offset of the layout
         * 
         * \param offset is the new offset of the layout
         */
        void set_offset(size_t offset) {
            offset_ = offset;
        }


        /****************************************************************
                                OPERATORS
        ****************************************************************/
        /*!
         * \brief Given a set of subscripts referring to a Layout, it returns the index of the corresponding element in the Holor
         * 
         * \param dims parameters pack containing the subscripts
         * 
         * \return the index of the subscripted element in the Holor
         */

        //TODO: rewrite this function in a better way
        template<typename... Dims>
        std::enable_if_t<impl::requesting_element<Dims...>(), size_t> index_layout(Dims... dims) const{
            static_assert(sizeof...(Dims)==N, "Layout<N>::operator(): dimension mismatch");
            size_t args[N]{size_t(dims)... }; 
            return offset_ + std::inner_product(args, args+N, strides_.begin(), size_t{0});
        }


        //TODO: change std_enable_if with requires, maybe createing a concept for the type of arguments allowed
        template<typename FirstArg, typename... OtherArgs>
        auto slice_layout(size_t dim, FirstArg first, OtherArgs... other) {
            if constexpr(std::is_same<FirstArg, slice_range>() || std::is_convertible<FirstArg, slice_range>()){
                return slice_dimension(dim, first).slice_layout(dim+1, other...);
            }else{
                return slice_dimension(dim, first).slice_layout(dim, other...);
            }
        }

        template<typename FirstArg>
        auto slice_layout(size_t dim, FirstArg first) {
            return slice_dimension(dim, first);
        }


        //slices the layout along one single dimension
        Layout<N> slice_dimension(size_t dim, slice_range range){
            Layout<N> res = *this;
            res.lengths_[dim] = range.end_-range.start_+1;
            res.size_ = std::accumulate(res.lengths_.begin(), res.lengths_.end(), 1, std::multiplies<size_t>());
            res.offset_ = offset_ + range.start_*strides_[dim];
            return res;
        }


        //TODO: now this requires that Layout<N> is friend to Layout<N+1>. This is not a clean solution
        //slices the layout along one single dimension
        Layout<N-1> slice_dimension(size_t dim, size_t num, size_t step = 1){
            //step is not used right now, should be modified in the future to use it
            Layout<N-1> res;
            size_t i = 0;
            for(size_t j = 0; j < N; j++){
                res.size_ = 1;
                if (j != dim){
                    res.lengths_[i] = lengths_[j];
                    res.strides_[i] = strides_[j];
                    res.size_ *= lengths_[j];
                    i++;
                }
                res.offset_ = offset_ + num*strides_[dim];
            }
            return res;
        }



    private:
        /*!
        * number of elements in each dimension
        */
        std::array<size_t,N> lengths_;

        /*!
        * total number of elements of the layout
        */
        size_t size_;

        /*!
        * offset from the beginning of the array of elements of the tensor where the layout starts
        */
        size_t offset_;

        /*!
        * distance between consecutive elements in each dimension
        */
        std::array<size_t,N> strides_;


        /*!
         * \brief Computes and sets the strides and total size of the Layout given the lengths of the Holor container
         */
        void compute_strides(){
            size_ = 1;
            for(int i = N-1; i>=0; --i){
                strides_[i] = size_;
                size_ *= lengths_[i];
            }
        }
};





// create degenerate case for N = 0


// /****************************************************************
// *                   SPECIALIZATIONS FOR N = 1
// ****************************************************************/
// /*!
// * Specialization of operator Layout() for <tt>N=1</tt> and for \p int subscripts
// * 
// * \param i subscript along first dimension (rows)
// * 
// * \return the index of the subscripted element in the Holor
// */
// template<>
// template<>
// inline size_t Layout<1>::operator()(int i) const{
//     return offset_ + i*strides_[0];
// }

// /*!
// * Specialization of operator Layout() for <tt>N=1</tt> and for <tt>unsigned int</tt> subscripts
// * 
// * \param i subscript along first dimension (rows)
// * 
// * \return the index of the subscripted element in the Holor
// */
// template<>
// template<>
// inline size_t Layout<1>::operator()(unsigned int i) const{
//     return offset_ + i*strides_[0];
// }

// /*!
// * Specialization of operator Layout() for <tt>N=1</tt> and for \p long subscripts
// * 
// * \param i subscript along first dimension (rows)
// * 
// * \return the index of the subscripted element in the Holor
// */
// template<>
// template<>
// inline size_t Layout<1>::operator()(long i) const{
//     return offset_ + i*strides_[0];
// }

// /*!
// * Specialization of operator Layout() for <tt>N=1</tt> and for <tt>unsigned long</tt>> subscripts
// * 
// * \param i subscript along first dimension (rows)
// * 
// * \return the index of the subscripted element in the Holor
// */
// template<>
// template<>
// inline size_t Layout<1>::operator()(unsigned long i) const{
//     return offset_ + i*strides_[0];
// }


// /****************************************************************
// *                   SPECIALIZATIONS FOR N = 2
// ****************************************************************/
// /*!
// * Specialization of operator Layout() for <tt>N=2</tt> and for \p int subscripts
// * 
// * \param i subscript along first dimension (rows)
// * \param j subscript along second dimension (columns)
// * 
// * \return the index of the subscripted element in the Holor
// */
// template<>
// template<>
// inline size_t Layout<2>::operator()(int i, int j) const{
//     return offset_ + i*strides_[0] + j*strides_[1];
// }

// /*!
// * Specialization of operator Layout() for <tt>N=2</tt> and for <tt>unsigned int</tt> subscripts
// * 
// * \param i subscript along first dimension (rows)
// * \param j subscript along second dimension (columns)
// * 
// * \return the index of the subscripted element in the Holor
// */
// template<>
// template<>
// inline size_t Layout<2>::operator()(unsigned int i, unsigned int j) const{
//     return offset_ + i*strides_[0] + j*strides_[1];
// }

// /*!
// * Specialization of operator Layout() for <tt>N=2</tt> and for \p long subscripts
// * 
// * \param i subscript along first dimension (rows)
// * \param j subscript along second dimension (columns)
// * 
// * \return the index of the subscripted element in the Holor
// */
// template<>
// template<>
// inline size_t Layout<2>::operator()(long i, long j) const{
//     return offset_ + i*strides_[0] + j*strides_[1];
// }

// /*!
// * Specialization of operator Layout() for <tt>N=2</tt> and for <tt>unsigned long</tt>> subscripts
// * 
// * \param i subscript along first dimension (rows)
// * \param j subscript along second dimension (columns)
// * 
// * \return the index of the subscripted element in the Holor
// */
// template<>
// template<>
// inline size_t Layout<2>::operator()(unsigned long i, unsigned long j) const{
//     return offset_ + i*strides_[0] + j*strides_[1];
// }



// /****************************************************************
// *                   SPECIALIZATIONS FOR N = 3
// ****************************************************************/
// /*!
// * Specialization of operator Layout() for <tt>N=3</tt> and for \p int subscripts
// * 
// * \param i subscript along first dimension (rows)
// * \param j subscript along second dimension (columns)
// * \param k subscript along third dimension (depth)
// * 
// * \return the index of the subscripted element in the Holor
// */
// template<>
// template<>
// inline size_t Layout<3>::operator()(int i, int j, int k) const{
//     return offset_ + i*strides_[0] + j*strides_[1] + k*strides_[2];
// }

// /*!
// * Specialization of operator Layout() for <tt>N=3</tt> and for <tt>unsigned int</tt> subscripts
// * 
// * \param i subscript along first dimension (rows)
// * \param j subscript along second dimension (columns)
// * \param k subscript along third dimension (depth)
// * 
// * \return the index of the subscripted element in the Holor
// */
// template<>
// template<>
// inline size_t Layout<3>::operator()(unsigned int i, unsigned int j, unsigned int k) const{
//     return offset_ + i*strides_[0] + j*strides_[1] + k*strides_[2];
// }

// /*!
// * Specialization of operator Layout() for <tt>N=3</tt> and for \p long subscripts
// * 
// * \param i subscript along first dimension (rows)
// * \param j subscript along second dimension (columns)
// * \param k subscript along third dimension (depth)
// * 
// * \return the index of the subscripted element in the Holor
// */
// template<>
// template<>
// inline size_t Layout<3>::operator()(long i, long j, long k) const{
//     return offset_ + i*strides_[0] + j*strides_[1] + k*strides_[2];
// }

// /*!
// * Specialization of operator Layout() for <tt>N=3</tt> and for <tt>unsigned long</tt>> subscripts
// * 
// * \param i subscript along first dimension (rows)
// * \param j subscript along second dimension (columns)
// * \param k subscript along third dimension (depth)
// * 
// * \return the index of the subscripted element in the Holor
// */
// template<>
// template<>
// inline size_t Layout<3>::operator()(unsigned long i, unsigned long j, unsigned long k) const{
//     return offset_ + i*strides_[0] + j*strides_[1] + k*strides_[2];
// }


} //namespace holor

#endif // HOLOR_LAYOUT_H;
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


/*!
 * \brief Structure that represents a range to index a slice of a Holor container. 
 * 
 * \b Example: Assume to have a 1D Holor container of size 7. If we wanted to select the slice that takes the elements from the second to the fourth, we 
 * can index the holor using `range{1, 3}` to create the slice.
 * 
 * \b Note: `range{1, 5, 1}` is equivalent to `1:5` in Matlab.
 */
struct range{
    size_t start_; /*! beginning of the range */
    size_t end_; /*! end of the range (this element is included in the range) */
    size_t step_; /*! number of steps between elements. \b Example: step = 1, the elements in the range are contiguous; step = 2, the range skips every other element. */

    /*!
     * \brief Constructor with checks on the arguments
     * \param start beginning of the range
     * \param end end (last element) of the range
     * \param step step between two elements in the range. Defaults to 1.
     */
    range(size_t start, size_t end, size_t step=1): start_{start}, end_{end}, step_{step}{
        // TODO: implement checks using dynamic asserts. it must be end > start, start >=0, end <= length[dim], step >=1

        // TODO: generalize to allow also ranges in decreasing order, e.g., start = 5, end = 1, step = -2
    }        
};


//TODO: introduce concept for range indexing
//TODO: introduce concept for size_t indexing?



/*! 
 * \brief predicate used to verify that at least one of the arguments used to index a layout is a range. This is needed to differentiate indexing of an element
 * and indexing of a slice
 * 
 * \tparam Args parameter pack of template arguments to be verified
 * 
 * \return true if some of the arguments are layouts
 */
template<typename... Args>
constexpr bool requesting_slice(){
    return assert::all((std::is_convertible<Args, size_t>() || std::is_same<Args, range>() || std::is_convertible<Args, range>())...) && assert::some(std::is_same<Args, range>()...);
    //TODO: replace with concepts
}









/*!
 * \brief Class that represents the memory layout of a Holor container or a Slice of a Holor.
 *
 * The Layout class contains the information for indexing the contiguous memory where the elements of the Holor or Slice are stored.
 * It uses the  idea of generalized layouts from the standard library, i.e., it is based on the fact that the elements of a Holor or Slice
 * are stored as a 1D data sequence following a row-major representation.
 * 
 * A layout contains three fundamental information: 
 *      - The \b offset is the offset in the contiguous memory of the first element indexed by the layout.  
 *      - The \b lengths are the numbers of elements for every dimension of the layout.
 *      - The \b strides are the distances in the 1D data sequence between successive elements in individual dimensions of the layout.
 *  For a Layout with `N` dimensions, both the length array and the stride array must be size `N`.
 * 
 * Given a Holor container with `N` dimensions, the elements of a layout are indexed from the underlying 1D data sequence according to the following formula
 * \f[
 *  index = offset + \sum_{j=0}^{N-1} j \cdot strides[j]
 * \f]
 * 
 * \tparam N is the number of dimensions in the layout
 */
template<size_t N>
class Layout{
    
    friend class Layout<N+1>; //TODO: this is ugly, needs to be fixed
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
         * \param layout Layout to be copied
         * 
         * \return a copy of the input Layout
         */
        Layout(const Layout<N>& layout) = default;


        /*!
         * \brief Copy assignment
         * 
         * \param layout Layout to be copied
         * 
         * \return a Layout copied from the input
         */
        Layout<N>& operator=(const Layout<N>& layout) = default;


        /*!
         * \brief Move constructor
         * 
         * \param layout Layout to be moved
         * 
         * \return a Layout moved from the input
         */
        Layout(Layout<N>&& layout) = default;


        /*!
         * \brief Move assignment
         * 
         * \param layout Layout to be moved
         * 
         * \return a Layout moved from the input
         */
        Layout<N>& operator=(Layout<N>&& layout) = default;


        /*!
         * \brief Constructor of a layout from an offset and an array of lengths
         * 
         * \param lengths array containing the number of elements along each dimension of the layout
         * \param offset index of the first element of the layout in the original Holor container (default is 0)
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


        /*!
         * \brief Constructor of a layout from an offset, an array of lengths and an array of strides
         * 
         * \param lengths array containing the number of elements along each dimension of the layout
         * \param strides array containing the strides along each dimension of the layout
         * \param offset index of the first element of the layout in the original Holor container (default is 0)
         * 
         * \return a Layout
         */
        Layout(const std::array<size_t,N>& lengths, const std::array<size_t,N>& strides, size_t offset=0): offset_{offset}, lengths_{lengths}, strides_{strides} {
        };


        /*!
         * \brief Constructor of a layout from an offset and an array of lengths
         * 
         * \param lengths array containing the number of elements along each dimension of the layout
         * \param strides array containing the strides along each dimension of the layout
         * \param offset index of the first element of the layout in the original Holor container
         * 
         * \return a Layout
         */
        Layout(std::array<size_t,N>&& lengths, std::array<size_t,N>&& strides, size_t offset): offset_{offset}, lengths_{lengths}, strides_{strides} {
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
         * \return the offset (with respect to the layout Holor container) of the layout
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
                                INDEXING AND SLICING
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

        template<typename... Args>
        auto slice(Args... args){
            return slice_helper(0, args...);
        }


        //slices the layout along one single dimension
        //step is not used right now.
        Layout<N> slice_dimension(size_t dim, range range){
            Layout<N> res = *this;
            res.lengths_[dim] = range.end_-range.start_+1;
            res.size_ = std::accumulate(res.lengths_.begin(), res.lengths_.end(), 1, std::multiplies<size_t>());
            res.offset_ = offset_ + range.start_*strides_[dim];
            return res;
        }


        //TODO: now this requires that Layout<N> is friend to Layout<N+1>. This is not a clean solution
        //slices the layout along one single dimension
        Layout<N-1> slice_dimension(size_t dim, size_t num){
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

        //TODO: change std_enable_if with requires, maybe creating a concept for the type of arguments allowed
        template<typename FirstArg, typename... OtherArgs>
        auto slice_helper(size_t dim, FirstArg first, OtherArgs... other) {
            if constexpr(std::is_same<FirstArg, range>() || std::is_convertible<FirstArg, range>()){
                return slice_dimension(dim, first).slice_helper(dim+1, other...);
            }else{
                return slice_dimension(dim, first).slice_helper(dim, other...);
            }
        }

        template<typename FirstArg>
        auto slice_helper(size_t dim, FirstArg first) {
            return slice_dimension(dim, first);
        }
};





// =====================================================================================================================
// =====================================================================================================================
// =====================================================================================================================
// =====================================================================================================================
// =====================================================================================================================
// =====================================================================================================================
// =====================================================================================================================
// =====================================================================================================================


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
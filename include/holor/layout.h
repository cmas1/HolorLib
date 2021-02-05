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
#include <concepts>
#include <ranges>
#include <algorithm>

#include "../common/static_assert.h"
#include "../common/dynamic_assert.h"



namespace holor{


/*==============================================================================================
                        MONO-DIMENSIONAL RANGE FOR SLICING LAYOUTS
===============================================================================================*/
/*!
 * \brief Structure that represents a range to index a slice of a Holor container. 
 * \b Example: Assume to have a 1D Holor container of size 7. To select the slice that takes the elements from the second to the fourth, we 
 * can index the holor using `range{1, 3}` to create the slice.
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
        // assert::dynamic_assert<assertion_level(assert::AssertionLevel::internal)>( (end>start)&&(start>=0) EXCEPTION_MESSAGE("Invalid range") );
        // TODO: implement checks using dynamic asserts. it must be end > start, start >=0, end <= length[dim], step >=1
        // TODO: generalize to allow also ranges in decreasing order, e.g., start = 5, end = 1, step = -2
    }        
};


        

/*================================================================================================
                                INDEX: CONCEPTS AND PREDICATES
================================================================================================*/
/*!
 * \brief concept that represents a type that can be used to index a single element of a layout
 */
template<typename T>
concept SingleIndex = std::convertible_to<T, size_t>;

/*!
 * \brief concept that represents a type that can be used to index a range of a layout
 */
template<typename T>
concept RangeIndex = std::convertible_to<T, range>;

/*!
 * \brief concept that represents a type that can be used to index a layout
 */
template<typename T>
concept Index = SingleIndex<T> || RangeIndex<T>;

namespace impl{
    /*! 
    * \brief predicate used to test that the arguments of an indexing operation are actual indexes and at least one of them is a RangeIndex
    * \tparam Args pack of indices to be verified
    * \return true if at least one of the indices is a RangeIndex
    */
    template<Index... Args>
    constexpr bool range_indexing(){
        return assert::some(RangeIndex<Args>...);
    }
}




namespace impl{

    template<size_t Dim>
    struct slice_helper_prova{
        // WIP: the else in the if constexpr call slice helper on a Layou<N-1>, but that should be a private function. Something is wrong
        template<typename Layout, Index FirstArg, typename... OtherArgs>
        static auto recursion(Layout layout, FirstArg first, OtherArgs... other) {
            if constexpr(RangeIndex<FirstArg>){
                return slice_helper_prova<Dim+1>::recursion(layout.slice_dimension<Dim>(first), other...);
            }else{
                return slice_helper_prova<Dim>::recursion(layout.slice_dimension<Dim>(first), other...);
            }
        }


        template<typename Layout, typename FirstArg>
        static auto recursion(Layout layout, FirstArg first) {
            return layout.slice_dimension<Dim>(first);
        }
    };
}



/*================================================================================================
                                    Layout Class
================================================================================================*/
/*!
 * \brief Class that represents the contiguous memory layout of a Holor container or a Slice of a Holor.
 *
 * The Layout class contains the information for indexing the contiguous memory where the elements of the Holor or Slice are stored.
 * It uses the idea of generalized layouts from the standard library, i.e., it is based on the fact that the elements of a Holor or Slice
 * are stored as a 1D data sequence following a row-major representation.
 * 
 * A layout contains three fundamental information: 
 *      - The \b offset is the offset in the contiguous memory of the first element indexed by the layout.  
 *      - The \b lengths are the numbers of elements along every dimension of the layout.
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
    
    public:
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                CONSTRUCTORS, ASSIGNMENTS AND DESTRUCTOR
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/    
        /*!
         * \brief Default constructor that creates an empty layout with no elements
         */
        Layout():size_{0}, offset_{0}{};


        /*!
         * \brief Copy constructor
         * \param layout Layout to be copied
         * \return a copy of the input Layout
         */
        Layout(const Layout<N>& layout) = default;


        /*!
         * \brief Copy assignment
         * \param layout Layout to be copied
         * \return a copy of the input Layout
         */
        Layout<N>& operator=(const Layout<N>& layout) = default;


        /*!
         * \brief Move constructor
         * \param layout Layout to be moved
         * \return a Layout moved from the input
         */
        Layout(Layout<N>&& layout) = default;


        /*!
         * \brief Move assignment
         * \param layout Layout to be moved
         * \return a Layout moved from the input
         */
        Layout<N>& operator=(Layout<N>&& layout) = default;


        /*!
         * \brief Constructor of a layout from an offset and an array of lengths
         * \param lengths array containing the number of elements along each dimension of the layout
         * \param offset index of the first element of the layout in the original Holor container (default is 0)
         * \return a Layout
         */
        Layout(const std::array<size_t,N>& lengths, size_t offset=0): offset_{offset}, lengths_{lengths} {
            update_strides_size();
        };


        /*!
         * \brief Constructor of a layout from an offset and an array of lengths
         * \param lengths array containing the number of elements along each dimension of the layout
         * \param offset index of the first element of the layout in the original Holor container
         * \return a Layout
         */
        Layout(std::array<size_t,N>&& lengths, size_t offset): offset_{offset}, lengths_{lengths} {
            update_strides_size();
        };


        /*!
         * \brief Constructor of a layout from an offset, an array of lengths and an array of strides
         * \param lengths array containing the number of elements along each dimension of the layout
         * \param strides array containing the strides along each dimension of the layout
         * \param offset index of the first element of the layout in the original Holor container (default is 0)
         * \return a Layout
         */
        Layout(const std::array<size_t,N>& lengths, const std::array<size_t,N>& strides, size_t offset=0): offset_{offset}, lengths_{lengths}, strides_{strides} {
            update_size();
        };


        /*!
         * \brief Constructor of a layout from an offset and an array of lengths
         * \param lengths array containing the number of elements along each dimension of the layout
         * \param strides array containing the strides along each dimension of the layout
         * \param offset index of the first element of the layout in the original Holor container
         * \return a Layout
         */
        Layout(std::array<size_t,N>&& lengths, std::array<size_t,N>&& strides, size_t offset): offset_{offset}, lengths_{lengths}, strides_{strides} {
            update_size();
        };

       

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                    GET/SET FUNCTIONS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Get the number of dimensions of the layout
         * \return the number `N` of dimensions of the layout.
         */
        constexpr size_t dimensions(){
            return N;
        }

        /*!
         * \brief Get the size of the layout. This is a const function.
         * \return the size (total number of elements) of the layout
         */
        size_t size() const{
            return size_;
        }

        /*!
         * \brief updates the size of the layout. This is only useful when the layout is modified using a set_lengths or set_length function.
         */
        void update_size(){
            size_ = std::accumulate(lengths_.begin(), lengths_.end(), 1, std::multiplies<size_t>());
        }

        /*!
         * \brief Get the offset of the layout. This is a const function.
         * \return the offset (with respect to the layout Holor container) of the layout
         */
        size_t offset() const{
            return offset_;
        }

        /*!
         * \brief Get the lengths of the layout. This is a const function.
         * \return the lengths (number of elements per dimension) of the layout
         */
        std::array<size_t,N> lengths() const{
            return lengths_;
        }

        /*!
         * \brief Get the strides of the layout. This is a const function.
         * \return the strides of the layout
         */
        std::array<size_t,N> strides() const{
            return strides_;
        }

        /*!
         * \brief Set the lengths of the layout, without updating the strides
         * \param lengths are the lengths to be set
         */
        void set_lengths(const std::array<size_t,N>& lengths) {
            lengths_ = lengths;
        }

        /*!
         * \brief Set the lengths of the layout, updating also the strides
         * \param lengths are the lengths to be set
         */
        void set_lengths_ss(const std::array<size_t,N>& lengths) {
            lengths_ = lengths;
            update_strides_size();
        }

        /*!
         * \brief Set a single length of the layout, without updating the strides
         * \param length is the lengths to be set
         * \param dim is the dimension of the layout where the length is set
         */
        void set_length(size_t length, size_t dim) {
            assert::dynamic_assert(dim>=0 && dim<N, EXCEPTION_MESSAGE("holor::Layout - Tried to index invalid dimension.") );
            lengths_[dim] = length;
        }

        /*!
         * \brief Set the strides of the layout
         * \param strides are the strides to be set
         */
        void set_strides(std::array<size_t,N> strides) {
            strides_ = strides;
        }

        /*!
         * \brief Set a single stride of the layout
         * \param stride is the stride to be set
         * \param dim is the dimension of the layout where the stride is set
         */
        void set_stride(size_t stride, size_t dim) {
            assert::dynamic_assert(dim>=0 && dim<N, EXCEPTION_MESSAGE("holor::Layout - Tried to index invalid dimension.") );
            strides_[dim] = stride;
        }

        /*!
         * \brief Set the offset of the layout
         * \param offset is the new offset of the layout
         */
        void set_offset(size_t offset) {
            offset_ = offset;
        }


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            INDEXING AND SLICING
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Function for indexing a single element from the Layout
         * \tparam Dims are the types of the parameter pack. Dims must e a pack of `N` parameters, each indexing a single element along a dimension of the Layout
         * \param dims parameters pack containing the subscripts
         * \return the index of the subscripted element in the Holor
         */
        template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
        size_t operator()(Dims&&... dims) const{
            return offset_ + single_element_indexing_helper<0>(std::forward<Dims>(dims)...);
        }

        /*!
         * \brief Function for indexing a slice from the Layout
         * \tparam Args are the types of the parameter pack. Dims must e a pack of `N` parameters, with at least one of them indexing a range of elements along a dimension of the Layout
         * \param args parameters pack. Each element of the pack indexes either an element or a range of elements along a dimension of the Layout.
         * \return the Layout containing the indexed range of elements
         */
        template<typename... Args> requires (impl::range_indexing<Args...>() && (sizeof...(Args)==N) )
        auto operator()(Args&&... args) const{
            // return slice_helper(0, std::forward<Args>(args)...);
            return impl::slice_helper_prova<0>::recursion(*this, std::forward<Args>(args)...);
        }

        /*!
         * \brief Function for indexing a single dimension of the Layout
         * \tparam Dim dimension to be sliced. `Dim` must be a value in the range `[0, N-1]`.
         * \param range the range of elements to be taken from the dimension `Dim`. `range` must indicate a valid range of indices in the dimension `Dim`.
         * \return a new Layout with `N` dimensions, where the dimension `Dim` contains only the lements indexed by the `range` argument.
         * \exception holor::exception::HolorInvalidArgument if `range` does not satisfy their constraints. The exception level is `release`.
         * \b Note: the level of dynamic checks is by default set on `release`, and can be changed by setting the compiler directive `DEFINE_ASSERT_LEVEL`. For example, setting in the CMakeLists file '-DDEFINE_ASSERT_LEVEL=no_checks` disables all dynamic checks.
         */
        template<size_t Dim> requires(Dim>=0 && Dim<N)
        Layout<N> slice_dimension(range range) const{
            assert::dynamic_assert(range.end_ < lengths_[Dim], EXCEPTION_MESSAGE("holor::Layout - Tried to index invalid range.") );
            Layout<N> res = *this;
            res.lengths_[Dim] = range.end_-range.start_+1;
            res.size_ = std::accumulate(res.lengths_.begin(), res.lengths_.end(), 1, std::multiplies<size_t>());
            res.offset_ = offset_ + range.start_*strides_[Dim];
            return res;
        }

        /*!
         * \brief Function for indexing a single dimension of the Layout
         * \tparam Dim dimension to be sliced. `Dim` must be a value in the range `[0, N-1]`.
         * \param num the index of the element to be taken from the dimension `Dim`.
         * \return a new Layout with `N-1` dimensions, where the dimension `Dim` is reduced to a single element indexed by the argument `num`. `num` must be a valid value with respect to the number of elements in the dimension `Dim`.
         * \exception holor::exception::HolorInvalidArgument if `num` does not satisfy their constraints. The exception level is `release`.
         * \b Note: the level of dynamic checks is by default set on `release`, and can be changed by setting the compiler directive `DEFINE_ASSERT_LEVEL`. For example, setting in the CMakeLists file '-DDEFINE_ASSERT_LEVEL=no_checks` disables all dynamic checks.
         */
        template<size_t Dim> requires(Dim>=0 && Dim<N)
        Layout<N-1> slice_dimension(size_t num) const{
            assert::dynamic_assert(num>=0 && num<lengths_[Dim], EXCEPTION_MESSAGE("holor::Layout - Tried to index invalid element.") );
            Layout<N-1> res;
            size_t i = 0;
            for(size_t j = 0; j < N; j++){
                if (j != Dim){
                    res.set_length(lengths_[j], i);
                    res.set_stride(strides_[j], i);
                    i++;
                }
            }
            res.update_size();
            res.set_offset(offset_ + num*strides_[Dim]);
            return res;
        }


        //TODO: right now slice_dimension and slice_helper take dim as a parameter. Conceptually, it would make sense to have it as a template parameter, to separate the indexing argument from the dimension. Moreover, in slice_helper it is used to unwind the recursive calls to the function, therefore it would be better set as a template parameter. Using a template parameter would also make it possible to perform translate all the checks on dim as compile time requiurements, rather than dynamic assertions. The problem is that simply changing it to a template parameter makes slice_helper not compile anymore. 

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        PRIVATE MEMBERS AND FUNCTIONS
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    private:
        std::array<size_t,N> lengths_; /*! number of elements in each dimension */
        size_t size_; /*! total number of elements of the layout */
        size_t offset_; /*! offset from the beginning of the array of elements of the tensor where the layout starts */
        std::array<size_t,N> strides_; /*! distance between consecutive elements in each dimension */


        /*!
         * \brief Computes and sets the strides and total size of the Layout based on its lengths
         */
        void update_strides_size(){
            size_ = 1;
            for(int i = N-1; i>=0; --i){
                strides_[i] = size_;
                size_ *= lengths_[i];
            }
        }

        /*!
         * \brief Helper function that is used to index a single element of the layout. It uses a variadic template, where the indices for each dimension of the layout are unwind one at a time
         * \tparam M dimension to be indexed by the `FirstArg`
         * \tparam FirstArg first index of the parameter pack
         * \tparam OtherArgs rest of the indices in the parameter pack
         * \param first is the index to be considered for the dimension `M`
         * \param other is the pack with the remaining indices that need to be unwind
         */
        template<size_t M, SingleIndex FirstArg, SingleIndex... OtherArgs>
        size_t single_element_indexing_helper(FirstArg first, OtherArgs&&... other) const{
            return first * strides_[M] + single_element_indexing_helper<M+1>(std::forward<OtherArgs>(other)...);
        }

        /*!
         * \brief Helper function that is used to index a single element of the layout. It is the final recursion when unwinding the parameter pack of indices
         * \tparam M dimension to be indexed by the `FirstArg`
         * \tparam FirstArg first index of the parameter pack
         * \param first is the index to be considered for the dimension `M`
         */
        template<size_t M, SingleIndex FirstArg>
        size_t single_element_indexing_helper(FirstArg first) const{
            return first * strides_[M];
        }



        // WIP: the else in the if constexpr call slice helper on a Layou<N-1>, but that should be a private function. Something is wrong
        template<Index FirstArg, typename... OtherArgs>
        auto slice_helper(size_t dim, FirstArg first, OtherArgs... other) const{
            if constexpr(RangeIndex<FirstArg>){
                return slice_dimension(dim, first).slice_helper(dim+1, other...);
            }else{
                return slice_dimension(dim, first).slice_helper(dim, other...);
            }
        }


        template<typename FirstArg>
        auto slice_helper(size_t dim, FirstArg first) const{
            return slice_dimension(dim, first);
        }

}; //class Layout




//TODO: Specializations for N = 0, 1, 2, 3
/*================================================================================================
                                    LAYOUT SPECIALIZATIONS
================================================================================================*/


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
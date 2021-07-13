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

#include "./indexes.h"
#include "../common/static_assert.h"
#include "../common/dynamic_assert.h"


namespace holor{


/*================================================================================================
                        LAYOUT CONCEPT
================================================================================================*/
//WIP====================================
/*!
 * \brief 
 */
template<typename T>
concept LayoutType = true;
//NOTE: it must have indexing operations, it must have a fixed size, constructors from lengths, ...
// requires (LayoutType layout)
//indexing operations => layout()
//fixed size =>


// template<size_t N, typename... T>
// concept IndexesPack = (sizeof...(T)== N) && SingleIndex<T>;
//WIP====================================





/*================================================================================================
                                    HELPER FUNCTIONS FOR SLICING A LAYOUT
================================================================================================*/
template<size_t N>
class Layout;
 
namespace impl{

    //IMPROVE==========================================================================================================================================
    //IMPROVE: the slice_dimension operation is not very efficient, because it iterates over all dimensions and each time it returns a new layout
    //TODO: replace the template Layout with a proper concept
    /*!
     * \brief helper functor that is used by `Layout<N>::operator()(Args&&... args)` to index a slice of a Layout. The functor implements a recursive algorithm that indexes a dimension at a time, until they are all processed. Each iteration produces a new, subLayout.
     */
    template<size_t Dim>
    struct slice_helper{
        /*!
         * \brief recursive function used in slicing operatios
         * \tparam Layout is the type of the (sub)Layout the function slices
         * \tparam FirstArg is the type of the first `Index` in the parameter pack
         * \tparam OtherArgs are the `Index` type of the other indices besides the first
         * \param dim is an unsigned int that is used to unwind the recursion, depending on the type of indices.
         */
        template<typename Layout, Index FirstArg, typename... OtherArgs>
        auto operator()(Layout layout, FirstArg&& first, OtherArgs&&... other) const{
            if constexpr(RangeIndex<FirstArg>){
                return slice_helper<Dim+1>()(layout.template slice_dimension<Dim>(std::forward<FirstArg>(first)), std::forward<OtherArgs>(other)...);
            }else{
                return slice_helper<Dim>()(layout.template slice_dimension<Dim>(std::forward<FirstArg>(first)), std::forward<OtherArgs>(other)...);
            }
        }

        /*!
         * \brief final step of the recursive function used in slicing operatios
         * \tparam Layout is the type of the (sub)Layout the function slices
         * \tparam FirstArg is the type of the first `Index` in the parameter pack
         * \param dim is an unsigned int that is used to unwind the recursion, depending on the type of indices.
         */
        template<typename Layout, Index FirstArg>
        auto operator()(Layout layout, FirstArg&& first) const{
            return layout.template slice_dimension<Dim>(std::forward<FirstArg>(first));
        }
    };
}




/*================================================================================================
                                    LAYOUT CLASS
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
//TODO: mention that a Layout supports two fundamental operations: accessing an element and slicing
template<size_t N>
class Layout{

    
    /*
     * Layout<N> is made friend of Layout<M> so that we can modify its private ariables when slicing a layout (and reducing its dimension)
     */
    template<size_t M>
    friend class Layout;
    
    
    public:
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                    ALIASES
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        static constexpr size_t order = N; ///< \brief number of dimensions in the reference container 


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                CONSTRUCTORS, ASSIGNMENTS AND DESTRUCTOR
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/ 
        Layout():size_{0}, offset_{0}{};                            ///< \brief Default constructor that creates an empty layout with no elements
        Layout(const Layout<N>& layout) = default;                  ///< \brief default copy constructor
        Layout<N>& operator=(const Layout<N>& layout) = default;    ///< \brief default copy assignment
        Layout(Layout<N>&& layout) = default;                       ///< \brief default move constructor
        Layout<N>& operator=(Layout<N>&& layout) = default;         ///< \brief default move assignment

        /*!
         * \brief Constructor of a layout from a container of lengths with compile-time size equal to `N` (e.g., `std::array<size_t,N>`).
         * \param lengths container of the number of elements along each dimension of the layout
         * \return a Layout
         */
        template <class Container> requires assert::SizedTypedContainer<Container, size_t, N>
        explicit Layout(Container lengths) {
            offset_ = 0;
            std::copy(lengths.begin(), lengths.end(), lengths_.begin()); 
            update_strides_size();
        };

        /*!
         * \brief Constructor of a layout from a resizeable container of lengths with size equal to `N` (e.g., `std::vector<size_t>`). The dimensionality check on the lenght of the container is done at runtime. 
         * \param lengths container of the number of elements along each dimension of the layout
         * \exception holor::exception::HolorRuntimeError if the length of the resizeable container is not equal to `N`. The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to AssertionLevel::no_checks to exclude this check.
         * \return a Layout
         */
        template <class Container> requires assert::ResizeableTypedContainer<Container, size_t>
        explicit Layout(Container lengths) {
            assert::dynamic_assert(lengths.size()==N, EXCEPTION_MESSAGE("Wrong number of elements!"));
            offset_ = 0;
            std::copy(lengths.begin(), lengths.end(), lengths_.begin()); 
            update_strides_size();
        };

        /*!
         * \brief Constructor from a variadic template of lengths. For example, `Layout<N> my_layout(5,2)` creates a Layout for a container with 5 elements in the first dimension and 2 elements in the second dimension.
         * \tparam Lengths parameter pack of lengths. There must be `N` arguments in the pack.   
         * \param lengths variadic arguments denoting the number of elements along each dimension of the container.
         * \return a Layout
         */
        template<typename... Lengths> requires ((sizeof...(Lengths)==N) )
        explicit Layout(Lengths&&... lengths) {
            offset_ = 0;
            single_length_copy<0>(std::forward<Lengths>(lengths)...);
            update_strides_size();
        }
      

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                    GET/SET FUNCTIONS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Get the number of dimensions of the layout. This is a const function.
         * \return the number `N` of dimensions of the layout.
         */
        constexpr size_t dimensions() const{
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
         * \brief Get a length of a dimension of the layout. This is a const function.
         * \return the length along a dimension (number of elements in that dimension)
         */
        auto length(size_t dim) const{
            return lengths_[dim];
        }

        /*!
         * \brief Get the strides of the layout. This is a const function.
         * \return the strides of the layout
         */
        std::array<size_t,N> strides() const{
            return strides_;
        }

        /*!
         * \brief Get a stride along a dimension of the layout. This is a const function.
         * \return the stride along a dimension.
         */
        auto stride(size_t dim) const{
            return strides_[dim];
        }


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            INDEXING AND SLICING
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Function for indexing a single element from the Layout
         * \tparam Dims are the types of the parameter pack. Dims must e a pack of `N` parameters, each indexing a single element along a dimension of the Layout
         * \param dims parameters pack containing the subscripts
         * \return the index in memory of the selected element.
         */
        template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
        size_t operator()(Dims&&... dims) const{
            return offset_ + single_element_indexing_helper<0>(std::forward<Dims>(dims)...);
        }

        /*!
         * \brief Function for indexing a single element from the Layout
         * \tparam ID is a SingleIndex type
         * \param dims a container of indices, one for each dimension of the layout
         * \return the index in memory of the selected element.
         */
        template<SingleIndex ID>
        size_t operator()(std::array<ID,N> dims) const{
            auto result = offset_;
            for (auto cnt = 0; cnt<N; cnt++){
                result += dims[cnt]*strides_[cnt];
            }
            return result;
        }
        
        /*!
         * \brief Specialization of the function Layout<N>::operator()(Dims&&... dims) for the case when `N=1`, for a more efficient implementation. For the general case Layout<N> this specialization is not defined.
         */
        template<SingleIndex Index>
        size_t operator()(Index i) const;

        /*!
         * \brief Specialization of the function Layout<N>::operator()(Dims&&... dims) for the case when `N=2`, for a more efficient implementation. For the general case Layout<N> this specialization is not defined.
         */
        template<SingleIndex Index>
        size_t operator()(Index x, Index j) const;

        /*!
         * \brief Specialization of the function Layout<N>::operator()(Dims&&... dims) for the case when `N=3`, for a more efficient implementation. For the general case Layout<N> this specialization is not defined.
         */
        template<SingleIndex Index>
        size_t operator()(Index x, Index j, Index k) const;

        /*!
         * \brief Function for indexing a slice from the Layout
         * \tparam Args are the types of the parameter pack. Dims must e a pack of `N` parameters, with at least one of them indexing a range of elements along a dimension of the Layout
         * \param args parameters pack. Each element of the pack indexes either an element or a range of elements along a dimension of the Layout.
         * \return the Layout containing the indexed range of elements
         */
        template<typename... Args> requires (impl::range_indexing<Args...>() && (sizeof...(Args)==N) )
        auto operator()(Args&&... args) const{
            return impl::slice_helper<0>()(*this, std::forward<Args>(args)...);
        }
        
        //IMPROVE==========================================================================================================================================
        //IMPROVE: the slice_dimension operation is not very efficient, because it iterates over all dimensions and each time it returns a new layout. We can look at different things to improve this:
        //IMPROVE: 1) Coroutines, although it does not look an ideal solution
        //IMPROVE: 2) Templatized operators, but this is not easily scalable
        //IMPROVE: 3) We could use ranges, which support lazy evaluation, to operate on the lengths_ and size_ arrays. So that basically we can Chain operation on these ranges and, only at the end, after all arguments have been processed, create a Layout of the final size that evaluates these operations and outputs the resulting Layout.
        /*!
         * \brief Function for indexing a single dimension of the Layout
         * \tparam Dim dimension to be sliced. `Dim` must be a value in the range `[0, N-1]`.
         * \param range the range of elements to be taken from the dimension `Dim`. `range` must indicate a valid range of indices in the dimension `Dim`.
         * \return a new Layout with `N` dimensions, where the dimension `Dim` contains only the lements indexed by the `range` argument.
         * \exception holor::exception::HolorRuntimeError if `range` does not satisfy their constraints. The exception level is `release`.
         * \b Note: the level of dynamic checks is by default set on `release`, and can be changed by setting the compiler directive `DEFINE_ASSERT_LEVEL`. For example, setting in the CMakeLists file '-DDEFINE_ASSERT_LEVEL=no_checks` disables all dynamic checks.
         */
        template<size_t Dim> requires ( (Dim>=0) && (Dim <N) )
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
         * \exception holor::exception::HolorRuntimeError if `num` does not satisfy their constraints. The exception level is `release`.
         * \b Note: the level of dynamic checks is by default set on `release`, and can be changed by setting the compiler directive `DEFINE_ASSERT_LEVEL`. For example, setting in the CMakeLists file '-DDEFINE_ASSERT_LEVEL=no_checks` disables all dynamic checks.
         */
        template<size_t Dim> requires ( (Dim>=0) && (Dim <N) )
        Layout<N-1> slice_dimension(size_t num) const{
            assert::dynamic_assert(num>=0 && num<lengths_[Dim], EXCEPTION_MESSAGE("holor::Layout - Tried to index invalid element.") );
            Layout<N-1> res;
            size_t i = 0;
            for(size_t j = 0; j < N; j++){
                if (j != Dim){
                    res.lengths_[i] = lengths_[j]; 
                    res.strides_[i] = strides_[j];
                    i++;
                }
            }
            res.size_ = std::accumulate(res.lengths_.begin(), res.lengths_.end(), 1, std::multiplies<size_t>());
            res.offset_ = offset_ + num*strides_[Dim];
            return res;
        }
        //IMPROVE==========================================================================================================================================


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
            assert::dynamic_assert(first>=0 && first<lengths_[M], EXCEPTION_MESSAGE("holor::Layout - Tried to index invalid element.") );
            return first * strides_[M] + single_element_indexing_helper<M+1>(std::forward<OtherArgs>(other)...);
        }

        /*!
         * \brief this is the last step in unwinding the parameter pack with the function single_element_indexing_helper
         */
        template<size_t M, SingleIndex FirstArg>
        size_t single_element_indexing_helper(FirstArg first) const{
            assert::dynamic_assert(first>=0 && first<lengths_[M], EXCEPTION_MESSAGE("holor::Layout - Tried to index invalid element.") );
            return first * strides_[M];
        }


        /*!
         * \brief Helper function that is used to construct a Layout from a parameter pack of sizes that specify the number of elements along each dimension
         * \tparam M unsigned integer used to unpack the parameter pack, one element at a time, and copy into `lenghts_`
         * \tparam FirstLength first element of the parameter pack
         * \tparam OtherLengths remaining elements of the parameter pack
         * \param arg is the first dimension in the parameter pack
         * \param other remaining dimensions in the parameter pack
         */
        template<size_t M, typename FirstLength, typename... OtherLengths> requires (std::convertible_to<FirstLength, size_t>)
        void single_length_copy(FirstLength arg, OtherLengths&&... other){
            lengths_[M] = arg;
            single_length_copy<M+1>(std::forward<OtherLengths>(other)...);
        }

        /*!
         * \brief this is the last step in unwinding the parameter pack with the function single_length_copy
         */
        template<size_t M, typename FirstLength> requires (std::convertible_to<FirstLength, size_t>)
        void single_length_copy(FirstLength arg){
            lengths_[M] = arg;
        }

}; //class Layout



/*================================================================================================
                                    LAYOUT INDEXING SPECIALIZATIONS
================================================================================================*/
/*!
 * \brief Specialization of the function for indexing a single element from Layout with dimension `N=1`
 * \tparam Index is the type of the index
 * \param i is the value of the index
 * \return the index of the subscripted element in the Holor
 */
template<>
template<SingleIndex Index> 
size_t Layout<1>::operator()(Index i) const{
    assert::dynamic_assert(i>=0 && i<lengths_[0], EXCEPTION_MESSAGE("holor::Layout - Tried to index invalid element.") );
    return offset_ + i*strides_[0];
}


/*!
 * \brief Specialization of the function for indexing a single element from Layout with dimension `N=2`
 * \tparam Index1 is the type of the first index
 * \tparam Index2 is the type of the second index
 * \param i is the value of the first index
 * \param j is the value of the second index
 * \return the index of the subscripted element in the Holor
 */
template<>
template<SingleIndex Index> 
size_t Layout<2>::operator()(Index i, Index j) const{
    assert::dynamic_assert( (i>=0 && i<lengths_[0]) && (j>=0 && j<lengths_[1]), EXCEPTION_MESSAGE("holor::Layout - Tried to index invalid element.") );
    return offset_ + i*strides_[0] + j*strides_[1];
}


/*!
 * \brief Specialization of the function for indexing a single element from Layout with dimension `N=3`
 * \tparam Index1 is the type of the first index
 * \tparam Index2 is the type of the second index
 * \tparam Index3 is the type of the second index
 * \param i is the value of the first 
 * \param j is the value of the second index
 * \param k is the value of the third index
 * \return the index of the subscripted element in the Holor
 */
template<>
template<SingleIndex Index> 
size_t Layout<3>::operator()(Index i, Index j, Index k) const{
    assert::dynamic_assert( (i>=0 && i<lengths_[0]) && (j>=0 && j<lengths_[1]) && (k>=0 && k<lengths_[2]), EXCEPTION_MESSAGE("holor::Layout - Tried to index invalid element.") );
    return offset_ + i*strides_[0] + j*strides_[1] + k*strides_[2];
}

} //namespace holor

#endif // HOLOR_LAYOUT_H;
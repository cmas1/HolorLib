// This file is part of Holor, a C++ header-only template library for multi-dimensional containers

// Copyright 2020-2022 Carlo Masone

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


#ifndef HOLOR_LAYOUT_CIRCULAR_H
#define HOLOR_LAYOUT_CIRCULAR_H

#include <cstddef>
#include <array>
#include <numeric>
#include <type_traits>
#include <concepts>
#include <ranges>
#include <algorithm>

#include "../indexes/indexes.h"
#include "./layout_concepts.h"
#include "./layout.h"
#include "../common/static_assertions.h"
#include "../common/runtime_assertions.h"


namespace holor{

//TODO: check all the comments

/*================================================================================================
                                    LAYOUT CIRCULAR CLASS
================================================================================================*/
/*!
 * \brief Class that represents the (ptentially non contiguous) memory layout of a Circular Slice of a Holor.
 *
 * The LayoutCircular class contains the information for indexing the potentially non-contiguous memory obtained from circularly slicing a Holor or HolorRef.
 * It uses the idea of generalized layouts from the standard library, i.e., it is based on the fact that the elements of a Holor or HolorRef
 * are stored as a 1D data sequence following a row-major representation.
 * 
 * A LayoutCircular object contains three fundamental information: 
 *      - The __offset__ is the offset in the contiguous memory sliced Holor or HolorRef that refers to the first element indexed by the layout.  
 *      - The __lengths__ are the numbers of elements along every dimension of the circular layout.
 *      - The __strides__ are the distances in the 1D data sequence between successive elements in individual dimensions of the circular layout.
 *  For a LayoutCircular with `N` dimensions, both the length array and the stride array must be size `N`.
 * A LayoutCircular supports one fundamental operation:
 * - __Indexing__ a single element: this operation provides a map from the coordinates in the container to the index in memory of the selected element;
 * 
 * Note that a LayoutCircular does not support slicing operations, because successive circular slices may become invalid!
 * 
 * \tparam `N` is the number of dimensions in the layout
 */
template<size_t N> requires (N>0)
class LayoutCircular{  
    
    public:
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                    ALIASES
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        static constexpr size_t order = N; ///< \brief number of dimensions in the reference container 
        using layout_type = holor::impl::LayoutTypeTag; ///<!  \brief tags a Layout type
        

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                CONSTRUCTORS, ASSIGNMENTS AND DESTRUCTOR
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/ 
        ///< \brief creates an empty layout with no elements
        LayoutCircular():size_{0}, offset_{0}{
            lengths_.fill(0);
            strides_.fill(0);
        };                            
        LayoutCircular(const LayoutCircular<N>& layout) = default;                  ///< \brief default copy constructor
        LayoutCircular<N>& operator=(const LayoutCircular<N>& layout) = default;    ///< \brief default copy assignment
        LayoutCircular(LayoutCircular<N>&& layout) = default;                       ///< \brief default move constructor
        LayoutCircular<N>& operator=(LayoutCircular<N>&& layout) = default;         ///< \brief default move assignment


        /*!
         * \brief Constructor of a layout from containers of `N` elements specifying the lengths of the LayoutCircular as well as the strides, offsets and lengths of the original Layout
         * \param lengths container of the number of elements along each dimension of the layout
         * \param strides container of the strides along each dimension of the layout
         * \param offsets container of the offsets along each dimension of the layout
         * \param lengthsOG container of the number of elements along each dimension of the sliced (original) layout
         * \return a LayoutCircular
         */
        template <class Container> requires assert::RSTypedContainer<Container, size_t, N>
        explicit LayoutCircular(const Container& lengths, const Container& strides, const Container& offsets, const Container& lengthsOG) {
            if constexpr(assert::ResizeableContainer<Container>){
                assert::dynamic_assert(lengths.size()==N, EXCEPTION_MESSAGE("Wrong number of elements!"));
            }
            std::copy(lengths.begin(), lengths.end(), lengths_.begin());
            std::copy(strides.begin(), strides.end(), strides_.begin());
            std::copy(offsets.begin(), offsets.end(), offsets_.begin());
            std::copy(lengthsOG.begin(), lengthsOG.end(), lengthsOG_.begin());
            size_ = std::accumulate(lengths_.begin(), lengths_.end(), 1, std::multiplies<size_t>());
        };

        /*!
         * \brief Constructor of a layout from containers of `N` elements specifying the lengths of the LayoutCircular as well as the strides, offsets and lengths of the original Layout
         * \param lengths container of the number of elements along each dimension of the layout
         * \param strides container of the strides along each dimension of the layout
         * \param offsets container of the offsets along each dimension of the layout
         * \param lengthsOG container of the number of elements along each dimension of the sliced (original) layout
         * \return a LayoutCircular
         */
        template <class Container> requires assert::RSTypedContainer<Container, size_t, N>
        explicit LayoutCircular(const Container& lengths, const Container& offsets, const Layout<N>& layoutOG) {
            if constexpr(assert::ResizeableContainer<Container>){
                assert::dynamic_assert(lengths.size()==N, EXCEPTION_MESSAGE("Wrong number of elements!"));
            }
            std::copy(lengths.begin(), lengths.end(), lengths_.begin());
            std::copy(offsets.begin(), offsets.end(), offsets_.begin());
            std::ranges::copy(layoutOG.strides(), strides_.begin());
            std::ranges::copy(layoutOG.lengths(), lengthsOG_.begin());
            size_ = std::accumulate(lengths_.begin(), lengths_.end(), 1, std::multiplies<size_t>());
        };


        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                    COMPARISON FUNCTIONS
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
        * \brief comperison operator that verifies the equality of Layout objects of the same order `M`
        * \tparam M is the order of the two Layouts
        * \param l1 is the first Layout of the comparison
        * \param l2 is the second Layout of the comparison
        * \return true if the comparison is satisfied, false otherwise
        */
        template<size_t M>
        friend bool operator==(const LayoutCircular<M>& l1, const LayoutCircular<M>& l2);


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
         * \brief Get the offsets of the layout. This is a const function.
         * \return the offsets (with respect to the layout Holor container) of the layout
         */
        std::array<size_t,N> offsets() const{
            return offsets_;
        }
        
        /*!
         * \brief Get the lengths of the layout. This is a const function.
         * \return the lengths (number of elements per dimension) of the layout
         */
        std::array<size_t,N> lengths() const{
            return lengths_;
        }

        /*!
         * \brief Get the lengths of the original (sliced) layout. This is a const function.
         * \return the lengths (number of elements per dimension) of the originally sliced layout
         */
        std::array<size_t,N> lengthsOG() const{
            return lengthsOG_;
        }

        /*!
         * \brief Get a length of a dimension of the layout. This is a const function.
         * \param dim dimension queried
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

        //TODO: not sure that the transpose operation is needed for this layout
        /*!
         * \brief Function that inverts the lengths and strides of the layout. It is useful to perfrom transpose operations
         */
        void transpose(){
            std::ranges::reverse(lengths_);
            std::ranges::reverse(strides_);
        }

        /*!
         * \brief Function that reorders the lengths and strides of the layout based on an input. It is useful to perfrom transpose operations
         * \param order is a container that containes the indices of the new order of the lengths and strides. There is no check on the feasibility of its values.
         */
        template <class Container> requires assert::RSTypedContainer<Container, size_t, N>
        void transpose(const Container& order){
            std::array<size_t, N> reordered_lengths;
            std::array<size_t, N> reordered_strides;
            for (auto i = 0; i < N; i++){
                reordered_lengths[i] = lengths_[order[i]];
                reordered_strides[i] = strides_[order[i]];
            }
            std::copy(reordered_lengths.begin(), reordered_lengths.end(), lengths_.begin()); 
            std::copy(reordered_strides.begin(), reordered_strides.end(), strides_.begin()); 
        }

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                            INDEXING AND SLICING
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*!
         * \brief Function for indexing a single element from the Layout
         * \tparam Dims are the types of the parameter pack. Dims must e a pack of `N` parameters, each indexing a single element along a dimension of the Layout
         * \param dims parameters pack containing the subscripts
         * \exception holor::exception::HolorRuntimeError if the indices passed as arguments are invalid. The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to AssertionLevel::no_checks to exclude this check.
         * \return the index in memory of the selected element.
         */
        template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
        size_t operator()(Dims&&... dims) const{
            return offset_ + single_element_indexing_helper<0>(std::forward<Dims>(dims)...);
        }

        /*!
         * \brief Function for indexing a single element from the Layout given a container of indices
         * \tparam Container is a container (resizeable or with a fixed size) of SingleIndex type
         * \param dims a container of indices, one for each dimension of the layout
         * \return the index in memory of the selected element.
         */
        template <class Container> requires assert::RSContainer<Container, N> && SingleIndex<typename Container::value_type>
        size_t operator()(Container dims) const{
            if constexpr(assert::ResizeableContainer<Container>){
                assert::dynamic_assert(dims.size()==N, EXCEPTION_MESSAGE("Wrong number of elements!"));
            }
            auto result = offset_;
            for (auto cnt = 0; cnt<N; cnt++){
                result += (offsets_[cnt] + dims[cnt] * strides_[cnt]) % lengthsOG[cnt];
            }
            return result;
        }

        /*!
         * \brief Specializations of the function LayoutCircular<N>::operator()(Dims&&... dims) for the case when `N=1, ..., 4`, for a more efficient implementation.
         */
        template<SingleIndex Index>
        size_t operator()(Index i) const;

        template<SingleIndex Index>
        size_t operator()(Index x, Index j) const;

        template<SingleIndex Index>
        size_t operator()(Index x, Index j, Index k) const;

        template<SingleIndex Index>
        size_t operator()(Index x, Index j, Index k, Index w) const;


    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        PRIVATE MEMBERS AND FUNCTIONS
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    private:
        std::vector<size_t> lengthsOG_; /*! number of elements in each dimension of the original container that is sliced circularly*/
        std::array<size_t,N> lengths_; /*! number of elements in each dimension */
        std::array<size_t,N> offsets_; /*! offset from the beginning of the array in memory where the elements of the tensor are stored */
        std::array<size_t,N> strides_; /*! distance between consecutive elements in each dimension */
        size_t size_; /*! total number of elements of the layout */

        /*!
         * \brief Helper recursive template function that is used to index a single element of the layout. It uses a variadic template, where the indices for each dimension of the layout are unwind one at a time
         * \tparam M dimension to be indexed by the `FirstArg`
         * \tparam FirstArg first index of the parameter pack
         * \tparam OtherArgs rest of the indices in the parameter pack
         * \param first is the index to be considered for the dimension `M`
         * \param other is the pack with the remaining indices that need to be unwind
         * \exception holor::exception::HolorRuntimeError if `first` is not within the range [0, `lengths[M]). The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to AssertionLevel::no_checks to exclude this check.
         */
        template<size_t M, SingleIndex FirstArg, SingleIndex... OtherArgs>
        size_t single_element_indexing_helper(FirstArg first, OtherArgs&&... other) const{
            assert::dynamic_assert(first>=0 && first<lengths_[M], EXCEPTION_MESSAGE("holor::Layout - Tried to index invalid element.") );
            return (offsets_[M] + first * strides_[M]) % lengthsOG[M] + single_element_indexing_helper<M+1>(std::forward<OtherArgs>(other)...);
        }

        template<size_t M, SingleIndex FirstArg>
        size_t single_element_indexing_helper(FirstArg first) const{
            assert::dynamic_assert(first>=0 && first<lengths_[M], EXCEPTION_MESSAGE("holor::Layout - Tried to index invalid element.") );
            return (offsets_[M] + first * strides_[M]) % lengthsOG[M];
        }

}; //class Layout



/*================================================================================================
                                    LAYOUT INDEXING SPECIALIZATIONS
================================================================================================*/
/*!
 * \brief Specialization of the function for indexing a single element from LayoutCircular with dimension `N=1`
 * \tparam Index is the type of the index
 * \param i is the value of the index
 * \return the index of the subscripted element in the Holor
 */
template<>
template<SingleIndex Index> 
size_t LayoutCircular<1>::operator()(Index i) const{
    assert::dynamic_assert(i>=0 && i<lengths_[0], EXCEPTION_MESSAGE("holor::LayoutCircular - Tried to index invalid element.") );
    return (offsets_[0] + i * strides_[0]) % lengthsOG[0];
}


/*!
 * \brief Specialization of the function for indexing a single element from LayoutCircular with dimension `N=2`
 * \tparam Index is the type of the indexes
 * \param i is the value of the first index
 * \param j is the value of the second index
 * \return the index of the subscripted element in the Holor
 */
template<>
template<SingleIndex Index> 
size_t LayoutCircular<2>::operator()(Index i, Index j) const{
    assert::dynamic_assert( (i>=0 && i<lengths_[0]) && (j>=0 && j<lengths_[1]), EXCEPTION_MESSAGE("holor::LayoutCircular - Tried to index invalid element.") );
    return (offsets_[0] + i * strides_[0]) % lengthsOG[0] + (offsets_[1] + j * strides_[1]) % lengthsOG[1];
}


/*!
 * \brief Specialization of the function for indexing a single element from LayoutCircular with dimension `N=3`
 * \tparam Index is the type of the indexes
 * \param i is the value of the first 
 * \param j is the value of the second index
 * \param k is the value of the third index
 * \return the index of the subscripted element in the Holor
 */
template<>
template<SingleIndex Index> 
size_t LayoutCircular<3>::operator()(Index i, Index j, Index k) const{
    assert::dynamic_assert( (i>=0 && i<lengths_[0]) && (j>=0 && j<lengths_[1]) && (k>=0 && k<lengths_[2]), EXCEPTION_MESSAGE("holor::LayoutCircular - Tried to index invalid element.") );
    return (offsets_[0] + i * strides_[0]) % lengthsOG[0] + (offsets_[1] + j * strides_[1]) % lengthsOG[1]  + (offsets_[2] + k * strides_[2]) % lengthsOG[2];
}


/*!
 * \brief Specialization of the function for indexing a single element from LayoutCircular with dimension `N=3`
 * \tparam Index is the type of the indexes
 * \param i is the value of the first 
 * \param j is the value of the second index
 * \param k is the value of the third index
 * \param w is the value of the fourth index
 * \return the index of the subscripted element in the Holor
 */
template<>
template<SingleIndex Index> 
size_t LayoutCircular<4>::operator()(Index i, Index j, Index k, Index w) const{
    assert::dynamic_assert( (i>=0 && i<lengths_[0]) && (j>=0 && j<lengths_[1]) && (k>=0 && k<lengths_[2]) && (w>=0 && w<lengths_[3]), EXCEPTION_MESSAGE("holor::LayoutCircular   - Tried to index invalid element.") );
    return (offsets_[0] + i * strides_[0]) % lengthsOG[0] + (offsets_[1] + j * strides_[1]) % lengthsOG[1] + (offsets_[2] + k * strides_[2]) % lengthsOG[2] + (offsets_[3] + w * strides_[3]) % lengthsOG[3];
}




/*================================================================================================
                                    COMPARISONS
================================================================================================*/
/*!
* \brief comparison operator that verifies the equality of Layout objects of the same order `M`
* \tparam M is the order of the two Layouts
* \param l1 is the first Layout of the comparison
* \param l2 is the second Layout of the comparison
* \return true if the comparison is satisfied, false otherwise
*/
template<size_t M>
inline bool operator==(const LayoutCircular<M>& l1, const LayoutCircular<M>& l2){
    return ((l1.offsets_ == l2.offsets_) && (l1.size_==l2.size_) && (l1.strides_==l2.strides_) && (l1.lengths_==l2.lengths_) && (l1.lengthsOG_==l2.lengthsOG_) );
}


/*!
* \brief comparison operator that verifies the inequality of Layout objects of the same order `M`
* \tparam M is the order of the two Layouts
* \param l1 is the first Layout of the comparison
* \param l2 is the second Layout of the comparison
* \return true if the two Layous are not equal, false otherwise
*/
template<size_t M>
inline bool operator!=(const LayoutCircular<M>& l1, const LayoutCircular<M>& l2){
    return !(l1==l2);
}

} //namespace holor

#endif // HOLOR_LAYOUT_CIRCULAR_H;
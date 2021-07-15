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


#ifndef HOLOR_INDEXES_H
#define HOLOR_INDEXES_H

#include <cstddef>

#include "../common/runtime_assertions.h"
#include "../common/static_assert.h"

namespace holor{


/*==============================================================================================
                        RANGE OF INDEXES
===============================================================================================*/
/*!
 * \brief Structure that represents a range of indexes to slice a Holor container. 
 * \b Example: Assume to have a 1D Holor container of size 7. To select the slice that takes the elements from the second to the fourth, we 
 * can index the holor using `range{1, 3}` to create the slice.
 * \b Note: `range{1, 5, 1}` is equivalent to `1:5` in Matlab. `range{1, 5, 2}` yields the indexes `1, 3, 5`.
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
        assert::dynamic_assert( (start>=0) && (end>start) && (step>0) , EXCEPTION_MESSAGE("Invalid range!"));
        // TODO: generalize to allow also ranges in decreasing order, e.g., start = 5, end = 1, step = -2
    }        
};


        

/*================================================================================================
                                CONCEPTS AND PREDICATES PERTAINING INDEXES
================================================================================================*/
/*!
 * \brief concept that represents a type that can be used to index a single element of a layout
 */
template<typename T>
concept SingleIndex = std::integral<T> && std::convertible_to<T, size_t>;

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
    template<typename... Args>
    constexpr bool ranged_index_pack(){
        return assert::all(Index<Args>...) && assert::some(RangeIndex<Args>...);
    };

    /*! 
    * \brief predicate used to test that the arguments of an indexing operation are actual indexes
    * \tparam Args pack of indices to be verified
    * \return true if at least one of the indices is a RangeIndex
    */
    template<typename... Args>
    constexpr bool index_pack(){
        return assert::all(Index<Args>...);
    };
}

} //namespace holor

#endif // HOLOR_INDEXES_H;
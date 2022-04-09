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

#ifndef HOLOR_OPERATIONS_H
#define HOLOR_OPERATIONS_H


#include "holor_concepts.h"
#include "../common/runtime_assertions.h"


namespace holor{


/*================================================================================================
                                    Holor Operations
================================================================================================*/

/*!
 * \brief broadcast function
 */
template <size_t D, HolorType DestHolor, HolorType SourceHolor, class Op> requires ((D < DestHolor::dimensions) && (SourceHolor::dimensions==1))
void broadcast(DestHolor& dest, SourceHolor source, Op&& operation ){
    assert::dynamic_assert(dest.length(D) == source.size(), EXCEPTION_MESSAGE("The dimension of the destination where the sources should be broadcasted should have length equal to the size of the source!"));
    for( auto i = 0; i < dest.length(D); i++){
        auto slice = dest.template slice<D>(i);
        std::invoke(std::forward<Op>(operation), slice, source, i);
    }
}


} //namespace holor

#endif // HOLOR_OPERATIONS_H
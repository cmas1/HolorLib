// This file is part of Holor, a C++ template library for multi-dimensional containers

// Copyright 2020-2021 Carlo Masone

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


#ifndef HOLOR_TYPES_H
#define HOLOR_TYPES_H

#include <type_traits>
#include <concepts>
#include <utility>

#include "indexes.h"
#include "../common/static_assertions.h"
#include "layout_types.h"

namespace holor{


/*================================================================================================
                                    Holor Concepts
================================================================================================*/

namespace impl{

    struct HolorTypeTag{};  ///<! \brief type that is used to tag a holor container


    /*!
     * \brief Constraints Holor Containers to have a order and a HolorTypeTag
     */
    template<typename T>
    concept HolorWithDimensions = (T::dimensions > 0) && requires (T holor){
        std::is_same<typename T::holor_type, impl::HolorTypeTag>();
    };

    /*!
     * \brief Constraints Holor Containers to have iterators
     */
    template<typename T>
    concept IterableHolor = requires (T holor){
        holor.data();
        holor.begin();
        holor.end();
        holor.cbegin();
        holor.cend();
        holor.rbegin();
        holor.rend();
        holor.crbegin();
        holor.crend();
    };

}


template<typename T>
concept HolorType = impl::HolorWithDimensions<T> && impl::IterableHolor<T> && requires (T holor){
    //TODO: it must have access functions
    //TODO: it must be sliceable
    //TODO: it can set _lengths if it is a Holor
    //it has various get functions
    {holor.layout()}->DecaysToLayoutType<>;
    {holor.size()}->std::same_as<size_t>;
    {holor.lengths()}->std::same_as<std::array<size_t,T::dimensions>>;
    {holor.length(0)}->std::same_as<size_t>;
};

} //namespace holor

#endif // HOLOR_TYPES_H
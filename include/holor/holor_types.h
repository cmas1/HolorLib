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

    struct HolorOwningTypeTag{};  ///<! \brief type that is used to tag a holor container that has ownership over its data (Holor)
    struct HolorNonOwningTypeTag{};  ///<! \brief type that is used to tag a holor container that does not have ownership over its data (HolorRef)


    /*!
     * \brief Constraints Holor Containers to have a order and a HolorTypeTag
     */
    template<typename T>
    concept HolorWithDimensions = (T::dimensions > 0) && requires (T holor){
        std::is_same<typename T::holor_type, impl::HolorOwningTypeTag>() || std::is_same<typename T::holor_type, impl::HolorNonOwningTypeTag>();
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


    
    /*!
     * \brief Helper function that is used to check the presence of a single element access operation from a pack of indices
     */
    template<typename Obj, size_t... Indices>
    auto holor_variadic_element_access(Obj& obj, std::index_sequence<Indices ...>){
        return obj.operator()(Indices ...);
    };

    /*!
     * \brief Helper function that is used to check the presence of a slice access operation from a pack of indices/ranges
     */
    template<typename Obj, size_t... Indices>
    auto holor_variadic_slice_access(Obj& obj, std::index_sequence<Indices ...>){
        return obj.operator()(holor::range{0,1}, Indices ...);
    };

    /*!
     * \brief Constraints Holor to have access functions
     */
    template<typename T>
    concept AccessibleHolor = requires (T holor){
        {impl::holor_variadic_element_access(holor, std::make_index_sequence<T::dimensions>{})}->std::same_as<typename T::value_type>;
        {impl::holor_variadic_slice_access(holor, std::make_index_sequence<T::dimensions-1>{})}->impl::HolorWithDimensions<>;
    };


    /*!
     * \brief Constraints Holor to be sliceable
     */
    template<typename T>
    concept SliceableHolorByDim = (T::dimensions==1) || requires (T holor){
        {holor.template slice<0>(0)}->HolorWithDimensions<>;
        {holor.template slice<T::dimensions-1>(0)}->HolorWithDimensions<>;
    };

    template<typename T>
    concept SliceableHolorByRow = (T::dimensions==1) || requires (T holor){
        {holor.row(0)}->HolorWithDimensions<>;
    };

    template<typename T>
    concept SliceableHolorByCol = (T::dimensions<2) || requires (T holor){
        {holor.col(0)}->HolorWithDimensions<>;
    };

    template<typename T>
    concept SliceableHolor = SliceableHolorByDim<T> && SliceableHolorByRow<T> && SliceableHolorByCol<T>;



    /*!
     * \brief Helper function that is used to check the presence of an set_length operation from a pack of indices
     */
    template<typename Obj, size_t... Indices>
    void holor_variadic_set_lengths(Obj& obj, std::index_sequence<Indices ...>){
        obj.set_lengths()(Indices ...);
    };


    /*!
     * \brief Constraints Layouts to have a resizeable lengths
     */
    template<typename T>
    concept ResizeableHolor = std::is_same_v<typename T::holor_type, impl::HolorNonOwningTypeTag> || requires (T holor){
        impl::holor_variadic_set_lengths(holor, std::make_index_sequence<T::dimensions>{});
        holor.set_lengths(std::array<size_t, T::dimensions>());
        holor.set_lengths(std::vector<size_t>());
    };

}


template<typename T>
concept HolorType = impl::HolorWithDimensions<T> && impl::IterableHolor<T> && impl::AccessibleHolor<T> && impl::SliceableHolor<T> && impl::ResizeableHolor<T> && requires (T holor){
    //it has various get functions
    {holor.layout()}->DecaysToLayoutType<>;
    {holor.size()}->std::same_as<size_t>;
    {holor.lengths()}->std::same_as<std::array<size_t,T::dimensions>>;
    {holor.length(0)}->std::same_as<size_t>;
};

} //namespace holor

#endif // HOLOR_TYPES_H
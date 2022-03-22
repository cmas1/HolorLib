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


#ifndef LAYOUT_TYPES_H
#define LAYOUT_TYPES_H


#include <type_traits>
#include <concepts>
#include <utility>

#include "indexes.h"
#include "../common/static_assertions.h"

namespace holor{


/*================================================================================================
                                    Layout Concepts
================================================================================================*/

namespace impl{

    struct LayoutTypeTag{};  ///< \brief type that is used to tag a layout


    /*!
     * \brief Helper function that is used to check the presence of an indexing operation from a pack of indices
     */
    template<typename Obj, size_t... Indices>
    auto layout_variadic_indexing(Obj& obj, std::index_sequence<Indices ...>){
        return obj.operator()(Indices ...);
    };


    /*!
     * \brief Constraints Layouts to have a suitable indexing function
     */
    template<typename T>
    concept IndexableLayout = requires (T layout){
        {impl::layout_variadic_indexing(layout, std::make_index_sequence<T::order>{})}->std::same_as<size_t>;
        {layout(std::array<size_t,T::order>{})}->std::same_as<size_t>;
    };


    /*!
     * \brief Helper function that is used to check the presence of an indexing operation from a pack of indices
     */
    template<typename Obj, size_t... Indices>
    void layout_variadic_set_lengths(Obj& obj, std::index_sequence<Indices ...>){
        obj.set_lengths()(Indices ...);
    };


    /*!
     * \brief Constraints Layouts to have a resizeable lengths
     */
    template<typename T>
    concept ResizeableLayout = requires (T layout){
        impl::layout_variadic_set_lengths(layout, std::make_index_sequence<T::order>{});
        layout.set_lengths(std::array<size_t, T::order>());
        layout.set_lengths(std::vector<size_t>());
    };


    /*!
     * \brief Constraints Layouts to have a order and a LayoutType tag
     */
    template<typename T>
    concept LayoutWithOrder = (T::order > 0) && requires (T layout){
        std::is_same<typename T::layout_type, impl::LayoutTypeTag>(); ///<! \brief it has a layout_type tag;
    };


    /*!
     * \brief Constraints Layouts to be sliceable
     */
    template<typename T>
    concept SliceableLayoutByRange = requires (T layout){
        {layout.template slice_dimension<0>(holor::range(0,1))}->std::same_as<T>;
        {layout.template slice_dimension<T::order -1>(holor::range(0,1))}->std::same_as<T>;
    };

    template<typename T>
    concept SliceableLayoutByDim = (T::order==1) || requires (T layout){
        layout.template slice_dimension<0>(0);
        layout.template slice_dimension<T::order -1>(0);
        requires ( decltype(layout.template slice_dimension<0>(0))::order == T::order-1);
        requires ( decltype(layout.template slice_dimension<T::order -1>(0))::order == T::order-1);
    };

    template<typename T>
    concept SliceableLayout = SliceableLayoutByRange<T> && SliceableLayoutByDim<T>;


}



template<typename T>
concept LayoutType = impl::LayoutWithOrder<T> && impl::IndexableLayout<T> && impl::ResizeableLayout<T> && impl::SliceableLayout<T> && std::equality_comparable<T> && requires (T layout){
    //it has various get functions
    {layout.dimensions()}->std::same_as<size_t>;
    {layout.size()}->std::same_as<size_t>;
    {layout.offset()}->std::same_as<size_t>;
    {layout.lengths()}->std::same_as<std::array<size_t,T::order>>;
    {layout.length(0)}->std::same_as<size_t>;
    {layout.strides()}->std::same_as<std::array<size_t,T::order>>;
    {layout.stride(0)}->std::same_as<size_t>;
};


} //namespace holor

#endif // LAYOUT_TYPES_H
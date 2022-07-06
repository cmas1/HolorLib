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


#include "../holor/holor_concepts.h"
#include "../common/runtime_assertions.h"
#include <algorithm>
#include <type_traits>
#include <numeric>
#include <cmath>

namespace holor{


/*================================================================================================
                                    Holor Operations
================================================================================================*/

//TODO: add requires clauses to constrain the OPs to be a binary/unary function (depending on the use case). 
//NOTE: for this purpose use std::is_invocable or std::is_invocable_r from <type_traits>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    BROADCAST
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*!
 * \brief The `broadcast` function is an operation that modifies one holor by broadcasting along one of its dimensions another holor (here called slice because its lengths must be consistent with a slice of the destination holor) and applying a binary function to their individual elements.
 * \tparam D is the dimension of the destination holor along which the slice is broadcasted
 * \tparam Destination is the type of the destination Holor container
 * \tparam Slice is the type of the broadcasted Holor container
 * \tparam OP is the binary function that is applied to the elements
 * \param dest is the holor that is modified by the broadcast operation
 * \param source_slice is the holor that is broadcasted
 * \param operation is the function that is applied to the pairs of elements from dest and source_slice
 */
template <size_t D, HolorType Destination, HolorType Slice, class Op> requires ((D < Destination::dimensions) && (Slice::dimensions==Destination::dimensions-1) && (std::is_same_v<typename Destination::value_type, typename Slice::value_type>))
void broadcast(Destination& dest, Slice source_slice, Op&& operation ){
    assert::dynamic_assert(dest.template slice<D>(0).lengths() == source_slice.lengths(), EXCEPTION_MESSAGE("The lengths of slice to be broadcasted are not consistent with the lengths of the destination container!"));
    for( auto i = 0; i < dest.length(D); i++){
        auto dest_slice = dest.template slice<D>(i);
        std::ranges::transform(dest_slice, source_slice, dest_slice.begin(), std::forward<Op>(operation));
    }
}

/*!
 * \brief The `broadcast_all` function is an operation that modifies one holor by replacing all its elements with the results from applying to them and to another input element a binary function.
 * \tparam Destination is the type of the destination Holor container
 * \tparam ElementType is the type of the broadcasted element
 * \tparam OP is the binary function that is applied to the elements
 * \param dest is the holor that is modified by the broadcast operation
 * \param element is the element that is broadcasted
 * \param operation is the function that is applied to the pairs of elements
 */
template <HolorType Destination, class ElementType, class Op> requires (std::is_same_v<typename Destination::value_type, ElementType>)
void broadcast_all(Destination& dest, ElementType element, Op&& operation ){
    for(auto& e : dest){
        e = std::invoke(std::forward<Op>(operation), e, element);
    }
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    REDUCE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*!
 * \brief The `reduce_all` function is an operation that takes an holor and reduces it to a single scalar value obtained by applying a unitary function to all the elements of the container.
 * \tparam Source is the Holor container to be reduced
 * \tparam ElementType is the type of the result
 * \tparam OP is the binary function that is applied to the elements
 * \param source is the holor that is reduced
 * \param result is the initial value from which the result is computed
 * \param operation is the function that is applied to the pairs of elements
 */
template <HolorType Source, class ElementType, class Op> requires (std::is_same_v<typename Source::value_type, ElementType>)
auto reduce_all(Source source, ElementType result, Op&& operation ){
    for(auto& e : source){
        result = std::invoke(std::forward<Op>(operation), e, result);
    }
    return result;
}

/*!
 * \brief The `reduce` function is an operation that takes an holor and reduces it to a slice value obtained by applying a unitary function to all the elements of the container.
 * \tparam D is the dimension of the source holor along which it is reduced
 * \tparam Source is the Holor container to be reduced
 * \tparam InitHolor is the type of the result
 * \tparam OP is the binary function that is applied to the elements
 * \param source is the holor that is reduced
 * \param result is the initial value from which the result is computed
 * \param operation is the function that is applied to the pairs of elements
 */
template <size_t D, HolorType Source, HolorType InitHolor, class Op> requires ((D < Source::dimensions) && (InitHolor::dimensions==Source::dimensions-1) && (std::is_same_v<typename Source::value_type, typename InitHolor::value_type>))
auto reduce(Source source, InitHolor result, Op&& operation ){
    assert::dynamic_assert(source.template slice<D>(0).lengths() == result.lengths(), EXCEPTION_MESSAGE("The lenghts of the result container are not consistent with the dimensions of the source container!"));
    for( auto i = 0; i < source.length(D); i++){
        auto source_slice = source.template slice<D>(i);
        std::ranges::transform(result, source_slice, result.begin(), std::forward<Op>(operation));
    }
    return result;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    APPLY
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*!
 * \brief The `apply` function is an operation that modifies one holor by applying a unary function to all its elements
 * \tparam Destination is the type of the Holor container that is transformed by the function
 * \tparam OP is the unary function that is applied to the elements
 * \param dest is the holor that is modified
 * \param operation is the function that is applied to the elements in the container
 */
template <HolorType Destination, class Op>
void apply(Destination& dest, Op&& operation ){
    std::ranges::transform(dest, dest.begin(), std::forward<Op>(operation));
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    CONCATENATION
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*!
 * \brief Helper functions that are used to implement the concatenate operation
 */
namespace impl_concatenate{
    template <class Container, DecaysToHolorType First_Arg> requires (assert::IterableContainer<Container>)
    void check_lengths(Container lengths, First_Arg first_arg){
        static_assert(std::is_same_v<decltype(lengths), decltype(first_arg.lengths())>, "The arguments of the concatenation have different dimensions!" );
        if constexpr(std::is_same_v<decltype(lengths), decltype(first_arg.lengths())>){
            assert::dynamic_assert(lengths==first_arg.lengths(), EXCEPTION_MESSAGE("The arguments of the concatenation have different lengths!"));
        }
    }

    template <class Container, DecaysToHolorType First_Arg, DecaysToHolorType... Args> requires (assert::IterableContainer<Container>)
    void check_lengths(Container lengths, First_Arg first_arg, Args&&... args){
        static_assert(std::is_same_v<decltype(lengths), decltype(first_arg.lengths())>, "The arguments of the concatenation have different dimensions!" );
        if constexpr(std::is_same_v<decltype(lengths), decltype(first_arg.lengths())>){
            assert::dynamic_assert(lengths==first_arg.lengths(), EXCEPTION_MESSAGE("The arguments of the concatenation have different lengths!"));
        }
        check_lengths(lengths, std::forward<Args>(args)...);
    }

    template<typename T, DecaysToHolorType First_Arg>
    void check_type(First_Arg arg){
        static_assert(std::is_same_v<T, typename std::decay_t<First_Arg>::value_type>, "The arguments of the concatenation have inconsistent value_type");
    }

    template<typename T, DecaysToHolorType First_Arg, DecaysToHolorType... Args>
    void check_type(First_Arg arg, Args&&... args){
        static_assert(std::is_same_v<T, typename std::decay_t<First_Arg>::value_type>, "The arguments of the concatenation have inconsistent value_type");
        check_type<T>(std::forward<Args>(args)...);
    }

    template <size_t Dim, DecaysToHolorType First_Arg, DecaysToHolorType... Args>
    auto check_args(First_Arg first_arg, Args&&... args){
        auto lengths = first_arg.lengths();
        check_lengths(lengths, std::forward<Args>(args)...);
        check_type<typename std::decay_t<First_Arg>::value_type>(std::forward<Args>(args)...);
        static_assert(Dim<lengths.size(), "Invalid dimension for the concatenation.");
        constexpr auto N_concatenate = sizeof...(Args);
        lengths[Dim] *=(N_concatenate+1);
        return Holor<typename First_Arg::value_type, First_Arg::dimensions> (lengths);
    }

    template <size_t Dim, size_t M, DecaysToHolorType Result, DecaysToHolorType First_Arg>
    void do_concatenation(Result& result, First_Arg first_arg){
        auto length = first_arg.length(Dim);
        auto slice_result = result.template slice<Dim>(holor::range{M*length, (M+1)*length-1});
        slice_result.substitute(first_arg);
    }

    template <size_t Dim, size_t M, DecaysToHolorType Result, DecaysToHolorType First_Arg, DecaysToHolorType... Args>
    void do_concatenation(Result& result, First_Arg first_arg, Args&&... args){
        auto length = first_arg.length(Dim);
        auto slice_result = result.template slice<Dim>(holor::range{M*length, (M+1)*length-1});
        slice_result.substitute(first_arg);
        do_concatenation<Dim, M+1>(result, std::forward<Args>(args)...);
    }
}

template <size_t Dim, DecaysToHolorType... Args> requires (sizeof...(Args)>=2)
auto concatenate(Args&&... args){
    auto result = impl_concatenate::check_args<Dim>(std::forward<Args>(args)...);
    impl_concatenate::do_concatenation<Dim,0>(result, std::forward<Args>(args)...);
    return result;
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    TRANSPOSE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*!
 * \brief The `transpose` function is an operation that changes the coordinates of a Holor container (e.g. inverting them)
 * \param source is the holor that is transposed
 * \param order is the (optional) array of indices that specify the reordering of the Holor coordinates. There is no check on the values of these indices. If this parameter is not given the coordinates of the holor are inverted
 * \return a new Holor that is equal to the original one but transposed
 */
template <HolorType Source, class Container> requires assert::SizedTypedContainer<Container, size_t, Source::dimensions>
auto transpose(Source& source, Container order){
    auto layout = source.layout();
    layout.transpose(order);
    Holor<typename Source::value_type, Source::dimensions> result(layout);
    std::copy(source.begin(), source.end(), result.data());
    return result;
}

template <HolorType Source>
auto transpose(Source& source){
    auto layout = source.layout();
    layout.transpose();
    Holor<typename Source::value_type, Source::dimensions> result(layout);
    std::copy(source.begin(), source.end(), result.data());
    return result;
}

template <HolorType Source, class Container> requires assert::SizedTypedContainer<Container, size_t, Source::dimensions>
auto transpose_view(Source& source, Container order){
    auto layout = source.layout();
    layout.transpose(order);
    HolorRef<typename Source::value_type, Source::dimensions> result(source.data(), layout);
    return result;
}

template <HolorType Source>
auto transpose_view(Source& source){
    auto layout = source.layout();
    layout.transpose();
    HolorRef<typename Source::value_type, Source::dimensions> result(source.data(), layout);
    return result;
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    SHIFT
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*!
 * \brief The `shift` function is an operation that shifts the content of a Holor along a certain direction
 * \tparam Dim is the direction along which the Holor is shifted
 * \param source is the holor that is transposed
 * \param n indicates how many places the content should be shifted
 * \return a new Holor that is equal to the original one but shifted
 */
template <size_t Dim, HolorType Source> requires (Dim<Source::dimensions)
auto shift(Source source, int n){
    int length = source.length(Dim);
    Holor<typename Source::value_type, Source::dimensions> result(source.layout());
    for (int i = 0; i < length; i++){
        auto source_slice = source.template slice<Dim>(i);
        auto result_slice = result.template slice<Dim>(std::abs((i+n)%length));
        result_slice.substitute(source_slice);
    }
    return result;
}


} //namespace holor

#endif // HOLOR_OPERATIONS_H
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
template <size_t D, HolorType Destination, HolorType Slice, class Op> requires ((D < Destination::dimensions) && (Slice::dimensions==Destination::dimensions-1) && (std::is_same_v<typename Destination::value_type, typename Slice::value_type>) && assert::Binaryfunction<typename Destination::value_type, typename Destination::value_type, typename Slice::value_type, Op>)
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
template <HolorType Destination, class ElementType, class Op> requires ( (std::is_same_v<typename Destination::value_type, ElementType>) && assert::Binaryfunction<typename Destination::value_type, typename Destination::value_type, ElementType, Op>)
void broadcast_all(Destination& dest, ElementType element, Op&& operation ){
    for(auto& e : dest){
        e = std::invoke(std::forward<Op>(operation), e, element);
    }
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    REDUCE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*!
 * \brief The `reduce_all` function is an operation that takes an holor and reduces it to a single scalar value obtained by applying a binary function to all the elements of the container.
 * \tparam Source is the Holor container to be reduced
 * \tparam ElementType is the type of the result
 * \tparam OP is the binary function that is applied to the elements
 * \param source is the holor that is reduced
 * \param result is the initial value from which the result is computed
 * \param operation is the function that is applied to the pairs of elements
 */
template <HolorType Source, class ElementType, class Op> requires ( (std::is_same_v<typename Source::value_type, ElementType>) && assert::Binaryfunction<ElementType, typename Source::value_type, ElementType, Op> )
auto reduce_all(Source source, ElementType result, Op&& operation ){
    for(auto& e : source){
        result = std::invoke(std::forward<Op>(operation), e, result);
    }
    return result;
}

/*!
 * \brief The `reduce` function is an operation that takes an holor and reduces it to a slice value obtained by applying a binary function to all the elements of the container.
 * \tparam D is the dimension of the source holor along which it is reduced
 * \tparam Source is the Holor container to be reduced
 * \tparam InitHolor is the type of the result
 * \tparam OP is the binary function that is applied to the elements
 * \param source is the holor that is reduced
 * \param result is the initial value from which the result is computed
 * \param operation is the function that is applied to the pairs of elements
 */
template <size_t D, HolorType Source, HolorType InitHolor, class Op> requires ((D < Source::dimensions) && (InitHolor::dimensions==Source::dimensions-1) && (std::is_same_v<typename Source::value_type, typename InitHolor::value_type>) && assert::Binaryfunction<typename Source::value_type, typename Source::value_type, typename InitHolor::value_type, Op>)
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
template <HolorType Destination, class Op> requires assert::Unaryfunction<typename Destination::value_type, typename Destination::value_type, Op>
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
    /*!
     *\brief helper function to verify that the arguments of the concatenate function have consistent dimensions and lengths
     */
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

    /*!
     *\brief helper function to verify that the arguments of the concatenate function have consistent type
     */
    template<typename T, DecaysToHolorType First_Arg>
    void check_type(First_Arg arg){
        static_assert(std::is_same_v<T, typename std::decay_t<First_Arg>::value_type>, "The arguments of the concatenation have inconsistent value_type");
    }

    template<typename T, DecaysToHolorType First_Arg, DecaysToHolorType... Args>
    void check_type(First_Arg arg, Args&&... args){
        static_assert(std::is_same_v<T, typename std::decay_t<First_Arg>::value_type>, "The arguments of the concatenation have inconsistent value_type");
        check_type<T>(std::forward<Args>(args)...);
    }

    /*!
     *\brief helper function to verify that the arguments of the concatenate function are consistent and to initialize the result Holor
     */
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

    /*!
     *\brief helper function to iterate through all the arguments and concatenate them
     */
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

/*!
 * \brief function that takes as input a sequence of Holor containers all with the same number of dimensions, lengths and value type, and concatenates them along a dimension
 * \tparam Dim is the direction where the Holors are concatenated. It must be `0 <= Dim < N` where `N` is the number of dimensions of the input Holors.
 * \param args the Holors to be concatenated passed as a parameter pack
 * \return a new Holor that concatenates all the input ones
 */
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
 * \param source is the holor that is shifted
 * \param n indicates how many places the content should be shifted
 * \return a new Holor that is equal to the original one but shifted
 */
template <size_t Dim, HolorType Source> requires (Dim<Source::dimensions)
auto shift(Source source, int n){
    int length = source.length(Dim);
    Holor<typename Source::value_type, Source::dimensions> result(source.layout());
    for (int i = 0; i < length; i++){
        auto source_slice = source.template slice<Dim>(i);
        int shift = (i+n)%length;
        if (shift<0){
            shift += length;
        }
        auto result_slice = result.template slice<Dim>(shift);
        result_slice.substitute(source_slice);
    }
    return result;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                CIRCULAR SLICE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// //NOTE: this function is a temporary workaround to the implementation of more general Holors that i) allow circular access to their elements (using a circular layout), and ii) can be indexed by passing a list of indices
// //WIP <><><><><><><><><><><><><><><><><><><><><><>
// /*!
//  * \brief The `shift` function is an operation that shifts the content of a Holor along a certain direction
//  * \param source is the holor that is sliced circularly
//  * \param n indicates how many places the content should be shifted
//  * \return a new Holor that is equal to the original one but shifted
//  */
// template <HolorType Source, class ContainerOffsets, class ContainerLengths> requires ((assert::TypedContainer<ContainerOffsets, size_t>) && (assert::TypedContainer<ContainerLengths, size_t>))
// auto circular_slice(Source source, ContainerOffsets offsets, ContainerLengths lengths){
//     Holor<typename Source::value_type, Source::dimensions> result(Layout<Source::dimensions>(lengths));
//     for (int i = 0; i < lengths; i++){
//         auto source_slice = source.template slice<Dim>(i);
//         int shift = (i+n)%lengths;
//         if (shift<0){
//             shift += lengths;
//         }
//         auto result_slice = result.template slice<Dim>(shift);
//         result_slice.substitute(source_slice);
//     }
//     return result;
// }



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    PERMUTATION
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*!
 * \brief The `permutation` and `permutation_pair` functions implement an operation that swaps selected components of a Holor container along a direction. The function does not modify the original Holor, but returns a new one.
 * \tparam Dim is the direction along which the components to be swapped are selected (e.g. when Dim = 0 the permutation is done on rows, when Dim = 1 the permutation is done on columns, etc.)
 * \param source is the holor which is permuted.
 * \param n1 is the index of the first component to be swapped in a pairwise permutation
 * \param n2 is the index of the second component to be swapped in a pairwise permutation
 * \param order is a container with the indices of the components along the selected dimension. For example {3, 0, 2, 1} means that the result of trhe permutation is a new Holor that along the selected dimension presents the components of the initial Holor taken in the order of these indices.
 * \return a new Holor that is the result of the permutation operation
 */
template <size_t Dim, HolorType Source, class Container> requires (assert::TypedContainer<Container, size_t> && (Dim < Source::dimensions))
auto permutation(Source& source, Container order){
    Holor<typename Source::value_type, Source::dimensions> result(source);
    assert::dynamic_assert(order.size() == source.length(Dim), EXCEPTION_MESSAGE("The indices of the permutation do not match the length of the container!"));
    for (auto i =0; i < source.length(Dim); i++){
        auto result_slice = result.template slice<Dim>(i);  
        result_slice.substitute(source.template slice<Dim>(order[i]));
    }
    return result;
}

template <size_t Dim, HolorType Source> requires (Dim < Source::dimensions)
auto permutation_pair(Source& source, size_t n1, size_t n2){
    Holor<typename Source::value_type, Source::dimensions> result(source);
    auto result_slice1 = result.template slice<Dim>(n1);
    auto result_slice2 = result.template slice<Dim>(n2);
    result_slice1.substitute(source.template slice<Dim>(n2));
    result_slice2.substitute(source.template slice<Dim>(n1));
    return result;
}


} //namespace holor

#endif // HOLOR_OPERATIONS_H
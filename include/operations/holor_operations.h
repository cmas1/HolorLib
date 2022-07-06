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

namespace holor{


/*================================================================================================
                                    Holor Operations
================================================================================================*/

//TODO: add requires clauses to constrain the OPs to be a binary/unary function (depending on the use case)

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
                    CONCATENATE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//WIP: this should be restricted to holors that all have the same number of dimensions (here N), and the concatenation dimension should be a value in [0, N)
//TODO: CONCATENATE? takes a pack of containers with the same dimensions and outputs a new container by applying an op on the pairs of elements
namespace impl{
    template <DecaysToHolorType Arg1, DecaysToHolorType Arg2>
    constexpr size_t unroll_min_dim(){
        return std::min(std::decay_t<Arg1>::dimensions, std::decay_t<Arg2>::dimensions);
    }

    template <DecaysToHolorType Arg1, DecaysToHolorType... Args> requires (sizeof...(Args)>=2)
    constexpr size_t unroll_min_dim(){
        return std::min(std::decay_t<Arg1>::dimensions, unroll_min_dim<Args...>());
    }

    template <DecaysToHolorType... Args>
    constexpr size_t min_dim(){
        return unroll_min_dim<Args...>();
    }  
    
    template <DecaysToHolorType Arg1, DecaysToHolorType Arg2>
    constexpr size_t unroll_max_dim(){
        return std::max(std::decay_t<Arg1>::dimensions, std::decay_t<Arg2>::dimensions);
    }

    template <DecaysToHolorType Arg1, DecaysToHolorType... Args> requires (sizeof...(Args)>=2)
    constexpr size_t unroll_max_dim(){
        return std::max(std::decay_t<Arg1>::dimensions, unroll_max_dim<Args...>());
    }

    template <DecaysToHolorType... Args>
    constexpr size_t max_dim(){
        return unroll_max_dim<Args...>();
    }



    template <size_t Dim, assert::IterableContainer Lenghts, DecaysToHolorType Arg1, DecaysToHolorType... Args> requires (sizeof...(Args)>=1)
    void concatenation_lengths(Lenghts& lengths, Arg1 h1, Args&&... args){
        auto h1_lengths = h1.lengths();
        std::copy(h1_lengths.begin(), h1_lengths.end(), lengths.begin());
        if (std::decay_t<Arg1>::dimensions < lengths.size()){
            lengths.back()=1;
        }
    }

    // template <size_t Dim, DecaysToHolorType Arg1, DecaysToHolorType Arg2>
    // auto simple_concat(Arg1 h1){

    //     return h1;
    // }

    // template <size_t Dim, DecaysToHolorType Arg1, DecaysToHolorType... Args> requires (sizeof...(Args)>=1)
    // auto unroll_concat(Arg1 h1, Args&&... args){
        
    //     if constexpr(Dim<std::decay_t<Arg1>::dimensions){
    //         auto lengths = (h1.slice<Dim>(0)).lengths();
    //     } else{
    //         auto lengths
    //     }

    // }
}


template <size_t Dim, DecaysToHolorType... Args> requires (sizeof...(Args)>=2)
auto concat(Args&&... args){
    constexpr auto min_dim = impl::min_dim<Args...>();
    constexpr auto max_dim = impl::max_dim<Args...>();
    static_assert(max_dim <= min_dim+1, "Invalid dimensions of the Holors to be concatenated");
    static_assert(Dim <= min_dim, "Invalid concatenation dimension");
    std::array<size_t,std::max(max_dim, Dim+1)> concat_lengths;
    impl::concatenation_lengths<Dim>(concat_lengths, std::forward<Args>(args)...);
    //1) compute the lenght of the slice
    //2) compute the length of the concatenation dimension
    //3) recursive process the various holors
        //3a) check lengths and concatenate
        //3b) iterate
    return impl::min_dim<Args...>(); //impl::start_unroll_concat<Dim>(lengths, std::forward<Args>(args)...);
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
                    CIRCULATE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//TODO: CIRCULATE applies an op to all elements in the container, modifying it
//Si tratta solo di fare uno shift dei dati a seconda degli strides






} //namespace holor

#endif // HOLOR_OPERATIONS_H
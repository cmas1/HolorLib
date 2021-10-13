// This file is part of Holor, a C++ template library for multi-dimensional containers

// Copyright 2020 Carlo Masone

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


#ifndef HOLOR_COMPARISON_H
#define HOLOR_COMPARISON_H


#include "holor.h"
#include "holor_ref.h"
#include <concepts>
#include <algorithm>
#include <iostream>


using namespace holor; 

/*!
 * \brief Equality comparison between two Holor containers. Two Holor containers of the same dimension and type of elements are considered to be the same if they have the same layout and their elements have the same values.
 * \tparam `T` is the type of the elements in the containers. `T` must be a type that supports an equality comparison
 * \tparam `N` is the dimensionality of the Holor containers.
 * \param h1 is the lhs in the comparison
 * \param h2 is the rhs in the comparison
 * \return true if the two Holors are equal, false otherwise
 */
template<typename T, size_t N> requires std::equality_comparable<T>
bool operator==(const Holor<T,N>& h1, const Holor<T,N>& h2){
    return ( (h1.layout()==h2.layout()) && std::ranges::equal(h1.cbegin(), h1.cend(), h2.cbegin(), h2.cend()) );
}


/*!
 * \brief Inequality comparison between two Holor containers. Two Holor containers of the same dimension and type of elements are considered to be the same if they have the same layout and their elements have the same values.
 * \tparam `T` is the type of the elements in the containers. `T` must be a type that supports an equality comparison
 * \tparam `N` is the dimensionality of the Holor containers.
 * \param h1 is the lhs in the comparison
 * \param h2 is the rhs in the comparison
 * \return true if the two Holors are not equal, false otherwise
 */
template<typename T, size_t N> requires std::equality_comparable<T>
bool operator!=(const Holor<T,N>& h1, const Holor<T,N>& h2){
    return !( h1==h2 );
}


/*!
 * \brief Equality comparison between two HolorRef containers. Two HolorRef containers of the same dimension and type of elements are considered to be the same if they have the same lengths and their elements have the same values.
 * \tparam `T` is the type of the elements in the containers. `T` must be a type that supports an equality comparison
 * \tparam `N` is the dimensionality of the Holor containers.
 * \param h1 is the lhs in the comparison
 * \param h2 is the rhs in the comparison
 * \return true if the two HolorsRefs are equal, false otherwise
 */
template<typename T, size_t N> requires std::equality_comparable<T>
bool operator==(const HolorRef<T,N>& h1, const HolorRef<T,N>& h2){
    return ( (h1.lengths()==h2.lengths()) && std::ranges::equal(h1.cbegin(), h1.cend(), h2.cbegin(), h2.cend()) );
}


/*!
 * \brief Inequality comparison between two HolorRef containers. Two HolorRef containers of the same dimension and type of elements are considered to be the same if they have the same lengths and their elements have the same values.
 * \tparam `T` is the type of the elements in the containers. `T` must be a type that supports an equality comparison
 * \tparam `N` is the dimensionality of the Holor containers.
 * \param h1 is the lhs in the comparison
 * \param h2 is the rhs in the comparison
 * \return true if the two HolorsRefs are not equal, false otherwise
 */
template<typename T, size_t N> requires std::equality_comparable<T>
bool operator!=(const HolorRef<T,N>& h1, const HolorRef<T,N>& h2){
    return !( h1==h2 );
}


/*!
 * \brief Equality comparison between a Holor and a HolorRef containers. The two containers must have the same dimension and type of elements. The containers are considered equal if they have the same lengths and elements.
 * \tparam `T` is the type of the elements in the containers. `T` must be a type that supports an equality comparison
 * \tparam `N` is the dimensionality of the Holor containers.
 * \param h1 is the lhs in the comparison
 * \param h2 is the rhs in the comparison
 * \return true if the two containers are equal, false otherwise
 */
template<typename T, size_t N> requires std::equality_comparable<T>
bool operator==(const Holor<T,N>& h1, const HolorRef<T,N>& h2){
    return ( (h1.lengths()==h2.lengths()) && std::ranges::equal(h1.cbegin(), h1.cend(), h2.cbegin(), h2.cend()) );
}



/*!
 * \brief Inquality comparison between a Holor and a HolorRef containers. The two containers must have the same dimension and type of elements. The containers are considered equal if they have the same lengths and elements.
 * \tparam `T` is the type of the elements in the containers. `T` must be a type that supports an equality comparison
 * \tparam `N` is the dimensionality of the Holor containers.
 * \param h1 is the lhs in the comparison
 * \param h2 is the rhs in the comparison
 * \return true if the two containes are not equal, false otherwise
 */
template<typename T, size_t N> requires std::equality_comparable<T>
bool operator!=(const Holor<T,N>& h1, const HolorRef<T,N>& h2){
    return !( h1==h2 );
}


/*!
 * \brief Equality comparison between a Holor and a HolorRef containers. The two containers must have the same dimension and type of elements. The containers are considered equal if they have the same lengths and elements.
 * \tparam `T` is the type of the elements in the containers. `T` must be a type that supports an equality comparison
 * \tparam `N` is the dimensionality of the Holor containers.
 * \param h1 is the lhs in the comparison
 * \param h2 is the rhs in the comparison
 * \return true if the two HolorsRefs are equal, false otherwise
 */
template<typename T, size_t N> requires std::equality_comparable<T>
bool operator==(const HolorRef<T,N>& h1, const Holor<T,N>& h2){
    return ( (h1.lengths()==h2.lengths()) && std::ranges::equal(h1.cbegin(), h1.cend(), h2.cbegin(), h2.cend()) );
}


/*!
 * \brief Inequality comparison between a Holor and a HolorRef containers. The two containers must have the same dimension and type of elements. The containers are considered equal if they have the same lengths and elements.
 * \tparam `T` is the type of the elements in the containers. `T` must be a type that supports an equality comparison
 * \tparam `N` is the dimensionality of the Holor containers.
 * \param h1 is the lhs in the comparison
 * \param h2 is the rhs in the comparison
 * \return true if the two containers are not equal, false otherwise
 */
template<typename T, size_t N> requires std::equality_comparable<T>
bool operator!=(const HolorRef<T,N>& h1, const Holor<T,N>& h2){
    return !( h1==h2 );
}


#endif // HOLOR_COMPARISON_H
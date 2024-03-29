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



#ifndef HOLOR_PRINTER_H
#define HOLOR_PRINTER_H

#include "holor.h"
#include "holor_ref.h"
#include "../common/static_assertions.h"

#include <iostream>



namespace holor{


namespace impl{


    //TODO: use a proper concept for HolorType
    /*!
     * \brief Functor that is used to help printing the content of a Holor or HolorRef container in a readable
     */
    template<typename HolorType> 
    struct holor_printer{
        std::ostream& operator()(std::ostream& os, HolorType h){
            os << " [";
            if constexpr(HolorType::dimensions == 1){
                for (auto i = 0; i<h.lengths()[0]-1; i++){
                    os << h(i) << ", ";
                }
                os << h(h.size()-1);
            }else{
                for (auto i = 0; i<h.lengths()[0]; i++){
                    holor_printer<std::remove_cvref_t<decltype(h.row(i))>>()(os, h.row(i));
                }
            }
            os << "] ";
            return os;
        }
    };


} //namespace impl



/*!
 * \brief operator to print the content of a Holor on a ostream
 * \tparam `T` is the type of the data contained in the Holor.
 * \b Note: `T` is required to be a printable data type
 * \tparam `N` is the number of dimensions of the container
 * \param os is the reference to the ostream
 * \param h is the container to be printed
 * \return a reference to the ostream
 */
template<typename T, size_t N> requires (assert::Printable<T>)
std::ostream& operator<<(std::ostream& os, const Holor<T,N>& h){
    return impl::holor_printer<std::remove_cvref_t<decltype(h)>>()(os, h);
}


/*!
 * \brief operator to print the content of a Holor on a ostream
 * \tparam `T` is the type of the data contained in the Holor.
 * \b Note: `T` is required to be a printable data type
 * \tparam `N` is the number of dimensions of the container
 * \param os is the reference to the ostream
 * \param h is container to be printed
 * \return a reference to the ostream
 */
template<typename T, size_t N> requires (assert::Printable<T>)
std::ostream& operator<<(std::ostream& os, const HolorRef<T,N>& h){
    return impl::holor_printer<std::remove_cvref_t<decltype(h)>>()(os, h);
}

} //namespace holor

#endif // HOLOR_PRINTER_H
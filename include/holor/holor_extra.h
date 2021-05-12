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



#ifndef HOLOR_EXTRA_H
#define HOLOR_EXTRA_H

#include "holor.h"
#include "holor_ref.h"


namespace holor{


namespace impl{
    template<typename HolorType> //TODO: requires concept HolorType
    struct holor_printer{
        std::ostream& operator()(std::ostream& os, const HolorType& h){
            os << "["
            if constexpr(HolorType::dimensions == 1){
                for (auto i = 0; i<h.size()-1; i++){
                    os << h(i) << ", ";
                }
                os << h(h.size()-1);
            }else{
                for (auto i = 0; i<h.size(); i++){
                    holor_printer()(os, h.row(i));
                }
            }
            os << "]";
        }
    };
}


/*!
 * \brief operator to print the content of a Holor on a ostream
 * \tparam `T` is the type of the data contained in the Holor.
 * \b Note: `T` is required to be a printable data type
 * \tparam `N` is the number of dimensions of the container
 * \param os is the reference to the ostream
 * \param h is the container to be printed
 * \return a reference to the ostream
 */
template<typename T, size_t N> //TODO: requires printable T
std::ostream& operator<<(std::ostream& os, const Holor<T,N>& h){
    return impl::holor_printer()(os, h);
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
template<typename T, size_t N> //TODO: requires printable T
std::ostream& operator<<(std::ostream& os, const HolorRef<T,N>& h){
    return impl::holor_printer()(os, h);
}

} //namespace holor

#endif // HOLOR_EXTRA_H
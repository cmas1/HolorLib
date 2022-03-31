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



#ifndef HOLOR_UTILS_H
#define HOLOR_UTILS_H

/** \file utils.h
 * \brief Common utilities.
 *
 * This header contains common utilities of the holor library.
 */


#include <utility>
#include <chrono>


namespace holor{
namespace utils{


/*!
* \brief Measure the time elapsed by a function, averaging it over multiple repetitions.
* \b Example:
* \verbatim embed:rst:leading-asterisk
*  .. code:
*      //various includes ...
*
*      using namespace holor;
*      Holor<int, 2> holor2D{ {1, 2, 3}, {4, 5, 5} };
*      std::cout << "average elapsed time = " << utils::elapsed_time_ms(100000, ([&holor2D](){holor2D(1,1);})) << "ms" << std::endl;
* \endverbatim
* \param n_times number of repetitions used to compute the average elapsed time
* \param func is the function object whose duration is to be measured
* \param args is the pack of argument to be passed to the function, i.e. to invoke `func(args...);`
* \return the the average elapsed time measured in microseconds
*/
auto elapsed_time_ms = [](int n_times, auto&& func, auto&&... args){
    std::chrono::duration<double, std::milli>  average_duration{0};
    for (int i = 0; i < n_times; i++){
        auto t1 = std::chrono::high_resolution_clock::now();
        std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...);
        average_duration += std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(std::chrono::high_resolution_clock::now() - t1);
    }
    return average_duration.count()/n_times;
};


/*!
* \brief Measure the time elapsed by a function, averaging it over multiple repetitions
* \b Example:
* \verbatim embed:rst:leading-asterisk
*  .. code:
*      //various includes ...
*
*      using namespace holor;
*      Holor<int, 2> holor2D{ {1, 2, 3}, {4, 5, 5} };
*      std::cout << "average elapsed time = " << utils::elapsed_time_ns(100000, ([&holor2D](){holor2D(1,1);})) << "ns" << std::endl;
* \endverbatim
* \param n_times number of repetitions used to compute the average elapsed time
* \param func is the function object whose duration is to be measured
* \param args is the pack of argument to be passed to the function, i.e. to invoke `func(args...);`
* \return the the average elapsed time measured in nanoseconds
*/
auto elapsed_time_ns = [](int n_times, auto&& func, auto&&... args){
    std::chrono::duration<double, std::nano>  average_duration{0};
    for (int i = 0; i < n_times; i++){
        auto t1 = std::chrono::high_resolution_clock::now();
        std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...);
        average_duration += std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(std::chrono::high_resolution_clock::now() - t1);
    }
    return average_duration.count()/n_times;
};


} //namespace utils
} //namespace holor


#endif // HOLOR_UTILS_H
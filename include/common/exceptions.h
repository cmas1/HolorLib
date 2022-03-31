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



#ifndef HOLOR_EXCEPTIONS_H
#define HOLOR_EXCEPTIONS_H

/** \file exceptions.h
 * \brief This header contains exception-related utils.
 */

#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>



namespace holor{

namespace exception{


/*!
 * \brief Type of exceptions thrown when an invalid argument is passed to a function. It is derived from the base type \p std::invalid_argument
 */
struct HolorInvalidArgument : public std::invalid_argument {
    HolorInvalidArgument(const std::string& p) : std::invalid_argument(p){};
};

/*!
 * \brief Type of exceptions thrown when an invalid argument is passed to a function. It is derived from the base type \p std::runtime_error
 */
struct HolorRuntimeError : public std::runtime_error {
    HolorRuntimeError(const std::string& p) : std::runtime_error(p){}
};


/*!
 * \brief Function that composes the message for a thrown exception.
 * 
 * \param file name of the file where the exceptions is thrown
 * \param line line number of the file where the exception originated
 * \param info additonal information to be inserted in the message
 * 
 * \return the message as a \p std::string
 */
std::string compose_message(const char* file, int line, const std::string& info){
   std::ostringstream os("(");
   os << file << ", " << line << "): " << info;
   return os.str();
}


} //namespace exception

} //namespace holor


/*!
 * \brief macro that is used to compose an exception message that specifies the file and line number which originated the exception
 */
#define EXCEPTION_MESSAGE(msg) holor::exception::compose_message(__FILE__, __LINE__, msg)


#endif // HOLOR_EXCEPTIONS_H
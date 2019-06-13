//  ____   _____   _____   __       __   __   __   ______       _______   _____   _____   __       _____ 
//  |   \ |   __| |  _  \ |  |     |  | |  \ |  | |  ____|     |__   __| |  _  | |  _  | |  |     |   __|
//  ____/  \  \   | |_| | |  |     |  | |       | |  |__          | |    | | | | | | | | |  |      \  \  
//  |   \  _\  \  |  ___/ |  |___  |  | |  |\   | |  |___         | |    | |_| | | |_| | |  |___   _\  \ 
//  ____/ |_____| |__|    |______| |__| |__| \__| |______|        |_|    |_____| |_____| |______| |_____|

// This file is part of BSTools, a lightweight C++ template library that implements B-Splines

// Copyright 2019 Carlo Masone

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.



#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

/** \file exceptions.h
 * \brief Exceptions.
 *
 * This header contains exception-related utils used within BSTools.
 */

#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>



namespace bst{

namespace exception{


/// BstInvalidArgument struct
/*!
 * Type of exceptions thrown when an invalid argument is passed to a function. It is derived from the base type \p std::invalid_argument
 */
struct BstInvalidArgument : public std::invalid_argument {
    BstInvalidArgument(const std::string& p) : std::invalid_argument(p){};
};


/// BstRuntimeError struct
/*!
 * Type of exceptions thrown when an invalid argument is passed to a function. It is derived from the base type \p std::runtime_error
 */
struct BstRuntimeError : public std::runtime_error {
    BstRuntimeError(const std::string& p) : std::runtime_error(p){}
};


/*!
* Function that composes the message for a thrown exception.
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

} //namespace bst

#define EXCEPTION_MESSAGE(msg) bst::exception::compose_message(__FILE__, __LINE__, msg)


#endif // EXCEPTIONS_H
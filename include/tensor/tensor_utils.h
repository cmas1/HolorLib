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



#ifndef TENSOR_UTILS_H
#define TENSOR_UTILS_H


#include "tensor_slice.h"




namespace bst{
namespace tensor_impl{

    template<size_t M>
    struct slice_dim{
        template<size_t N>
        enable_if_t<M<N, TensorSlice<N-1>> operator()(size_t n, TensorSlice<N> in){
            dynamic_assert<assertion_level(AssertionLevel::internal), bst::exception::BstInvalidArgument>( n < in.lengths_[M], \
                EXCEPTION_MESSAGE("slice_dim error: n is greater than the length in the dimension M") );
            TensorSlice<N-1> res;
            size_t i = 0;
            for(size_t j = 0; j < N; j++){
                res.size_ = 1;
                res.offset_ = 0;
                if (j != M){
                    res.lengths_[i] = in.lengths_[j];
                    res.strides_[i] = in.strides_[j];
                    res.size_ *= in.lengths_[j];
                    i++;
                }
                res.offset_ = in.offset_ + n*in.strides_[M];
            }
            return res;
        }
    };



} //namespace tensor_impl
} //namespace bst

#endif // TENSOR_UTILS_H
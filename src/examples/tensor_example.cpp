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


#include "../../include/tensor/tensor.h"
#include "../../include/tensor/tensor_ref.h"
#include "../../include/tensor/tensor_slice.h"
#include "../../include/tensor/tensor_initializer.h"
#include <iostream>
#include <chrono>
#include <utility>

#include "../../include/utils/static_assert.h"

using namespace bst;


int main(){
    // auto sss = TensorSlice<3>(12, {2,2,3}, {9,3,1});
    // std::cout << sss(1,0,1) << std::endl;


    // // speed test
    // int id = 1;
    // auto start = std::chrono::system_clock::now();
    // for(int i = 0; i<100000000; i++){
    //     sss(id,id,id);
    // }
    // auto end = std::chrono::system_clock::now();
    // std::chrono::duration<double> Dt = end-start;
    // std::cout << Dt.count() << ", " << Dt.count()/100000000 << std::endl;


    Tensor<double, 2> t1{{1.0, 2.0, 3}, {4, 5, 6}};
    std::cout << t1 << std::endl;



    // // speed test
    // auto start = std::chrono::system_clock::now();
    // for(int i = 0; i<1000000; i++){
    //     t1(1, 1);
    // }
    // auto end = std::chrono::system_clock::now();
    // std::chrono::duration<double> Dt = end-start;
    // std::cout << Dt.count() << ", " << Dt.count()/1000000 << std::endl;

    // // speed test
    // start = std::chrono::system_clock::now();
    // for(int i = 0; i<1000000; i++){
    //     t1.at(1, 1);
    // }
    // end = std::chrono::system_clock::now();
    // Dt = end-start;
    // std::cout << Dt.count() << ", " << Dt.count()/1000000 << std::endl;



    std::cout << "t1(x,y) = " << t1(1, 1) << std::endl;
    std::cout << "t1.at(x,y) = " << t1.at(1, 1) << std::endl;

    t1(1,1) = 99;
    std::cout << "t1(x,y) = " << t1(1, 1) << std::endl;
    std::cout << t1 << std::endl;
    
    Tensor<double, 2> t2(t1);
    std::cout << "t2 = " << t2 << std::endl;

    Tensor<double, 2> t3(3,3);
    std::cout << "t3 = " << t3 << std::endl;

    TensorSlice<2>s4 (0, {2,3}, {3,1});
    vector<double> data{0, 1, 2, 3, 4, 5};
    TensorRef<double,2> tr4(s4, data.data());
    std::cout << "tr4 = " << tr4 << std::endl;
    Tensor<double,2> t4(tr4);
    std::cout << "t4 = " << t4 << std::endl;

    auto tr5 = t4.row(1);
    std::cout << "tr5 = t4.row(1) = [" << tr5(0) << ", " << tr5(1) << ", " << tr5(2) << "]" << std::endl;
    tr5(1) = 99;
    std::cout << "t4 = " << t4 << std::endl;

    auto tr6 = t4.col(2);
    std::cout << "tr6 = t4.col(2) = [" << tr6(0) << ", " << tr6(1) << "]" << std::endl;
    Tensor<double, 1> t7(tr6);
    std::cout << "Tensor<double,1>(tr6) t7 => " << t7 << std::endl;
    Tensor<double, 1> t8 = tr6;
    std::cout << "Tensor<double,1> t8 = tr6 => " << t8 << std::endl;

    t8.print_details("Tensor<double,1> t8 = tr6");


    Tensor<double, 1> t5{1.0, 2.0, 3};
    std::cout << t5 << std::endl;



    Tensor<double, 3> t6{{{1.0, 2.0, 3}, {4,5,6}}, {{7,8,9},{10,11,12}}};
    std::cout << "t6 = " << t6 << std::endl << std::endl;
    t6.print_details("     t6       ");
    std::cout << "t6.dim_slice<2>(0) = " << t6.dim_slice<2>(0) <<std::endl;



    std::cout << "std::is_integral<double> = " << std::is_integral<double>::value << std::endl;
    std::cout << "assert::all<std::is_integral<double>> = " << assert::all(std::is_integral<double>::value, std::is_integral<int>::value) << std::endl;
    std::cout << "assert::some<std::is_integral<double>> = " << assert::some(std::is_integral<double>::value, std::is_integral<int>::value) << std::endl;

    slice_request pippo{1,3};
    std::cout << "pippo: " << pippo.start_ << ", " << pippo.end_ << ", " << pippo.step_ << std::endl;

    std::cout << std::is_same_v<slice_request, decltype(std::initializer_list<size_t>{1,2,3})> << std::endl;
    std::cout << std::is_convertible_v<std::initializer_list<size_t>, slice_request> << std::endl;

    t6(1,slice_request{1,2,1}, slice_request{2,3});

    return 0;
}
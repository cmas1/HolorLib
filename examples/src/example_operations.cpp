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




#include <holor/holor_full.h>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

using namespace holor;


int main(){
    /*
     * This file shows some usage examples of the operations on Holors that are available in the library.
     * For these examples we will initialize a few Holors.
     */
    std::cout << "\n\033[33m Holor Operations\033[0m\n This file shows some usage examples of the operations on Holors that are available in the library. For these examples we will initialize a few Holors. \n\n";
    Holor<int, 2> A;
    A.set_lengths(4,3);
    std::ranges::fill(A,1);
    Holor<int, 1> B{1,2,3};
    Holor<int, 1> C{8,7,6,5};
    Holor<int,2> D {{2,3}, {2,3}, {2,3}};
    Holor<int,2> E{{4,5}, {4,5}, {4,5}};;
    std::cout << "A = " << A << "\n";
    std::cout << "B = " << B << "\n";
    std::cout << "C = " << C << "\n";
    std::cout << "D = " << D << "\n";
    std::cout << "E = " << E << "\n\n";

    
    /*
     * 1) Reduction operations. Reduction can be used to apply an operation to all elements in a Holor container and to accumulate them to a single return value
     */
    std::cout << "\n\033[33m Example 1): Reduction\033[0m\nReduction can be used to apply an operation to all elements in a Holor container and to accumulate them to a single return value\n";
    std::cout << "We can use reduction to sum all the elements in a Holor. For example  \033[32m holor::reduce_all(A, 0, std::plus{}) \033[0m returns: ";
    std::cout << "\033[32m" << holor::reduce_all(A, 0, std::plus{}) << "\033[0m\n";
    std::cout << "Similarly, we can multiply all the elements in a Holor. For example  \033[32m holor::reduce_all(B, 1, std::multiplies{})  \033[0m returns: ";
    std::cout << "\033[32m" << holor::reduce_all(B, 1, std::multiplies{}) << "\033[0m\n\n";

    std::cout << "Reduction operations can also be applied by slices. \n";
    std::cout << "We can use reduction to sum all the elements in a Holor, in a certain direction. For example:\n";
    std::cout << "\033[32m holor::reduce<0>(A, Holor<int, 1>{0,0,0}, std::plus{}) \033[0m returns: ";
    std::cout << "\033[32m" << holor::reduce<0>(A, Holor<int, 1>{0,0,0}, std::plus{}) << "\033[0m\n";
    std::cout << "\033[32m holor::reduce<1>(A, Holor<int, 1>{0,0,0,0}, std::plus{})  \033[0m returns: ";
    std::cout << "\033[32m" << holor::reduce<1>(A, Holor<int, 1>{0,0,0,0}, std::plus{}) << "\033[0m\n\n";

    
    /*
     * 2) Broadcast operations. Broadcast can broadcast to modify a Holor by applying to all its elements a binary function where the second argument is a value or a slice. In the first case the operation is broadcasted to all values, in the second case it is broadcasted to all slices in a certain direction
     */
    std::cout << "\n\033[33m Example 2): Broadcast\033[0m\nBroadcast can broadcast to modify a Holor by applying to all its elements a binary function where the second argument is a value or a slice. In the first case the operation is broadcasted to all values, in the second case it is broadcasted to all slices in a certain direction\n";
    std::cout << "We can use broadcast to multiply to arrays. For example, consider the following sequence of operations:\n";  
    std::cout << "\033[32m" << "holor::broadcast<0>(A, B, [](auto& a, auto b){return a*b;});" << "\033[0m\n";
    std::cout << "\033[32m" << "holor::broadcast<1>(A, C, std::multiplies{});" << "\033[0m\n";
    holor::broadcast<0>(A, B, [](auto& a, auto b){return a*b;});
    holor::broadcast<1>(A, C, std::multiplies{});
    std::cout << "After these operations we obtain that" << "\033[32m A : " << A << "\033[0m\n\n";

    std::cout << "Then, if we want to double all values in A we can simply do:\n";
    std::cout << "\033[32m" << "holor::broadcast_all(A, 2, std::multiplies{});" << "\033[0m\n";
    holor::broadcast_all(A, 2, std::multiplies{});
    std::cout << "After this operations we obtain that" << "\033[32m A : " << A << "\033[0m\n\n";
    
    
    /*
     * 3) Apply operation. Apply modifies an Holor by applying a function to all its elements
     */
    std::cout << "\n\033[33m Example 3): Apply\033[0m\nEarlier, we doubled all the elements in A using a broadcast_all operation, but we could have done the same using apply. For example, let's halve all the values in A using apply:\n";
    std::cout << "\033[32m" << "holor::apply(A, [](auto& a){return a/2;}); \033[0m\n";
    holor::apply(A, [](auto& a){return a/2;});
    std::cout << "After this operations we obtain that" << "\033[32m A : " << A << "\033[0m\n\n";


    /*
     * 4) Transpose operation. Transpose allows to change the order of the coordinates of a Holor container
     */
    std::cout << "\n\033[33m Example 4): Transpose\033[0m\nThe transpose function allows to change the coordinates of a Holor container, either to create a new container or to get a view (HolorRef) on an existing container.\n";
    std::cout << "For example, let's create a new Holor that is the transpose of A by doing:\n";
    std::cout << "\033[32m" << "auto Atr1 = transpose(A);" << "\033[0m\n";
    auto Atr1 = transpose(A);
    std::cout << "After this operations we obtain that" << "\033[32m Atr1 : " << Atr1 << "\033[0m\n\n";
    std::cout << "Note that in that call of the transpose function the coordinates of the original container are reversed. However we can also pass as argument a desired reordering of the coordinats. For example:\n";
    std::cout << "\033[32m" << "auto Atr2 = transpose(A, std::array<size_t,2>{1,0});" << "\033[0m\n";
    auto Atr2 = transpose(A, std::array<size_t,2>{1,0});
    std::cout << "After this operations we obtain that" << "\033[32m Atr2 : " << Atr2 << "\033[0m\n\n";

    std::cout << "Finally, in these transpose example we have generated new containers so that the original one is not modified. However, we can also generate a transposed view that can be used to modify the elements in the original Holor. Consider:\n";
    std::cout << "\033[32m" << "auto AtrV = transpose_view(A);" << "\033[0m\n";
    auto AtrV = transpose_view(A);
    std::cout << "This yelds a HolorRef " << "\033[32m AtrV : " << AtrV << "\033[0m\n";
    std::cout << "This new Holor is a view over the data owned by A. Thus, changing its elements would also modify A. For example, Let's do ";
    std::cout << "\033[32m" << "AtrV(1,2) = 1000;" << "\033[0m\n";
    AtrV(1,2) = 1000;
    std::cout<< "After this operation we have that  \033[32m A : " << A << "\033[0m\n\n";


    /*
     * 5) Shift operation. The shift function allows circularly shift the elements in a Holor in a certain direction. This function creates a new Holor, it does not modify directly the original one
     */
    std::cout << "\n\033[33m Example 5): Shift\033[0m\nT The shift function allows circularly shift the elements in a Holor in a certain direction. This function creates a new Holor, it does not modify directly the original one.\n";
    std::cout << "For example, we can shift the Container A. We recall that:\n";
    std::cout<< "\033[32m A : " << A << "\033[0m\n";
    std::cout << "Let's create a new container that shifts A elements to the vertically by 2:\n";
    std::cout << "\033[32m" << "auto Ashift1 = shift<0>(A,2);" << "\033[0m\n";
    auto Ashift1 = shift<0>(A,2);
    std::cout<< "This yelds \033[32m Ashift1 : " << Ashift1 << "\033[0m\n";
    std::cout << "Now let's create a new container that shifts A elements horizontally by -1:\n";
    std::cout << "\033[32m" << "auto Ashift2 = shift<1>(A,-1);" << "\033[0m\n";
    auto Ashift2 = shift<1>(A,-1);
    std::cout<< "This yelds \033[32m Ashift2 : " << Ashift2 << "\033[0m\n\n";

 

    /*
     * 6) Concatenate operation. The concatenate function takes as input a parameter pack of Holor containers all with the same value type and lengths, and concatenates them along a single direction. This direction must be one of the dimensions of the input containers.
     */
    std::cout << "\n\033[33m Example 6): Concatenate\033[0m\nThe concatenate function takes as input a parameter pack of Holor containers all with the same value type and lengths, and concatenates them along a single direction. This direction must be one of the dimensions of the input containers.\n";
    std::cout << "To demonstrate this function, let's try and concatenate the following containers:\n";
    std::cout<< "\033[32m D : " << D << "\033[0m\n";
    std::cout<< "\033[32m E : " << E << "\033[0m\n";
    std::cout << "First, let's concatenate them verticaly by doing \033[32m auto DE_ver = concatenate<0>(D,E); \033[0m\n";
    auto DE_ver = concatenate<0>(D,E);
    std::cout<< "This yields \033[32m DE_ver : " << DE_ver << "\033[0m\n\n";
    std::cout << "Now, let's concatenate them horizontally by doing \033[32m auto DE_hor = concatenate<1>(D,E); \033[0m\n";
    auto DE_hor = concatenate<1>(D,E);
    std::cout<< "This yields \033[32m DE_hor : " << DE_hor << "\033[0m\n\n";


    return 0;
}
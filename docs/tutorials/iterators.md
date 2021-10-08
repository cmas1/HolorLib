# Iterators

Holor and HolorRef containers provide iterators that can be used in conjunction with standard algorithm functions, e.g. to copy values to the containers

``` cpp
    #include <holor/holor_full.h>
    #include <algorithm>
    using namespace holor;

    Holor<int,2> matrix{{1, 2, 3}, {4, 5, 6}};
    std::transform(matrix.begin(), matrix.end(), matrix.begin(), [](int n) -> int { return c*10; }); 
    //multiplies all elements in matrix by 10. Matrix now is ((10, 20, 30), (40, 50, 60))

    Holor<int,1> vec{111, 222};
    auto col1 = matrix.col(1);
    std::copy(vec.begin(), vec.end(), matrix.rbegin()); 
    //copies vec in reverse on the second column of matrix. Matrix now is ((10, 222, 30), (40, 111, 60))

```
# Indexing and Slicing


## Indexing
Holor and HolorRef containers allow to access specific elements by referring to them using a classic index notation.
``` cpp hl_lines="5 6 11 12"
    #include <holor/holor_full.h>
    using namespace holor;

    Holor<int,3> tensor{{{1, 2, 3}, {4, 5, 6}}, {{7, 8, 9}, {10, 11, 12}}};
    auto n = tensor(1,0,2); //n has the value 9
    tensor(0,0,0) = 1000; //now tensor has elements {{{1000, 2, 3}, {4, 5, 6}}, {{7, 8, 9}, {10, 11, 12}}}

    //the same functionality is available for HolorRef containers
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    HolorRef<int,3> tensor_ref(vec.data(), Layout<3>{2,2,3});
    auto m = tensor_ref(1,0,2); //m has the value 9
    tensor_ref(0,0,0) = 1000; //now tensor has elements {{{1000, 2, 3}, {4, 5, 6}}, {{7, 8, 9}, {10, 11, 12}}}

    std::cout<< vec[0] << "\n"; //this prints 1000

```


## Slicing
Holor and HolorRef can be sliced using the operator() similarly to the indexing, but passing ranges of values for at leas a coordinate.
``` cpp
    #include <holor/holor_full.h>
    using namespace holor;

    Holor<int,3> tensor{{{1, 2, 3}, {4, 5, 6}}, {{7, 8, 9}, {10, 11, 12}}, {{13, 14, 15}, {16, 17, 18}}};
    //tensor is a 3 x 2 x 3 matrix

    // Let's take a 2D slice corresponding to the first two rows, second column and last two aisles
    auto my_slice = tensor(range(0,1), 1, range(1,2)); //my_slice is a HolorRef view over the 2D slice of elements
    std::cout << my_slice << "\n"; //this prints ((5, 6), (11, 12))

    // Since the sice is a HolorRef, modifying its elements modifies the original holor
    my_slice(1,0) = 1000;
    std::cout << tensor << "\n"; //this now prints (((1, 2, 3), (4, 5, 6)), ((7, 8, 9), (10, 1000, 12)), ((13, 14, 15), (16, 17, 18)))

    //Since a slice is a HolorRef, we can furhter slice it
    auto my_slice2 = my_slice(1, range(0,1)); //my_slice2 is a 1D HolorRef containing the elements (1000, 12)
```

<hr style="background-color:#9999ff; opacity:0.4; width:50%">


HolorLib also provides some functions to get a slice over a specific dimension of the containers (both with Holor and HolorRef)
``` cpp
    #include <holor/holor_full.h>
    using namespace holor;

    Holor<int,3> tensor{{{1, 2, 3}, {4, 5, 6}}, {{7, 8, 9}, {10, 11, 12}}, {{13, 14, 15}, {16, 17, 18}}};

    auto row0 = tensor.row(0); //slices the first row, i.e., ((1, 2, 3), (4, 5, 6))
    auto col1 = tensor.col(1); //slices the second column, i.e., ((4, 5, 6), (10, 11, 12), (16, 17, 18))
    auto my_slice = tensor.slice<3>(2) // slices the third aisle (third dimension), i.e., ((3, 6), (9, 12), (15, 18))
```
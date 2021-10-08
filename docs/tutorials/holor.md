# Holor Basics

## Construction and assignment of Holors
A Holor container can be created from a nested list of elements
``` cpp
    #include <holor/holor_full.h>
    using namespace holor;
    Holor<int, 2> my_holor{{1, 2, 3}, {4, 5, 6}};

    std::cout << my_holor << std:endln; //prints the elements in my_holor, i.e., ((1, 2, 3), (4, 5, 6)).
```
<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 

A Holor container can be created or assigned from another Holor or HolorRef
```cpp
    #include <holor/holor_full.h>
    using namespace holor;
    Holor<int, 2> my_holor{{1, 2, 3}, {4, 5, 6}};

    Holor<int, 2> my_holor_2 = my_holor;
    Holor<int, 2> my_holor_3{my_holor};

    std::cout << my_holor_2 << std:endln; //prints the elements in my_holor_2, i.e., ((1, 2, 3), (4, 5, 6)).
    std::cout << my_holor_3 << std:endln; //prints the elements in my_holor_3, i.e., ((1, 2, 3), (4, 5, 6)).
```
<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 


A Holor container can be created by specifying the length of its dimensions with a  standard container. In this case its elements are not initialized.
```cpp
    #include <holor/holor_full.h>
    using namespace holor;

    std::vector<int> my_lengths1{3,6,2};
    Holor<float, 3> empty_holor1(my_lengths1);

    std::array<int,4> my_lengths2{3,1,6,2};
    Holor<float, 4> empty_holor2(my_lengths2);
```

<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 
An empty holor can be constructed without specifying its elements or dimensions.

``` cpp
    #include <holor/holor_full.h>
    using namespace holor;

    struct my_object{
        float a;
        int b;
        double c;
    };

    Holor<my_object, 3> empty_holor;
    std::cout << "  \033[32m Holor<my_object, 3> empty_holor_03; \033[0m (my_object is a generic type, for example a struct) \n";
    std::cout << "  The lenghts of \033[1m empty_holor_03 \033[0m  along its dimensions are: [";
    for(auto cnt = 0; cnt<3; cnt++){
        std::cout<< "  " << empty_holor_03.lengths(cnt);
    }
    std::cout << " ]\n\n";
```

## Checking the number of elements
``` cpp
```
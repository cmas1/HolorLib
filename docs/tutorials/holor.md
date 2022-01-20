# Holor Basics

## Construction and assignment of Holors
A Holor container can be created from a nested list of elements
``` cpp hl_lines="3"
    #include <holor/holor_full.h>
    using namespace holor;
    Holor<int, 2> my_holor{{1, 2, 3}, {4, 5, 6}};

    /* This creates a 2D matrix container 
        |1   2   3|
        |4   5   6|
     */   
    std::cout << my_holor << std:endln; //prints the elements in my_holor, i.e., ((1, 2, 3), (4, 5, 6)).
```
<hr style="background-color:#9999ff; opacity:0.4; width:50%">

A Holor container can be created or assigned from another Holor or HolorRef
```cpp hl_lines="5 6"
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
```cpp hl_lines="5 8"
    #include <holor/holor_full.h>
    using namespace holor;

    std::vector<int> my_lengths1{3,6,2};
    Holor<float, 3> empty_holor1(my_lengths1);

    std::array<int,4> my_lengths2{3,1,6,2};
    Holor<float, 4> empty_holor2(my_lengths2);
```

<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 
An empty holor can be constructed without specifying its elements or dimensions.

``` cpp hl_lines="10"
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


## Resizing a Holor
A `Holor` has a fixed number of dimensions, but the number of elements along each dimension can be changed. This is useful when the number of elements needs to be modified over time, or to create an empty container and resize it later.
``` cpp
    #include <Holor/holor/holor_full.h>
    using namespace holor;

    Holor<float,2> hf; //An empty container with 2 dimensions
    hf.set_lengths(3,2); //Now hf is a 3x2 container
    std::ranges::fill(hf,1.2); //All the elements of hf are set to the value 1.2
    hf.set_lengts(0,2); //This operation modifies only the first length and sets it to 2. Now hf is a 2x2 container
```


## Using Holor's get functions
The `Holor` class has few member functions that can be used to retrieve information regarding the container or to gain direct access to its memory.
``` cpp
    #include <holor/holor_full.h>
    using namespace holor;

    Holor<float,2> hf{{1.1, -0.3}, {7.7, 9.12}}; //A 2-by-2 container of floats

    auto hf_layout = hf.layout(); //get the layout of the Holor container
    auto hf_lengths = hf.lengths(); // get the lengths of the Holor container as a std::array
    auto hf_size = hf.size(); //get the number of elements in the container, i.e. 4
    auto hf_data_vector = hf.data_vector(); // get a copy of the vector that internally stores the data
    auto hf_data = hf.data(); // get a pointer to the memory where the data is stored
    &(hf_data()+1) = 0.3; // we are manually changing the element at coordinates (0,1) in the container from -0.3 to 0.3
    
```
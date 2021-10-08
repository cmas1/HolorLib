# HolorRef Basics

## Construction and assignment of HolorRefs
Unlike Holor containers, HolorRef containers do not own the memory where the elements are stored. The purpose of HolorRef is to provide a Holor-like view over data that is owned by other objects. Therefore, HolorRef containers need to be provided with a pointer to the data in order to be constructed.
``` cpp hl_lines="5 6 7"
    #include <holor/holor_full.h>
    using namespace holor;
    std::vector<int> my_sequence{1,2,3,4,5,6,7,8,9,10};

    HolorRef<int,2> matrix_view(my_sequence.data(), Layout<2>{2,5}); //constructor passing a layout
    HolorRef<int,2> matrix_view2(my_sequence.data(), std::vector<int>{2,5}); //same result as before, but passing a dynamic-size container of lengths
    HolorRef<int,2> matrix_view2(my_sequence.data(), std::array<int,2>{2,5}); //same result as before, but passing a fixed-size container of lengths
    /* All these constructors create a 2-by-2 HolorRef container shaped as 
        |1   2   3   4   5 |
        |6   7   8   9   10|
     */
    //matrix_view does not own its elements. Changing them changes the elements in my sequence
    matrix_view(1,0) = 99; //now we have changed the entry at coordinates (0,1) from 6 to 99
    for (auto&& x:my_sequence){
        std::cout << x << " "; 
    }
    std::cout << std::endln; //this prints the sequence 1 2 3 4 5 99 7 8 9 10
```


<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 


A HolorRef container can be created or assigned from another HolorRef
```cpp hl_lines="7 8"
    #include <holor/holor_full.h>
    using namespace holor;
    std::vector<int> my_sequence{1,2,3,4,5,6,7,8,9,10};

    HolorRef<int,2> matrix_view(my_sequence.data(), Layout<2>{2,5}); //constructor passing a layout

    HolorRef<int, 2> href_assign = matrix_view;
    Holor<int, 2> href_copy{matrix_view};
```
<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 



## Using HolorRef's get functions
The `HolorRef` class has few member functions that can be used to retrieve information regarding the container or to gain direct access to its memory.
``` cpp
    #include <holor/holor_full.h>
    using namespace holor;
    
    std::array<float,4> my_data{1.1, -0.3, 7.7, 9.12};
    HolorRef<float,2> href(my_data.data(), Layout<2>{2,2}); //create a view of the data as a 2-by-2 matrix

    auto href_layout = href.layout(); //get the layout of the Holor container
    auto href_lengths = href.lengths(); // get the lengths of the Holor container as a std::array
    auto href_size = href.size(); //get the number of elements in the container, i.e. 4
    auto href_data = href.data(); // get a pointer to the memory where the data is stored
    &(href_data()+1) = 0.3; // we are manually changing the element at coordinates (0,1) in the container from -0.3 to 0.3
    
    for (auto&& x:my_data){
        std::cout << x << " "; 
    }
    std::cout << std::endln; //this prints the sequence 1.1 0.3 7.7 9.12
```
# Holor Basics

## Constructing and assigning to Holors
``` cpp
    /*
     * 1) Construct an holor from a nested list
     * A Holor container can be built directly from a nested initializer list of elements.
     */
    Holor<int, 2> my_holor_01{{1, 2, 3}, {4, 5, 6}};

    // 2) Assign or construct from another holor
    auto my_holor_02 = my_holor_01;
    Holor<int, 2> my_holor_03{my_holor_01};

    // 3) Construct an empty holor specifying the length of its dimensions with a  standard container
    std::vector<int> length_01{3,6,2};
    Holor<float, 3> empty_holor_01(length_01);

    std::array<int,4> length_02{3,1,6,2};
    Holor<float, 4> empty_holor_02(length_02);
    


    // 4) Construct an empty holor, without specifying its elements or dimensions
    std::cout << "\n\033[33m Example 4):\033[0m  Construct an empty holor without specifying its lengths or elements\n";
    struct my_object{
        float a;
        int b;
        double c;
    };

    Holor<my_object, 3> empty_holor_03;
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
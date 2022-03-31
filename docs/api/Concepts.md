# Concepts
Defined in headers  `holor/holor_concepts.h` and `holor/layout_concepts.h` , within the `#!cpp namespace holor`.


HolorLib provide the definition of concepts that can be used to identify types that are a `Layout` or a holor container (`Holor` or `HolorRef`).


## Layout concepts
Concepts that describes a type that is a Layout (or that can decay to a Layout).
#### LayoutType 
``` cpp
    template <typename T>
    concept LayoutType
```
###### brief
This concept denotes a type that is a Layout (not a reference or pointer to a Layout).

###### example
``` cpp
    using namespace holor;
    std::cout << LayoutType<Layout<2>> << "\n"; //prints 1
    std::cout << LayoutType<Layout<2>&> << "\n"; //prints 0
```

#### DecaysToLayoutType 
``` cpp
    template <typename T>
    concept DecaysToLayoutType
```
###### brief
This concept denotes a type that decays to a LayoutType

###### example
``` cpp
    using namespace holor;
    std::cout << DecaysToLayoutType<Layout<2>> << "\n"; //prints 1
    std::cout << DecaysToLayoutType<Layout<2>&> << "\n"; //prints 1
```





## Holor concepts
Concepts that describes a type that is a Holor (or that can decay to a Holor).
#### HolorType 
``` cpp
    template <typename T>
    concept HolorType
```
###### brief
This concept denotes a type that is a Holor (not a reference or pointer to a Holor).

###### example
``` cpp
    using namespace holor;
    std::cout << HolorType<Holor<int, 2>> << "\n"; //prints 1
    std::cout << HolorType<Holor<int, 2>&> << "\n"; //prints 0
    std::cout << HolorType<HolorRef<int, 2>> << "\n"; //prints 1
    std::cout << HolorType<HolorRef<int, 2>&> << "\n"; //prints 0
```

#### DecaysToHolorType 
``` cpp
    template <typename T>
    concept DecaysToHolorType
```
###### brief
This concept denotes a type that decays to a HolorType

###### example
``` cpp
    using namespace holor;
    std::cout << DecaysToHolorType<Holor<int, 2>> << "\n"; //prints 1
    std::cout << DecaysToHolorType<Holor<int, 2>&> << "\n"; //prints 1
    std::cout << DecaysToHolorType<HolorRef<int, 2>> << "\n"; //prints 1
    std::cout << DecaysToHolorType<HolorRef<int, 2>&> << "\n"; //prints 1
```




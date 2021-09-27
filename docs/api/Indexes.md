# Index and RangeIndex
Defined in header  `holor/indexes.h`.


Holor containers are generic multi-dimensional containers. The individual elements within the container are identified by a set of indices. These indices indicate the position of the element along each axis of the container.
This index notation is core to provide the interface to access individual elements or gourp of elements stored in a Holor container.

From an abstract perspective, indices are amenable to positive natural numbers. HolorLib introduces a few concepts to describe indices and ranges of indices.



## SingleIndex
``` cpp
    template<typename T>
    concept SingleIndex = std::integral<T> && std::convertible_to<T, size_t>;
```
concept that represents a type that can be used to index a single element of a layout.



## RangeIndex
``` cpp
    template<typename T>
    concept RangeIndex = std::convertible_to<T, range>;
```
concept that represents a type that can be used to index a range of a layout.


##### range
``` cpp
    struct range{
        size_t start_; 
        size_t end_; 
        size_t step_; 
    };
```
###### brief
Structure that represents a range of indices. 

###### members
* `start_`: beginning of the range.
* `end_`: end of the range (this element is included in the range.
* `step_`: number of steps between consecutive indices.

!!! warning
    The field `step_` to select a range of spaced elements is not yet used supported.

###### constructor
``` cpp
    holor::range(size_t start, size_t end, size_t step=1);
```
!!! note
    The constructor expects that `(start>=0) && (end>start) && (step>0)` and it tould throw an exception otherwise 

###### example 
Assume to have a 1D Holor container of size 7. To select the slice that takes the elements from the second to the fourth, we can slice the holor using `index_range{1, 3}` to create the slice.
!!! note 
    `range{1, 5, 1}` is equivalent to `1:5` in Matlab. `range{1, 5, 2}` yields the indexes 1, 3, 5.






## Index
``` cpp
    template<typename T>
    concept Index = SingleIndex<T> || RangeIndex<T>;
```
concept that represents a type that can be used to index a layout.





        




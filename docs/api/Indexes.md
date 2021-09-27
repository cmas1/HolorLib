# Indexes and Ranges
Defined in header  `holor/indexes.h`.



## Ranges
``` cpp

struct range{
    size_t start_; 
    size_t end_; 
    size_t step_; 
};
```
###### brief
Structure that represents a range of indexes to slice a Holor container. 

###### members
* `start_`: beginning of the range.
* `end_`: end of the range (this element is included in the range.
* `step_`: number of steps between elements.

!!! warning
    The field `step_` to select a range of spaced elements is not yet used supported.

###### constructor
``` cpp
    range(size_t start, size_t end, size_t step=1);
```
!!! note
    The constructor now expects that `(start>=0) && (end>start) && (step>0)` and it tould throw an exception otherwise 

###### example 
Assume to have a 1D Holor container of size 7. To select the slice that takes the elements from the second to the fourth, we can slice the holor using `range{1, 3}` to create the slice.
!!! note 
    `range{1, 5, 1}` is equivalent to `1:5` in Matlab. `range{1, 5, 2}` yields the indexes 1, 3, 5.



        

## Index-related Concepts

#### SingleIndex
``` cpp
    template<typename T>
    concept SingleIndex = std::integral<T> && std::convertible_to<T, size_t>;
```
concept that represents a type that can be used to index a single element of a layout.
<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>


#### RangeIndex
``` cpp
    template<typename T>
    concept RangeIndex = std::convertible_to<T, range>;
```
concept that represents a type that can be used to index a range of a layout.
<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>


#### Index
``` cpp
    template<typename T>
    concept Index = SingleIndex<T> || RangeIndex<T>;
```
concept that represents a type that can be used to index a layout.


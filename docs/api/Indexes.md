# Indices and Ranges of Indices
Defined in header  `holor/indexes.h`, within the `#!cpp namespace holor`.


The individual elements within a Holor or HolorRef container are identified by a set of indices. These indices indicate the position of the element along each axis of the container.
This index notation is used to provide the interface to access individual elements or range of elements stored in a Holor container.
HolorLib introduces a few concepts to describe indices and ranges of indices.



## SingleIndex
This concept describes an index, a non-negative natural number that represent a position along an axis of the container. This concept is instrumental to access an individual element of the container, and it is implemented as:
``` cpp
    template<typename T>
    concept SingleIndex = std::integral<T> && std::convertible_to<T, size_t>;
```



## RangeIndex
This concept describes a range of indices, which is instrumental to access a slice of contiguous elements in the container. This concept is implemented as:
``` cpp
    template<typename T>
    concept RangeIndex = std::convertible_to<T, range>;
```
The implementation of a `RangeIndex` is based upon the `holor::range` structure.

##### holor::range
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
    The field `step_` to select a range of spaced elements is not yet supported and it may be removed.

###### constructor
``` cpp
    holor::range(size_t start, size_t end, size_t step=1);
```
!!! important "exceptions"
    The constructor throws an `holor::exception::HolorRuntimeError` if its arguments do not implement a meaningful range of indices, i.e., if they do not satisfy `(start>=0) && (end>start) && (step>0)`. The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to `AssertionLevel::no_checks` to exclude this check. Refer to [Exceptions](./Exceptions.html) for more details.

!!! note 
    `range{1, 5, 1}` is equivalent to `1:5` in Matlab. `range{1, 5, 2}` yields the set of indices 1, 3, 5.






## Index
This final concept describes a type that is either a SingleIndex or a RangeIndex and tha can be used to access an individual element of a Holor container or a slice with multiple elements. 
It is defined as:
``` cpp
    template<typename T>
    concept Index = SingleIndex<T> || RangeIndex<T>;
```





        




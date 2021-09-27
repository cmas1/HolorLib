# holor::HolorRef

Defined in header `holor/holor?ref.h`.    

``` cpp
    template<typename T, size_t N>
    class HolorRef;
```

Class implementing a general N-dimensional container that does not own the memory where the elements are stored.

A HolorRef is intended as a general `N`-dimensional container, whose elements need not to be numerical types, but can be of a generic type `T`. 
HolorRefs are implemented with a (row-major)[https://en.wikipedia.org/wiki/Row-_and_column-major_order] representation, i.e., the elements of last dimension of the container are contiguous.




<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>

## Template parameters

|Name | Description                        |
|-----|------------------------------------|
| `N` | number of dimensions of the container |
| `T` | type of the elements stored in the container |

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>


## Member types and aliases
|Name | Description                        |
|-----|------------------------------------|
| `order` | number of dimensions in the container (equal to `N`) |
| `value_type` | type of the elements in the container (equal to `T`) |
| `iterator` | type of the iterator for the container |
| `const_iterator` | type of the const_iterator for the container |
| `reverse_iterator` | type of the reverse_iterator for the container |
| `const_reverse_iterator` | type of the const_reverse_iterator for the container |


<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>


## Public Member functions

### Constructors
1. 
``` cpp
    HolorRef();
```
2. 
``` cpp
    HolorRef(const HolorRef<T, N>& holor_ref);
```
3. 
``` cpp
    HolorRef(HolorRef<T, N>&& holor_ref);
```
4. 
``` cpp
    HolorRef(T* dataptr, const Layout<N>& layout);
```
5. 
``` cpp
    template <class Container> requires assert::SizedTypedContainer<Container, size_t, N>
    explicit HolorRef(T* dataptr, const Container& lengths);
```
6. 
``` cpp
    template <class Container> requires assert::ResizeableTypedContainer<Container, size_t>
    explicit HolorRef(T* dataptr, const Container& lengths);
```

###### brief
Create a HolorRef object, either as an empty HolorRef with 0-length dimensions (1), or initializing it from another HolorRef (2, 3), or creating it from a pointer to a memory location and a Layout (4), or by creating it from a pointer to a memory location and a container of lengths.

###### parameters
* `holor_ref`:  HolorRef object used to initialize the created HolorRef. 
* `dataptr`: pointer to the memory location where the elements are stored.
* `layout`: Layout used to initialize the way coordinates are mapped to memory locations.
* `lenghts`: number of elements along each individual dimension of the HolorRef container, given either as a compile-time size container (e.g. a `std::array`) or as a dynamic size container (e.g., a `std::vector`).

###### return
A HolorRef container.

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Assignments
1. 
``` cpp
    HolorRef& operator=(HolorRef<T, N>&& holor_ref);
```
2. 
```cpp
    HolorRef& operator=(HolorRef<T, N>&& holor_ref);
```

###### brief
Assign to a HolorRef.

###### parameters
* `holor_ref`: HolorRef object to assign from.

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Iterators

#### begin
``` cpp    
    auto begin();
``` 
###### brief
Returns an iterator to the beginning.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### end
``` cpp
    auto end();
```
###### brief
Returns an iterator to the end.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### cbegin
``` cpp
    auto cbegin() const;
```
###### brief 
Returns a constant iterator to the beginning.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### cend
``` cpp
    auto cend() const;
```
###### brief
Returns a constant iterator to the end.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### rbegin
``` cpp
    auto rbegin();
```
###### brief
Returns a reverse iterator to the beginning.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### rend
``` cpp
    auto rend();
```
###### brief
Returns a reverse iterator to the end.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### crebegin
``` cpp
    auto crbegin();
```
###### brief
Returns a constant reverse iterator to the beginning.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### crend
``` cpp
    auto crend();
```
###### brief
Returns a constant reverse iterator to the end.
<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Get/Set functions

#### layout
``` cpp
    const Layout<N>& layout();
```
###### brief 
Get the Layout used by the Holor to map indices to memory locations.
###### return
a Layout.
<hr style="background-color:#9999ff; opacity:0.4; width:50%;">



#### lengths
1. 
``` cpp
    auto lengths() const;
```
2. 
``` cpp
    auto lengths(size_t dim) const;
```
###### brief
Get the lengths, i.e., the number of elements that the container has per each dimension.
###### parameters
* `dim`: request to get only the lenght for the `dim` dimension.
###### return
When called without arguments, the function returns a `std::array` with the lenght of all dimensions ofthe container. When the argument `dim` is passed, the function returns a single legnth (a `size_t`).
<hr style="background-color:#9999ff; opacity:0.4; width:50%;">



#### size
``` cpp
    size_t size() const;
```
###### brief
Get the total number of elements in the container.
###### return
Return the number of elements in the container, a `size_t`.
<hr style="background-color:#9999ff; opacity:0.4; width:50%;">



#### dataptr
1. 
``` cpp
    T* dataptr();
```
2. 
``` cpp
    const T* dataptr();
```
###### brief
Get a flat access to the memory that stores the elements contained in the container.
###### return
A pointer to the memory location where the elements are stored.

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>




### Indexing functions

#### operator()
1. 
``` cpp
    template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
    T& operator()(Dims&&... dims);
```
2. 
``` cpp
    template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
    const T operator()(Dims&&... dims) const;
```
###### brief
Access a single element in the container.
###### parameters
* `dims`: pack of indices, one per dimension of the Holor container. The coordinates must abide the `SingleIndex` concept.
###### return
The element in the Holor at the selected coordinates.


<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>




### Slicing functions

#### operator()
``` cpp
    template<typename... Args> requires (impl::ranged_index_pack<Args...>() && (sizeof...(Args)==N) )
    auto operator()(Args&&... args);
```

###### brief 
Access a slice of the container.
###### parameters
* `args`: pack of indices and ranges, one per dimension of the Holor container. At least one argument of the pack must be a range.

###### return
A slice as a HolorRef. The number of dimensions of the returned holorRef depends on the input arguments, as each SingleIndex passed in the parameters pack collapses one dimension.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">



#### row
1. 
``` cpp
    HolorRef<T, N-1> row(size_t i);
```
2. 
``` cpp
    const HolorRef<T, N-1> row(size_t i) const;
```
###### brief 
Get a single row of the container.
###### parameters
* `i`: selects the `i`-th row.
###### return
the slice corresponding to the `i`-th row. This slice is a HolorRef with `N-1` dimensions.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">



#### col
1. 
``` cpp
    HolorRef<T, N-1> col(size_t i);
```
2. 
``` cpp
    const HolorRef<T, N-1> col(size_t i) const;
```
###### brief 
Get a single column of the container.
###### parameters
* `i`: selects the `i`-th column.
###### return
The slice corresponding to the `i`-th column. This slice is a HolorRef with `N-1` dimensions.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### slice
1. 
``` cpp
    template<size_t M>
    HolorRef<T, N-1> slice(size_t i);
```
2. 
``` cpp
    template<size_t M>
    const HolorRef<T, N-1> slice(size_t i) const;
```
###### brief
Get the `i`-th slice of a single dimension (e.g., the fifth row or the second column)
###### template parameters
* `M` is the dimension to be sliced. 0 is a row, 1 is a column, ...
###### parameters
* `i` is the index of the slice to be taken along the `M`-th dimension.
###### return
The slice corresponding to the `i`-th coordinate along the `M`-th dimension. This slice is a HolorRef with `N-1` dimensions.




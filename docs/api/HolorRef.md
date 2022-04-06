# HolorRef class

Defined in header `holor/holor_ref.h`, within the `#!cpp namespace holor`.    

``` cpp
    template<typename T, size_t N> requires (N>0)
    class HolorRef;
```

This class implements a general `N`-dimensional container that doesn't own the memory where the elements are stored.
The elements in the container need not to be numerical types, but can be of a generic type `T`. 
Holors are implemented with a [row-major](https://en.wikipedia.org/wiki/Row-_and_column-major_order) representation, i.e., the elements of the last dimension of the container are contiguous.




<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>

## Template parameters

|Name | Description                        |
|-----|------------------------------------|
| `N` | number of dimensions of the container. It must be `N>0` |
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
##### signature
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

##### brief
Create a HolorRef object, either as an empty HolorRef with 0-length dimensions (1), or initializing it from another HolorRef (2, 3), or creating it from a pointer to a memory location and a Layout (4), or by creating it from a pointer to a memory location and a container of lengths.

##### parameters
* `holor_ref`:  HolorRef object used to initialize the created HolorRef. 
* `dataptr`: pointer to the memory location where the elements are stored.
* `layout`: Layout used to initialize the way coordinates are mapped to memory locations.
* `lenghts`: number of elements along each individual dimension of the HolorRef container, given either as a compile-time size container (e.g. a `std::array`) or as a dynamic size container (e.g., a `std::vector`).

##### return
A HolorRef container.

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Assignments
##### signature
1. 
``` cpp
    HolorRef& operator=(HolorRef<T, N>&& holor_ref);
```
2. 
```cpp
    HolorRef& operator=(HolorRef<T, N>&& holor_ref);
```

##### brief
Assign to a HolorRef.

##### parameters
* `holor_ref`: HolorRef object to assign from.

##### return
A reference to a HolorRef.

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Iterators

#### begin
##### signature
``` cpp    
    auto begin();
``` 
##### brief
Returns an iterator to the beginning.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### end
##### signature
``` cpp
    auto end();
```
##### brief
Returns an iterator to the end.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### cbegin
##### signature
``` cpp
    auto cbegin() const;
```
##### brief 
Returns a constant iterator to the beginning.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### cend
##### signature
``` cpp
    auto cend() const;
```
##### brief
Returns a constant iterator to the end.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### rbegin
##### signature
``` cpp
    auto rbegin();
```
##### brief
Returns a reverse iterator to the beginning.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### rend
##### signature
``` cpp
    auto rend();
```
##### brief
Returns a reverse iterator to the end.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### crebegin
##### signature
``` cpp
    auto crbegin() const;
```
##### brief
Returns a constant reverse iterator to the beginning.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">


#### crend
##### signature
``` cpp
    auto crend() const;
```
##### brief
Returns a constant reverse iterator to the end.
<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Get/Set functions

#### layout
##### signature
``` cpp
    const Layout<N>& layout();
```
##### brief 
Get the Layout used by the Holor to map indices to memory locations.
##### return
a Layout.
<hr style="background-color:#9999ff; opacity:0.4; width:50%;">

HolorRef<T, N-1>

#### lengths
##### signature
1. 
``` cpp
    auto lengths() const;
```
2. 
``` cpp
    auto lengths(size_t dim) const;
```
##### brief
Get the lengths, i.e., the number of elements that the container has per each dimension.
##### parameters
* `dim`: request to get only the lenght for the `dim` dimension. There is no check on the validity of this argument.
##### return
When called without arguments, the function returns a `std::array` with the lenght of all dimensions ofthe container. When the argument `dim` is passed, the function returns a single legnth (a `size_t`).
<hr style="background-color:#9999ff; opacity:0.4; width:50%;">



#### size
##### signature
``` cpp
    size_t size() const;
```
##### brief
Get the total number of elements in the container.
##### return
Return the number of elements in the container, a `size_t`.
<hr style="background-color:#9999ff; opacity:0.4; width:50%;">



#### data
##### signature
1. 
``` cpp
    T* data();
```
2. 
``` cpp
    const T* data();
```
##### brief
Get a flat access to the memory that stores the elements contained in the container.
##### return
A pointer to the memory location where the elements are stored.

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>




### Indexing functions

#### operator()
##### signature
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
3. 
``` cpp
    template <class Container> requires ((assert::SizedContainer<Container, N> || assert::ResizeableContainer<Container>) && SingleIndex<typename Container::value_type>)
    T& operator()(const Container& indices);
```
4. 
``` cpp
    template <class Container> requires ((assert::SizedContainer<Container, N> || assert::ResizeableContainer<Container>) && SingleIndex<typename Container::value_type>)
    const T access(const Container& indices) const;
```
##### brief
Access a single element in the container.
##### parameters
* `dims`: pack of indices, one per dimension of the Holor container, given either as a parameter pack or as an array of `SingleIndex` elements (Refer to [Indices](./Indexes.html) for more details).
* `indices`: container of `SingleIndex` elements (e.g. a std::vector or a std::array).

!!! warning
    When indexing a Holor, the conversion from indices to a memory location performed by the Layout may throw an `holor::exception::HolorRuntimeError` if the arguments are outside the admissible range for each coordinate of the Layout. The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to `AssertionLevel::no_checks` to exclude this check. Refer to [Exceptions](./Exceptions.html) for more details.

##### return
The element in the Holor at the selected coordinates.


<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>




### Slicing functions

#### operator()
##### signature
``` cpp
    template<typename... Args> requires (impl::ranged_index_pack<Args...>() && (sizeof...(Args)==N) )
    auto operator()(Args&&... args);
```

##### brief 
Access a slice of the container.
##### parameters
* `args`: parameters pack. Each element of the pack must be either a `SingleIndex` or a `RangeIndex`, and at least one of them must be a `RangeIndex`. Refer to [Indices](./Indexes.html) for more details.

!!! warning
    When slicing a Holor, the conversion from indices to a memory location performed by the Layout may throw an `holor::exception::HolorRuntimeError` if the arguments are outside the admissible range for each coordinate of the Layout. The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to `AssertionLevel::no_checks` to exclude this check. Refer to [Exceptions](./Exceptions.html) for more details.

##### return
A slice as a HolorRef. The number of dimensions of the returned holorRef depends on the input arguments, as each SingleIndex passed in the parameters pack collapses one dimension.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">



#### row
##### signature
1. 
``` cpp
    auto row(size_t i);
```
2. 
``` cpp
    const auto row(size_t i) const;
```
##### brief 
Get a single row of the container.
##### parameters
* `i`: selects the `i`-th row.

!!! warning
    When slicing a row of the Holor an `holor::exception::HolorRuntimeError` exception maybe thrown if the arguments are outside the admissible range for each coordinate of the Layout. The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to `AssertionLevel::no_checks` to exclude this check. Refer to [Exceptions](./Exceptions.html) for more details.

##### return
the slice corresponding to the `i`-th row. This slice is a HolorRef with `N-1` dimensions.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">

!!! warning
    Since the slice has `N-1` dimensions, this function is available only for Holor containers with `N>1`.


#### col
##### signature
1. 
``` cpp
    HolorRef<T, N-1> col(size_t i);
```
2. 
``` cpp
    const HolorRef<T, N-1> col(size_t i) const;
```
##### brief 
Get a single column of the container.
##### parameters
* `i`: selects the `i`-th column.

!!! warning
    When slicing a column of the Holor an `holor::exception::HolorRuntimeError` exception maybe thrown if the arguments are outside the admissible range for each coordinate of the Layout. The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to `AssertionLevel::no_checks` to exclude this check. Refer to [Exceptions](./Exceptions.html) for more details.

##### return
The slice corresponding to the `i`-th column. This slice is a HolorRef with `N-1` dimensions.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">

!!! warning
    Since the slice has `N-1` dimensions, this function is available only for Holor containers with `N>1`.

#### slice
##### signature
1. 
``` cpp
    template<size_t M> requires (M<N)
    auto slice(size_t i);
```
2. 
``` cpp
    template<size_t M> requires (M<N)
    const auto slice(size_t i) const;
```
##### brief
Get the `i`-th slice of a single dimension (e.g., the fifth row or the second column)
##### template parameters
* `M` is the dimension to be sliced. 0 is a row, 1 is a column, ...
##### parameters
* `i` is the index of the slice to be taken along the `M`-th dimension.

!!! warning
    When slicing a dimension of the Holor an `holor::exception::HolorRuntimeError` exception maybe thrown if the arguments are outside the admissible range for each coordinate of the Layout. The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to `AssertionLevel::no_checks` to exclude this check. Refer to [Exceptions](./Exceptions.html) for more details.
    
##### return
The slice corresponding to the `i`-th coordinate along the `M`-th dimension. This slice is a HolorRef with `N-1` dimensions.

!!! warning
    Since the slice has `N-1` dimensions, this function is available only for Holor containers with `N>1`. Moreover, it must be `M<N`.

## Non-Member functions

#### equality operator==
##### signature
1. 
``` cpp
    template<typename T, size_t N> requires std::equality_comparable<T>
    bool operator==(const HolorRef<T,N>& h1, const HolorRef<T,N>& h2);
```
2. 
``` cpp
    template<typename T, size_t N> requires std::equality_comparable<T>
    bool operator==(const HolorRef<T,N>& h1, const Holor<T,N>& h2);
```
3. 
``` cpp
    template<typename T, size_t N> requires std::equality_comparable<T>
    bool operator==(const Holor<T,N>& h1, const HolorRef<T,N>& h2);
```
##### brief 
Comparison operator that verifies the equality of two HolorLib containers with the same dimension `N` and type of elements `T`.

!!! note
    Two HolorRef containers or a HolorRef and a Holor container are considered equal if they have the same lengths and contain the same elements (but their layouts may be different).

##### template parameter
* `N`: the dimension of the two container to be compared.
* `T`: the type of the elements in the container.
##### parameter
* `h1`: left hand side of the comparison.
* `h2`: right hand side of the comparison.

##### return
true if the two containers are equal, false otherwise
<hr style="background-color:#9999ff; opacity:0.4; width:50%">



#### inequality operator!=
##### signature
1. 
``` cpp
    template<typename T, size_t N> requires std::equality_comparable<T>
    bool operator!=(const HolorRef<T,N>& h1, const HolorRef<T,N>& h2);
```
2. 
``` cpp
    template<typename T, size_t N> requires std::equality_comparable<T>
    bool operator!=(const HolorRef<T,N>& h1, const Holor<T,N>& h2);
```
3. 
``` cpp
    template<typename T, size_t N> requires std::equality_comparable<T>
    bool operator!=(const Holor<T,N>& h1, const HolorRef<T,N>& h2);
```
##### brief 
Comparison operator that verifies the inequality of two HolorLib containers with the same dimension `N` and type of elements `T`.

!!! note
    Two HolorRef containers or a HolorRef and a Holor container are considered equal if they have the same lengths and contain the same elements (but their layouts may be different).

##### template parameter
* `N`: the dimension of the two container to be compared.
* `T`: the type of the elements in the container.
##### parameter
* `h1`: left hand side of the comparison.
* `h2`: right hand side of the comparison.

##### return
true if the two containers are not equal, false otherwise.
# Holor class

Defined in header `holor/holor.h`, within the `#!cpp namespace holor`.       

``` cpp
    template<typename T, size_t N> requires (N>0)
    class Holor;
```

This class implements a general `N`-dimensional container that owns the memory where the elements are stored.
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
    Holor();
```
2. 
``` cpp
    Holor(const Holor<T, N>& holor);
```
3. 
``` cpp
    Holor(Holor<T, N>&& holor);
```  
4. 
``` cpp
    template <class Container> requires assert::SizedTypedContainer<Container, size_t, N>
    explicit Holor(const Container& lengths);
```
5. 
``` cpp
    template <class Container> requires assert::ResizeableTypedContainer<Container, size_t>
    explicit Holor(const Container& lengths);
```
6. 
``` cpp
    template<typename U> requires (std::convertible_to<U, T>)
    explicit Holor(const HolorRef<U,N>& ref);
```
7. 
``` cpp
    Holor(holor::nested_list<T,N> init);
```

##### brief
Create a Holor object, either as an empty holor with 0-length dimensions (1), or initializing it from another Holor (2, 3) or HolorRef (6), or providing the lenghts (number of elements) in each dimension (4, 5), or by passing as arguments a nested list with the elements (7).

##### parameters
* `holor`:  Holor object used to initialize the created Holor from. 
* `lengths`: number of elements per dimension (a container such as `#!cpp std::vector<size_t>` or `#!cpp std::array<size_t, N>`).
* `ref`: HolorRef object used to initialize the created Holor from.
* `init`: nested list of the elements to be inserted in the container.

##### return
A Holor container.

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Assignments
##### signature
1. 
``` cpp
    Holor& operator=(Holor<T, N>&& holor);
```
2. 
```cpp
    Holor& operator=(Holor<T, N>&& holor);
```
3. 
``` cpp
    Holor& operator=(holor::nested_list<T,N> init);
```

##### brief
Assign to a Holor.

##### parameters
* `holor`: Holor object to assign from.
* `init`: nested list of the elements to be assigned in the container.

##### return
A reference to the Holor container.

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
A Layout.
<hr style="background-color:#9999ff; opacity:0.4; width:50%;">



#### lengths
##### signature
``` cpp
    auto lengths() const;
```
##### brief
Get the lengths, i.e., the number of elements that the container has per each dimension.
##### return
A `std::array` with the length of all dimensions of the container.
<hr style="background-color:#9999ff; opacity:0.4; width:50%;">



#### set_lengths
##### signature
1. 
``` cpp
    template<typename... Lengths> requires ((sizeof...(Lengths)==N) && (assert::all(std::is_convertible_v<Lengths,size_t>...)) )
    void set_lengths(Lengths&&... lengths);
```
2. 
``` cpp
    template <class Container> requires assert::SizedTypedContainer<Container, size_t, N>
    void set_lengths(const Container& lengths);
```
3. 
``` cpp
    template <class Container> requires assert::ResizeableTypedContainer<Container, size_t>
    void set_lengths(const Container& lengths);
```    
##### brief
Set the lengths of the container, resizing it.

##### parameters
* `lengths`: number of elements per dimension ( either a container such as `#!cpp std::vector<size_t>` and `#!cpp std::array<size_t, N>`, or a variadic argument.).

<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 



#### length
##### signature
``` cpp
    auto length(size_t dim) const;
```
##### brief
Get the number of elements that the container has along the dimension `dim`.
##### parameters
* `dim`: the dimension queried. There is no check on the validity of this argument.
##### return
The function returns a single legnth (a `size_t`).
<hr style="background-color:#9999ff; opacity:0.4; width:50%;">



#### set_length
##### signature
``` cpp
    void set_length(size_t dim, size_t length)
```
##### brief
Change a single length of the container.

##### parameters
* `dim`: the dimension queried.
* `length`: the new value.
        
<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 



#### size
##### signature
``` cpp
    size_t size() const;
```
##### brief
Get the total number of elements in the container.
##### return
Return the number of elements in the container, as a `size_t`.
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
Get a flat access to the data contained in the container.
##### return
A pointer to the data stored in the container.
<hr style="background-color:#9999ff; opacity:0.4; width:50%;">



#### data_vector
##### signature
``` cpp
    auto data_vector() const;
```
##### brief
Get a copy of the container's data.
##### return
A `std::vector` with the data.

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
    auto col(size_t i);
```
2. 
``` cpp
    const auto col(size_t i) const;
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
    bool operator==(const Holor<T,N>& h1, const Holor<T,N>& h2);
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
    Two Holor containers are considered equal if they have the same layout and contain the same elements. A Holor and a HolorRef container are considered equal if they have the same lengths across all dimensions (but their layouts may be different) and store the same elements.

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
    bool operator!=(const Holor<T,N>& h1, const Holor<T,N>& h2);
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
    Two Holor containers are considered equal if they have the same layout and contain the same elements. A Holor and a HolorRef container are considered equal if they have the same lengths across all dimensions (but their layouts may be different) and store the same elements.

##### template parameter
* `N`: the dimension of the two container to be compared.
* `T`: the type of the elements in the container.
##### parameter
* `h1`: left hand side of the comparison.
* `h2`: right hand side of the comparison.

##### return
true if the two containers are not equal, false otherwise.

# Layout class

Defined in header `holor/layout.h`, within the `#!cpp namespace holor`.    

``` cpp
    template<size_t N> requires (N>0)
    class Layout
```



This class implements the mapping between the indices a Holor container and the locations in the memory where the elements are stored.
It uses the idea of generalized layouts from the standard library, i.e., it is based on the fact that the elements of a Holor or HolorRef
are stored as a 1D data sequence following a [row-major](https://en.wikipedia.org/wiki/Row-_and_column-major_order) representation.

A layout contains three main pieces of attributes: 

- __offset__: the offset in the contiguous memory of the first element indexed by the layout.  
- __lengths__: the numbers of elements along every dimension of the layout.
- __strides__: the distances in the 1D memory sequence between successive elements in individual dimensions of the layout.

!!! note
    For a Layout with `N` dimensions, both the length array and the stride array must be size `N`.

A Layout supports two fundamental operations:

- __Indexing__: this operation provides a map from the coordinates in the container to the index in memory of the selected element;
- __Slicing__: this operation allows to select a subset of elements from a container by computing a new Layout that provides the needed information to index them. 


<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>

## Template parameters

|Name | Description                                          |
|-----|------------------------------------------------------|
| `N` | number of dimensions in the layout. It must be `N>0` |

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>


## Member types and aliases
|Name | Description                        |
|-----|------------------------------------|
| `order` | static constexpr member equal to `N`|


<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>


## Public Member functions

### Constructors
##### signature
1. 
``` cpp
    Layout();
```
2. 
``` cpp
    Layout(const Layout<N>& layout);
```
3. 
``` cpp
    Layout(Layout<N>&& layout);
```  
4. 
``` cpp
    template <class Container> requires assert::SizedTypedContainer<Container, size_t, N>
    explicit Layout(const Container& lengths);
```
5. 
``` cpp
    template <class Container> requires assert::ResizeableTypedContainer<Container, size_t>
    explicit Layout(const Container& lengths);
```
6. 
``` cpp
    template<typename... Lengths> requires ((sizeof...(Lengths)==N) && (assert::all(std::is_convertible_v<Lengths,size_t>...)) )
    explicit Layout(Lengths&&... lengths);
```

##### brief
Create a Layout object, either as an empty layout with 0-length dimensions (1), or initializing it from another layout (2, 3), or providing the lenghts (number of elements) mapped in each dimension (4, 5, 6).

##### parameters
* `layout`:  another layout to be used to initialize the created layout.
* `lengths`: number of elements per dimension ( either a container such as `#!cpp std::vector<size_t>` and `#!cpp std::array<size_t, N>`, or a variadic argument.).


!!! warning
    When constructing a Layout from a container of `lengths`, this container must have `N` elements. This check is performed as a static assert for fixed size containers ( e.g. a `std::array`) and as a runtime assert for dynamic size containers (e.g. a `std::vector`). In this second case, the constructor would throws an `holor::exception::HolorRuntimeError` exception . The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to `AssertionLevel::no_checks` to exclude this check. Refer to [Exceptions](./Exceptions.html) for more details.

##### return
A Layout.

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>




### Assignments
##### signature
1. 
``` cpp
    Layout<N>& operator=(const Layout<N>& layout);
```
2. 
```cpp
    Layout<N>& operator=(Layout<N>&& layout);
```

##### brief
Assign to a Layout from another Layout.

##### parameters
* `layout`: another layout to assign from.

##### return
A reference to the Layout.

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Get/Set functions

#### dimensions
##### signature
``` cpp
    constexpr size_t dimensions() const;
```
##### brief 
get the number of dimensions of the layout.

##### return
the number `N` of dimensions of the layout.

 <hr style="background-color:#9999ff; opacity:0.4; width:50%;">



#### size
##### signature
``` cpp
    size_t size() const;
```
##### brief
Get the size the layout.

##### return
The total number of elements mapped by the layout.

<hr style="background-color:#9999ff; opacity:0.4; width:50%">



#### offset
##### signature
``` cpp
    size_t offset() const;
```
##### brief 
Get the offset of the layout. This is a const function.
##### return
The offset of the position of first element in the container with respect to the beginning of the memory where the elements are stored.

<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 



#### lengths
##### signature
``` cpp
    std::array<size_t,N> lengths();
```
##### brief
Get the lengths of the layout.
##### return
The lengths (number of elements per dimension) of the layout.

<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 



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
Set the lengths of the layout.

##### parameters
* `lengths`: number of elements per dimension ( either a container such as `#!cpp std::vector<size_t>` and `#!cpp std::array<size_t, N>`, or a variadic argument.).

!!! warning
    When constructing a Layout from a container of `lengths`, this container must have `N` elements. This check is performed as a static assert for fixed size containers ( e.g. a `std::array`) and as a runtime assert for dynamic size containers (e.g. a `std::vector`). In this second case, the constructor would throws an `holor::exception::HolorRuntimeError` exception . The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to `AssertionLevel::no_checks` to exclude this check. Refer to [Exceptions](./Exceptions.html) for more details.

<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 



#### length
##### signature
``` cpp
    auto length(size_t dim) const;
```
##### brief
Get a length of a dimension of the layout.

##### parameters
* `dim`: the dimension queried. There is no check to verify if the value `dim` is feasible.

##### return 
The length along a dimension (number of elements in that dimension).
        
<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 



#### set_length
##### signature
``` cpp
    void set_length(size_t dim, size_t length)
```
##### brief
Set a single length of the layout.

##### parameters
* `dim`: the dimension queried.
* `length`: the new value.

!!! warning
    When changing a length, the dimension `dim` must be a valid dimension and the new value `length` msut be strictly positive. These checks are performed as a runtime assert for dynamic size containers and the function throws an `holor::exception::HolorRuntimeError` exception if the check fails. Refer to [Exceptions](./Exceptions.html) for more details.
        
<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 



#### strides
##### signature
``` cpp
    std::array<size_t,N> strides() const;
```
##### brief
Get the strides of the layout, i.e., the distances in the memory sequence between consecutive elements of the Holor along  its dimensions.
##### return
The strides of the layout.

<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 



#### stride
##### signature
``` cpp
    auto strides(size_t dim) const;
```
##### brief
Get the stride along a single dimension of the layout, i.e., the distance in the memory sequence between consecutive elements of the Holor in a dimension.
##### parameters
* `dim`: the dimension queried. There is no check to verify if the value `dim` is feasible.
##### return
The stride of the layout along the dimension `dim`.


<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Indexing functions

#### operator()
##### signature
1. 
``` cpp
    template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
    size_t operator()(Dims&&... dims) const;
```
2. 
``` cpp
    template <class Container> requires assert::SizedContainer<Container, N> && SingleIndex<typename Container::value_type>
    size_t operator()(Container dims) const;
```
3. 
``` cpp
    template <class Container> requires assert::ResizeableContainer<Container> && SingleIndex<typename Container::value_type>
    size_t operator()(Container dims) const;
```
##### brief
Get the position in memory of a single element according to the mapping described in the Layout.
##### parameters
* `dims`: the coordinates of the element accessed, given either as a parameter pack or as a container of `SingleIndex` elements (Refer to [Indices](./Indexes.html) for more details).

!!! warning
    When indexing a Layout using a parameter pack (signature 1.) an `holor::exception::HolorRuntimeError` exception maybe thrown if the arguments are outside the admissible range for each coordinate of the Layout. The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to `AssertionLevel::no_checks` to exclude this check. Refer to [Exceptions](./Exceptions.html) for more details. This check is not performed when using the signature 2.

##### return
The position of the element in the 1D data sequence.


<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Slicing functions

#### operator()
##### signature
1. 
``` cpp
    template<typename... Args> requires (impl::ranged_index_pack<Args...>() && (sizeof...(Args)==N) )
    auto operator()(Args&&... args) const;
```

##### brief 
Function for extracting a slice from the Layout. Singleton dimensions (dimensions that are reduced to a single element) are removed.
##### example
``` cpp
    using namespace holor;
    Layout<2> my_layout(2,2); //two dimensional Layout describing a 2x2 container
    auto row = my_layout(0, range{0,1}); // This operation computes the Layout corresponding to the first row. The result would is a Layout<1> with lengths = [2];
```
##### parameters
* `args`: parameters pack. Each element of the pack must be either a `SingleIndex` or a `RangeIndex`, and at least one of them must be a `RangeIndex`. Refer to [Indices](./Indexes.html) for more details.

!!! warning
    When slicing a Layout an `holor::exception::HolorRuntimeError` exception maybe thrown if the arguments are outside the admissible range for each coordinate of the Layout. The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to `AssertionLevel::no_checks` to exclude this check. Refer to [Exceptions](./Exceptions.html) for more details.

##### return
The Layout that maps to the subset of elements included in the slice.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">



#### slice_unreduced
##### signature
``` cpp
    template<typename... Args> requires (impl::ranged_index_pack<Args...>() && (sizeof...(Args)==N) )
    Layout<N> slice_unreduced(Args&&... args) const;
```
##### brief 
Function for extracting a slice from the Layout, but without removing singleton dimensions (dimensions with a single element).
##### example
``` cpp        
    using namespace holor;
    Layout<2> my_layout(2,2); //two dimensional Layout describing a 2x2 container
    auto row = my_layout(0, range{0,1}); // This operation compute the Layout corresponding to the first row. The result would is a Layout<2> with lengths = [1, 2];
```
##### parameter
* `args`: parameters pack. Each element of the pack indexes either an element or a range of elements along a dimension of the Layout.

!!! warning
    When slicing a Layout an `holor::exception::HolorRuntimeError` exception maybe thrown if the arguments are outside the admissible range for each coordinate of the Layout. The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to `AssertionLevel::no_checks` to exclude this check. Refer to [Exceptions](./Exceptions.html) for more details.

##### return
the Layout containing the indexed range of elements. In this case the Layout has dimension `N`, i.e. the dimensionality is not reduced.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">



#### slice_dimension
##### signature
1. 
``` cpp
    template<size_t Dim> requires ( (Dim>=0) && (Dim <N) )
    Layout<N> slice_dimension(range r) const
```
2. 
``` cpp
    template<size_t Dim> requires ( (Dim>=0) && (Dim <N) )
    auto slice_dimension(size_t num) const;
```

##### brief
Function for indexing a single dimension of the Layout

##### parameters
* `r`: the range of elements to be taken from the dimension `Dim`. `r` must indicate a valid range of indices in the dimension `Dim`.
* `num`: the index of the element to be taken from the dimension `Dim`.

!!! warning
    When slicing a Layout an `holor::exception::HolorRuntimeError` exception maybe thrown if the arguments are outside the admissible range for each coordinate of the Layout. The compiler flag DDEFINE_ASSERT_LEVEL in the CMakeLists can be set to `AssertionLevel::no_checks` to exclude this check. Refer to [Exceptions](./Exceptions.html) for more details.

##### return
A new Layout with `N` or `N-1` dimensions, where the dimension `Dim` contains only the lements indexed by the `range` argument.

!!! warning
    When slicing a Layout with a single number rather than with a range, the returned Layout as `N-1` dimensions. Thus, the Layout being sliced must have `N>1` dimensions.



<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



## Non-Member functions

#### equality operator==
##### signature
``` cpp
    template<size_t M>
    bool operator==(const Layout<M>& l1, const Layout<M>& l2);
```
##### brief 
Comparison operator that verifies the equality of Layout objects of the same order `M`.
##### template parameter
* `M`: the order of the two Layouts to be compared.
##### parameter
* `l1`: first Layout object of the comparison.
* `l2`: second Layout object of the comparison.

##### return
true if the two Layouts are equal, false otherwise
<hr style="background-color:#9999ff; opacity:0.4; width:50%">



#### inequality operator!=
##### signature
``` cpp
    template<size_t M>
    bool operator!=(const Layout<M>& l1, const Layout<M>& l2);
```
##### brief 
Comparison operator that verifies the inequality of Layout objects of the same order `M`.
##### template parameter
* `M`: the order of the two Layouts to be compared.
##### parameter
* `l1`: first Layout object of the comparison.
* `l2`: second Layout object of the comparison.

##### return
true if the two Layouts are unequal, false otherwise
<hr style="background-color:#9999ff; opacity:0.4; width:50%">
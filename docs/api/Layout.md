# holor::Layout

Defined in header `holor/layout.h`.    

``` cpp
    template<size_t N>
    class Layout
```



Class that represents the mapping to the contiguous memory layout of a Holor container.
The Layout class contains the information for indexing the contiguous memory where the elements of the Holor or HolorRef are stored.
It uses the idea of generalized layouts from the standard library, i.e., it is based on the fact that the elements of a Holor or HolorRef
are stored as a 1D data sequence following a [row-major](https://en.wikipedia.org/wiki/Row-_and_column-major_order) representation.

A layout contains three main pieces of information: 

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

|Name | Description                        |
|-----|------------------------------------|
| `N` | number of dimensions in the layout |

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>


## Member types and aliases
|Name | Description                        |
|-----|------------------------------------|
| `order` | static member equal to `N`|


<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>


## Public Member functions

### Constructors
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

###### brief
Create a Layout object, either as an empty layout with 0-length dimensions (1), or initializing it from another layout (2, 3), or providing the lenghts (number of elements) mapped in each dimension (4, 5, 6).

###### parameters
* `layout`:  another layout to be used to initialize the created layut|
* `lengths`: number of elements per dimension ( either a container such as `#!cpp std::vector<size_t>` and `#!cpp std::array<size_t, N>`, or a variadic argument.)|

!!! warning
    when constructing a Layout from a container of `lengths`, this container must have `N` elements. Otherwise, a compilation error will be generated if the container has a fixed size, or an exception (if enabled) will be thrown if the container has dynamic size

###### return

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Assignments
1. 
``` cpp
    Layout<N>& operator=(const Layout<N>& layout);
```
2. 
```cpp
    Layout<N>& operator=(Layout<N>&& layout);
```

###### brief
Assign to a Layout from another Layout.

###### parameters
* `layout`: another layout to assign from

<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Get/Set functions

#### dimensions
``` cpp
    constexpr size_t dimensions() const;
```
###### brief 
get the number of dimensions of the layout.

###### return
the number `N` of dimensions of the layout.

 <hr style="background-color:#9999ff; opacity:0.4; width:50%;">



#### size
``` cpp
    size_t size() const;
```
###### brief
get the size the layout.

###### return
the total number of elements mapped by the layout.

<hr style="background-color:#9999ff; opacity:0.4; width:50%">



#### offset
``` cpp
    size_t offset() const;
```
###### brief 
Get the offset of the layout. This is a const function.
###### return
The offset of the position of first element in the container with respect to the beginning of the memory where the elements are stored.

<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 



#### lengths
``` cpp
    std::array<size_t,N> lengths();
```
###### brief
Get the lengths of the layout. This is a const function.
###### return
the lengths (number of elements per dimension) of the layout.

<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 


#### length
``` cpp
    auto length(size_t dim) const;
```
###### brief
Get a length of a dimension of the layout. This is a const function.

###### parameters
* `dim`: the dimension queried. There is no check to verify if the value `dim` is feasible.

###### return 
the length along a dimension (number of elements in that dimension).
        
<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 


#### strides
``` cpp
    std::array<size_t,N> strides() const;
```
###### brief
Get the strides of the layout.
###### return
The strides of the layout.

<hr style="background-color:#9999ff; opacity:0.4; width:50%"> 



#### stride
``` cpp
    auto strides(size_t dim) const;
```
###### brief
Get the stride along a single dimension of the layout.
###### parameters
* `dim`: the dimension queried. There is no check to verify if the value `dim` is feasible.
###### return
The stride of the layout along the dimension `dim`.


<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Indexing functions

#### operator()
1. 
``` cpp
    template<SingleIndex... Dims> requires ((sizeof...(Dims)==N) )
    size_t operator()(Dims&&... dims) const;
```
2. 
``` cpp
    template<SingleIndex ID>
    size_t operator()(std::array<ID,N> dims) const;
```
###### brief
Get the position in memory of a single element according to the mapping described in the Layout.
###### parameters
* `dims`: the coordinates of the element accessed, given either as a parameter pack or as an array. The coordinates must abide the `SingleIndex` concept
###### return
The position of the element in the 1D data sequence.


<hr style="border:1px solid #9999ff; background-color:#9999ff; opacity:0.7"> </hr>



### Slicing functions

#### operator()
1. 
``` cpp
    template<typename... Args> requires (impl::ranged_index_pack<Args...>() && (sizeof...(Args)==N) )
    auto operator()(Args&&... args) const;
```

###### brief 
Function for extracting a slice from the Layout. Singleton dimensions (dimensions that are reduced to a single element) are removed.
###### example
``` cpp
    using namespace holor;
    Layout<2> my_layout(2,2); //two dimensional Layout describing a 2x2 container
    auto row = my_layout(0, range{0,1}); // This operation computes the Layout corresponding to the first row. The result would is a Layout<1> with lengths = [2];
```
###### parameters
* `args`: parameters pack. Each element of the pack indexes either an element or a range of elements along a dimension of the Layout.

###### return
The Layout that maps to the subset of elements included in the slice.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">



#### slice_unreduced
``` cpp
    template<typename... Args> requires (impl::ranged_index_pack<Args...>() && (sizeof...(Args)==N) )
    Layout<N> slice_unreduced(Args&&... args) const;
```
###### brief 
Function for extracting a slice from the Layout, but without removing singleton dimensions (dimensions with a single element).
###### example
``` cpp        
    using namespace holor;
    Layout<2> my_layout(2,2); //two dimensional Layout describing a 2x2 container
    auto row = my_layout(0, range{0,1}); // This operation compute the Layout corresponding to the first row. The result would is a Layout<2> with lengths = [1, 2];
```
###### parameter
* `args`: parameters pack. Each element of the pack indexes either an element or a range of elements along a dimension of the Layout.
###### return
the Layout containing the indexed range of elements. In this case the Layout has dimension `N`, i.e. the dimensionality is not reduced.
<hr style="background-color:#9999ff; opacity:0.4; width:50%">



#### slice_dimension
1. 
``` cpp
    template<size_t Dim> requires ( (Dim>=0) && (Dim <N) )
    Layout<N> slice_dimension(range range) const
```
2. 
``` cpp
    template<size_t Dim> requires ( (Dim>=0) && (Dim <N) )
    Layout<N-1> slice_dimension(size_t num) const;
```

###### brief
Function for indexing a single dimension of the Layout

###### parameters
* `range`: the range of elements to be taken from the dimension `Dim`. `range` must indicate a valid range of indices in the dimension `Dim`.
* `num`: the index of the element to be taken from the dimension `Dim`.

###### return
A new Layout with `N` or `N-1` dimensions, where the dimension `Dim` contains only the lements indexed by the `range` argument.

###### exceptions
Throws a `holor::exception::HolorRuntimeError` if `range` does not satisfy their constraints. The exception level is `release`.

!!! note
    the level of dynamic checks is by default set on `release`, and can be changed by setting the compiler directive `DEFINE_ASSERT_LEVEL`. Refer to the [installation guide](../getting_started/install.html) for more details.

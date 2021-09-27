# holor::Layout

Defined in header `holor/layout.h`.    

``` cpp
    template<size_t N>
    class Layout
```



Class that represents the contiguous memory layout of a Holor container or a Slice of a Holor.
The Layout class contains the information for indexing the contiguous memory where the elements of the Holor or HolorRef are stored.
It uses the idea of generalized layouts from the standard library, i.e., it is based on the fact that the elements of a Holor or HolorRef
are stored as a 1D data sequence following a row-major representation.

A layout contains three main pieces of information: 

- __offset__: the offset in the contiguous memory of the first element indexed by the layout.  
- __lengths__: the numbers of elements along every dimension of the layout.
- __strides__: the distances in the 1D memory sequence between successive elements in individual dimensions of the layout.

!!! note
    For a Layout with `N` dimensions, both the length array and the stride array must be size `N`.

A Layout supports two fundamental operations:

- __Indexing__: this operation provides a map from the coordinates in the container to the index in memory of the selected element;
- __Slicing__: this operation allows to select a subset of elements from a container by computing a new Layout that provides the needed information to index them. 


## Template parameters

|Name | Description                        |
|-----|------------------------------------|
| `N` | number of dimensions in the layout |

## Member types and aliases
|Name | Description                        |
|-----|------------------------------------|
| `order` | static member equal to `N`|


## Public Member functions

### Constructors
1. Default constructor. It creates an empty layout with no elements.
``` cpp
    Layout();`
```
2. Copy constructor.
``` cpp
    Layout(const Layout<N>& layout);
```
3. Move constructor.
``` cpp
    Layout(Layout<N>&& layout);
```  
4. Constructor specifying the lengths (number of elements per dimension)
``` cpp
    template <class Container> requires assert::SizedTypedContainer<Container, size_t, N>
    explicit Layout(const Container& lengths);
```
``` cpp
    template <class Container> requires assert::ResizeableTypedContainer<Container, size_t>
    explicit Layout(const Container& lengths);
```
``` cpp
    template<typename... Lengths> requires ((sizeof...(Lengths)==N) && (assert::all(std::is_convertible_v<Lengths,size_t>...)) )
    explicit Layout(Lengths&&... lengths);
```
#### Parameters
| Name | Description |
|--|---|
|`layout` | another layout to be used to initialize the created layut|
|`lengths`| number of elements per dimension ( either a container such as `#!cpp std::vector<size_t>` and `#!cpp std::array<size_t, N>`, or a variadic argument.)|

!!! warning
    when constructing a Layout from a container of `lengths`, this container must have `N` elements. Otherwise, a compilation error will be generated if the container has a fixed size, or an exception (if enabled) will be thrown if the container has dynamic size



### Assignments
1. Copy assignment
``` cpp
    Layout<N>& operator=(const Layout<N>& layout);
```
2. Move assignment
```cpp
    Layout<N>& operator=(Layout<N>&& layout);
```

#### Parameters
| Name | Description |
|--|---|
|`layout` | another layout to assign from|


### Get/Set functions


``` cpp
    constexpr size_t dimensions() const;
```
**brief**: get the number of dimensions of the layout.

**return**: the number `N` of dimensions of the layout.

 ---

``` cpp
    size_t size() const;
```
**brief**: get the size the layout.

**return**: the total number of elements mapped by the layout.



### Indexing functions

### Slicing functions



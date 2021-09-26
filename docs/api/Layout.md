# holor::Layout

Defined in header `holor/layout.h`.

---

``` cpp
    template<size_t N>
    class Layout
```

---

Class that represents the contiguous memory layout of a Holor container or a Slice of a Holor.
The Layout class contains the information for indexing the contiguous memory where the elements of the Holor or HolorRef are stored.
It uses the idea of generalized layouts from the standard library, i.e., it is based on the fact that the elements of a Holor or HolorRef
are stored as a 1D data sequence following a row-major representation.

A layout contains three fundamental information: 

- __offset__: the offset in the contiguous memory of the first element indexed by the layout.  
- __lengths__: the numbers of elements along every dimension of the layout.
- __strides__: the distances in the 1D memory sequence between successive elements in individual dimensions of the layout.

!!! note
    For a Layout with `N` dimensions, both the length array and the stride array must be size `N`.

A Layout supports two fundamental operations:

- __Indexing__: this operation provides a map from the coordinates in the container to the index in memory of the selected element;
- __Slicing__: this operation allows to select a subset of elements from a container by computing a new Layout that provides the needed information to index them. 


## Template parameters
`N` is the number of dimensions in the layout

## Member types

## Member variables

## Member functions

### Constructors

### Get/Set functions

### Constructors


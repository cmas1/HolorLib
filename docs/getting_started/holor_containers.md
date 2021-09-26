# What's a Holor container?
Holor containers are generic multi-dimensional collections of homogeneous elements, i.e., all elements must be of the same type. These containers are characterized by:

* the number of dimensions `N`;
* the `lengths`, that is the number of elements along each individual dimension;
* the type of the elements stored.

From the point of view of implementation, holor containers have three fundamental components:

* __Memory__: a holor container has access to a contiguous area of memory where the "contained" elements are stored.
* __Layout__: each holor container possesses a `Layout`, an object that contains the information to map from the the multi-dimensional coordinates (indices) in the container dimensions to a memory location, and viceversa. In particular, holor containers use a [row-major ordering](https://en.wikipedia.org/wiki/Row-_and_column-major_order).
* __Indexing/Slicing operations__: a holor container is endowed with a set of operations that allow to access its individual elements from a set of `N` coordinates (**indexing**), or to extract a holor container that is a subset of the original one (**slicing**)


<figure>
  <img src="../images/holor.png" width="500" />
  <figcaption>The implementation of Holor containers has three fundamental components: a contiguous memory location that stores the elements, a layout that contains information about the correspondance between the coordinates of the elements and their memory location, and a set of operations to access or slice the container.</figcaption>
</figure>


## Memory
Each holor container has access to a contiguous area of memory where its elements are stored. The library provides two different types of containers, which differentiate from each other for the ownership of the storage:

* A `Holor` is a container that has ownership of the memory where the elements are stored. This is implemented as an `std::array` within the container.
* A `HolorRef` is a non-owning Holor-like view over a sequence of elements stored in a contiguous area of memory. This is implemented as a pointer to the memory location.




## Layout
A Layout contains the essential information needed to map the indices of the holor container to a location in the memory where the elements are stored. 

- **Offset**: the displacement in the contiguous memory of the first element indexed by the layout from the beginning of the memory.  
- **Lengths**: the numbers of elements along every dimension of the layout.
- **Strides**: the distances in the 1D memory sequence between successive elements in individual dimensions of the layout.

!!! important
    For a Layout with `N` dimensions, both lengths and strides must have size `N`.

For a generic `N` dimensional holor container, the total number of elements stored is 
\begin{equation}
    size = \prod_{i=0}^{N-1} lengths[i]
\end{equation}

The index of the memory location corresponding to the element of the container  with coordinates $(c_0, \ldots, c_{N-1})$ is 
\begin{equation}
    index = offset + \sum_{i=0}^{N-1} c_i \cdot strides[i]
\end{equation}





## Indexing and Slicing operations
Holor containers are endowed with operations that allow to **index** individual elements of the container, accessing the memory location where they are stored,  or to **slice** parts of it yielding a new view (i.e., a HolorRef) over the selected part of the container.

The indexing and slicing operations have a simple sintax common to many scripting languages. For example, one can write 

``` cpp
    using namespace holor;
    Holor<int,2> A = {{1,2,3,4},{5,6,7,8},{9,10,11,12}}; // 3-by-4 matrix of integers
    auto c = A(2,3); // indexing a single element. c has the value 8. 
    auto B = A(range(0,1), range(2,3)); // Slicing a 2-by-2 matrix. B is a HolorRef that views the elements {{3,4}, {7,8}} of A
```

For more detailed examples refer to the section Tutorials.



#### Example: indexing

Consider a $2 \times 3 \times 4$ Holor as in figure.

<figure>
  <img src="../images/indexing_example.png" width="800" />
  <figcaption>Indexing example</figcaption>
</figure>

The container has 24 elements which are stored contiguously. The Holor's Layout has:

* offset = 0;
* lengths = [2, 3, 4];
* strides = [12, 4, 1];

The element with coordinates (0,0,0) is located ath the position $0 + 0 \cdot 12 + 0 \cdot 4 + 0 \cdot 1 = 0$ of the memory, whereas the element with coordinates (0,2,2) is located in the memory area at the position $0 + 0 \cdot 12 + 2 \cdot 4 + 2 \cdot 1 = 10$.




#### Example: slicing

Consider the same $2 \times 3 \times 4$ Holor as in the previous example, and suppose that we want to take a 2-by-2 slice corresponding to the first row, last two columns and first two lines in the third dimension.
In code, this would be 

``` cpp
    auto B = A(1, range(0,1), range(0,1)); //suppose that A is the name of the Holor to be sliced
```

<figure>
  <img src="../images/slice_example.png" width="800" />
  <figcaption>Slicing example</figcaption>
</figure>


The slicing operation extracts a subset of the Holor container. 
The slicing operation does not create a new Holor, rather it returns a HolorRef that provides a view over the selected slice. For this purpose, a new Layout that describes the location of the selected elements in the original contiguous memory is computed.
In this example, the Layout corresponding to the selected 2-by-2 slice is:

* offset = 4
* lengths = [2, 2]
* strides = [4, 1]
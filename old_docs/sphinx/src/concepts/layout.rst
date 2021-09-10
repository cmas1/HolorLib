Layout
======================================
Holor and HolorRef containers store their elements in a contiguous area of memory, i.e., given the start (pointer) of the area of memory, the location of the stored elements with respect to this initial point is defined by a single index even though the container itself may be multi-dimensional. 
In particular, we use a `row-major ordering <https://en.wikipedia.org/wiki/Row-_and_column-major_order>`_.
The correspondance between the shape of the multi-dimensional container and the locations in memory of the stored elements is provided by a Layout.

Each N-dimensional Holor or HolorRef is associated with a Layout<N> object that contains the information needed to establish a map from the multi-dimensional coordinates in the container to a memory location, and viceversa.
A layout contains three fundamental information: 
    - **Offset**: is the displacement in the contiguous memory of the first element indexed by the layout from the beginning of the memory.  
    - **Lengths**: are the numbers of elements along every dimension of the layout.
    - **Strides**: are the distances in the 1D data sequence between successive elements in individual dimensions of the layout.
For a Layout with `N` dimensions, both the length array and the stride array must be size `N`.

A Layout supports two fundamental operations:
    - *Indexing a single element*: determine the index in memory of a single element in the container;
        **Example**: consider the 3-dimensional Holor container in Figure.
        .. figure:: ../../docs/doxygen/images/tensorslice_1.jpg
            :alt: layout example
            :width: 400

        In this example the Holor container is a 3-dimensional tensor of floating point numbers. These numbers are stored contiguously in a 1D memory, using the row-major paradigm. In this case, the Layout that describes the location of the elements in memory has offset=0, lengths = [3,3,3] and strides = [9, 3, 1]. The location in the 1D memory (index) of the element at row i, column j and depth k is computed as 
        .. math::
            \begin{equation}
             index = offset + i \cdot strides[0] + j \cdot strides [1] + k \cdot strides [2]
            \end{equation}
        
        Indeed, for a generic `N` dimensional container the indexing formula is
        .. math::
            \begin{equation}
              index(c_0, \ldots, c_{N-1} = offset + \sum_{i=0}^{N-1} c_i \cdot strides[i]
            \end{equation}
        

    - *Indexing a slice of the container*: determine a new Layout that provides the information for indexing a subset of elements from the initial container.
        **Example**: consider the 3-dimensional Holor container in Figure.
            .. figure:: ../../docs/doxygen/images/tensorslice_2.jpg
                :alt: layout example
                :width: 400
            In this example the Holor container is a the same as in the previous case, but we index a sub-tensor from the container. To index a subset of elements as in this example, it is not sufficient to get one index, rahter we would need all the indexes of the elements in the subset. However, rather than extractng all these indexes, we can simply use another Layout that describes the mapping in memory of the considered subset. In this example, the Layout cthat describes the selected subset has `offset = 12`, `lengths = ` and `strides = `.




Implementations of layouts
^^^^^^^^^^^^^^^^^^^^^^^^^^



.. Docs of the implementations:

.. .. toctree::
..     :maxdepth: 1
  
..     poly
..     sparse_poly

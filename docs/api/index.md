# C++ API


## Public classes
| Class | Description |
|-------|-------------|
|[Layout](./Layout.html)| Class that provides the mapping between the indices a Holor container and the locations in the memory where the elements are stored. |
|[Holor](./Holor.html)| Class that implements a general `N`-dimensional container for elements of type `T` and that **owns the memory** where the elements are stored.|
|[HolorRef](./HolorRef.html)| Class that implements a general `N`-dimensional container for elements of type `T` and that **does not own the memory** where the elements are stored.|


## Other Facilities
| Name | Description |
|-------|-------------|
|[Indices](./Indexes.html)| HolorLib uses an index notation to provide the interface to access individual elements or range of elements stored in a Holor container. |
|[Exceptions](./Exceptions.html)| HolorLib defines some exceptions that may be thrown by runtime assertions. |
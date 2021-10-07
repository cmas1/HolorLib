# About HolorLib

HolorLib is a C++ library that implements generic multidimensional containers of objects. These objects may be standard types, such as `floats`, `ints` or `vector<double>`, or user defined types such as a `struct`. We call this containers `Holor` and `HolorRef`.
HolorLib provides fundamental operations to manipulate and access these containers, such as indexing and slicing. However, this is **not a mathematical library**: operations to manipulate the containers and their content, such as addition or product, are typically application specific and are not implemented in this library interface. It is left to the users, depending on their use-case, to extend the HolorLib interface as needed.


Next steps:

- [Origin of the word Holor](./origin_of_holor.html)
- [What's a Holor container?](./holor_containers.html)
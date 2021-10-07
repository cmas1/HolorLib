# Getting Started

HolorLib is a header only library that is written using [C++20](https://en.cppreference.com/w/cpp/20) features. Experimenting with C++20 was actually one of the reasons for the creation of this library, as an exercise. As it stands, C++20 is not yet fully supported by compilers and a few of its new features are not yet readily available. 
I did not perform a thorough test to verify compilers compatibility and I have only used `clang++-12`, so for now please refer to the compiler support list [here](https://en.cppreference.com/w/cpp/20). Be mindful that, as for all early releases of new compilers, even though they report supporting certain C++20 features, they may have some bugs. For example, I failed to compile the examples shipped with this library using `g++-10` due to a compiler bug when using concepts in partial template specializations.


Next steps:

- [Install the library](./install.html)
- [Build optional components](./optional.html)
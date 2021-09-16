# Optional modules

HolorLib contains some optional modules that can be built after the installation.



## Examples
Some examples that demonstrate few uses of the library, such as how to construct and slice Holors. 

=== "excplicit cmake"
    ``` bash
        cd build
        cmake .. -DHOLOR_BUILD_EXAMPLES=ON [-DCMAKE_CXX_COMPILER=...]
        make
    ```

=== "script"
    ``` bash
        ./holor.sh build_examples [--compiler ...]
    ```

The compilation of the examples is enabled by setting the cmake option ``-DHOLOR_BUILD_EXAMPLES=ON``. Optionally, one can specify the compiler to be used with the cmake option ``-DCMAKE_CXX_COMPILER``, for example if the default system compiler does not support `C++20`_.


The examples can be compiled using the script ``holor.sh``. Optionally, one can specify the compiler to be used with argument ``--compiler``, for example if the default system compiler does not support `C++20`_.
The compiled binaries are located  in the directory ``build/bin``.




## Tests
Some tests to verify the correct behavior of the library.

!!! warning "Requirements"

    The tests require gtest.

=== "excplicit cmake"
    ``` bash
        cd build
        cmake .. -DHOLOR_BUILD_TESTS=ON [-DCMAKE_CXX_COMPILER=...]
        make
    ```

=== "script"
    ``` bash
        ./holor.sh build_tests [--compiler ...]
    ```

The compilation of the tests is enabled by setting the cmake option ``-DHOLOR_BUILD_TESTS=ON``. Optionally, one can specify the compiler to be used with the cmake option ``-DCMAKE_CXX_COMPILER``, for example if the default system compiler does not support `C++20`_.

The tests can be compiled using the script ``holor.sh``. Optionally, one can specify the compiler to be used with argument ``--compiler``, for example if the default system compiler does not support `C++20`_.
The compiled binaries are located  in the directory ``build/bin``.




## Benchmarks
Some benchmarks to evalaute the performance of the Holor containers.

!!! warning "Requirements"

    BLABLA 

=== "explicit cmake"
    ``` bash
        cd build
        cmake .. -DHOLOR_BUILD_BENCHMARKS=ON [-DCMAKE_CXX_COMPILER=...]
        make
    ```
=== "script"
    ``` bash
        ./holor.sh build_benchmarks [--compiler ...]
    ```

The compilation of the benchmarks is enabled by setting the cmake option ``-DHOLOR_BUILD_BENCHMARKS=ON``. Optionally, one can specify the compiler to be used with the cmake option ``-DCMAKE_CXX_COMPILER``, for example if the default system compiler does not support `C++20`_.

The benchmarks can be compiled using the script ``holor.sh``. Optionally, one can specify the compiler to be used with argument ``--compiler``, for example if the default system compiler does not support `C++20`_.
The compiled binaries are located  in the directory ``build/bin``.



## Documentation
This same documentation can be built locally to be accessed offline.

!!! warning "Requirements"
    [Material for MkDocs](https://squidfunk.github.io/mkdocs-material/)

The documentation can be built using the following command

``` bash
    mkdocs build
```

The documentation, in html format, will be generated in the `site` folder. To consult it, open the ``index.html`` file in a web browser.
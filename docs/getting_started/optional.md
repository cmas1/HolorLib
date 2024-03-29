# Optional modules

HolorLib contains some optional modules that can be built after the installation.



## Examples
Some examples that demonstrate few uses of the library, such as how to construct and slice Holors. The examples can be compiled using the following instructions from the root folder.

=== "excplicit cmake"
    ``` bash
        cd build
        cmake .. -DHOLOR_BUILD_EXAMPLES=ON -DCMAKE_BUILD_TYPE=Release [-DCMAKE_CXX_COMPILER=...]
        make
    ```

=== "script"
    ``` bash
        ./holor.sh build_examples [--compiler ...]
    ```

The compilation of the examples is enabled by setting the cmake option `-DHOLOR_BUILD_EXAMPLES=ON`. Optionally, one can specify the compiler to be used with the cmake option `-DCMAKE_CXX_COMPILER`, for example if the default system compiler does not support C++20.


The examples can be compiled using the script `holor.sh`. Optionally, one can specify the compiler to be used with argument `--compiler`, for example if the default system compiler does not support C++20.
The compiled binaries are located  in the directory `build/bin`.




---


## Tests
Some tests to verify the correct behavior of the library. The tests can be compiled using the following instructions from the root folder.

!!! warning "Requirements"

    The tests require [gtest](https://github.com/google/googletest), to be used in conjunction with CMake. We recommend checking the instructions on the gtest page. For a quick setup, go to the directory where you want to download the gtest repository and follow these instructions
    ``` bash
        git clone https://github.com/google/googletest.git 
        cd googletest
        mkdir build 
        cd build
        cmake ..
        make
        sudo make install
    ```

=== "excplicit cmake"
    ``` bash
        cd build
        cmake .. -DHOLOR_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release [-DCMAKE_CXX_COMPILER=...]
        make
    ```

=== "script"
    ``` bash
        ./holor.sh build_tests [--compiler ...]
    ```

The compilation of the tests is enabled by setting the cmake option `-DHOLOR_BUILD_TESTS=ON`. Optionally, one can specify the compiler to be used with the cmake option `-DCMAKE_CXX_COMPILER`, for example if the default system compiler does not support C++20.

The tests can be compiled using the script `holor.sh`. Optionally, one can specify the compiler to be used with argument `--compiler`, for example if the default system compiler does not support C++20.
The compiled binaries are located  in the directory `build/bin`.




---


## Benchmarks
Some benchmarks to evalaute the performance of the Holor containers. The benchmarks can be compiled using the following instructions from the root folder.

!!! warning "Requirements"

    The tests require [google benchmark](https://github.com/google/benchmark), to be used in conjunction with CMake. We recommend checking the instructions on the benchmark page. For a quick setup, go to the directory where you want to download the  repository and follow these instructions
    ``` bash
        git clone https://github.com/google/benchmark.git
        cd benchmark
        cmake -E make_directory "build"
        cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../
        cmake -DCMAKE_BUILD_TYPE=Release -S . -B "build"
        cmake --build "build" --config Release 
        cd build
        sudo make install
    ```

=== "explicit"
    ``` bash
        cd build
        cmake .. -DHOLOR_BUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release [-DCMAKE_CXX_COMPILER=...]
        make
    ```
=== "script"
    `` bash
        ./holor.sh build_benchmarks [--compiler ...]
    ```

The compilation of the benchmarks is enabled by setting the cmake option `-DHOLOR_BUILD_BENCHMARKS=ON`. Optionally, one can specify the compiler to be used with the cmake option `-DCMAKE_CXX_COMPILER`, for example if the default system compiler does not support C++20.

The benchmarks can be compiled using the script `holor.sh`. Optionally, one can specify the compiler to be used with argument `--compiler`, for example if the default system compiler does not support C++20.
The compiled binaries are located  in the directory `build/bin`.



---


## Documentation
This very same documentation can be built locally to be accessed offline.

!!! warning "Requirements"
    The documentation is built using [Material for MkDocs](https://squidfunk.github.io/mkdocs-material/) 7.3.0 or superior. We recommend to install it using `pip`
    ``` bash
        pip install mkdocs-material
    ```

The documentation can be built using the following command from the root folder

``` bash
    mkdocs build
```
The documentation, is generated in html format in the `site` folder. To consult it, open the `index.html` file in a web browser.
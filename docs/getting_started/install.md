# Installation


## Premise
HolorLib is a header only library that is written using [C++20](https://en.cppreference.com/w/cpp/20) features. Experimenting with C++20 was actually one of the reasons for the creation of this library, as an exercise. As it stands, C++20 is not yet fully supported by compilers and a few of its new features are not yet readily available. 
I did not perform a thorough test to verify compilers compatibility and I have only used `clang++-12`, so for now please refer to the compiler support list [here](https://en.cppreference.com/w/cpp/20). Be mindful that, as for all early releases of new compilers, even though they report supporting certain C++20 features, they may have some bugs. For example, I failed to compile the examples shipped with this library using `g++-10` due to a compiler bug when using concepts in partial template specializations.



## Method 1. Installation with CMake (Recommended)
!!! warning "Requirements"
    CMake 3.12 or above

HolorLib is shipped as a CMake package, which allows it to be easily installed and used by other CMake projects. This process can be performed explicitly or using the installation script packaged with the library.


=== "explicit"

    ``` bash linenums="1"
      git clone TBD <destination_path>
      cd <destination path>/Holor
      mkdir build && cd build 
      cmake .. -[options]
      ccmake . #optional, to check and possibly modify the CMake options
      cmake --install .
    ```

=== "script"

    ``` bash linenums="1"
      git clone TBD <destination_path>
      cd <destination path>/Holor
      ./holor.sh install
    ```

!!! note
    Depending on the destination where the header files will be copied, the installation (line 6 in the _manual_ instructions) may require superuser privileges. In that case call it with `#!bash sudo`.


There are a few CMake options that can be passed to customize the manual CMake installation:

| CMake Option                  | Description                                          | Possible Values                                                                | Default                 |
| ----------------------------- | ---------------------------------------------------- |--------------------------------------------------------------------------------|-------------------------|
| `CMAKE_INSTALL_PREFIX`        | location where the header files will be copied       | a path                                                                         |`\user\local\install`    |
| `DEFINE_HOLOR_ASSERT_LEVEL`   | control HolorLibs exceptions-throwing dynamic checks | `AssertionLevel::release`, `AssertionLevel::no_checks`, `AssertionLevel::debug`|`AssertionLevel::release`|

!!! example
    For a manual installation to a different path (e.g. `/home/user/my_project`), line 4 in the _manual_ instructions becomes
    ``` bash
        cmake .. -DCMAKE_INSTALL_PREFIX=/home/user/my_project
    ```

Such options are not available in the installation _script_, which uses the default values.

!!! tip
    We advise to use the default value for `DEFINE_HOLOR_ASSERT_LEVEL`.



### How to use it in a project
When HolorLib is installed using CMake, it can be easily imported in other CMake projects.

``` cmake

    find_package(Holor REQUIRED)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    add_executable(my_program my_program.cpp) #example of an executable
    target_link_libraries(my_program PUBLIC Holor::Holor)
```

Within the the file `my_program.cpp`, the HolorLib C++ API is accessible with a simple include 

``` cpp
    #include <holor/holor_full.h>
```


### Deleting the installation
During the installation with these method, CMake create an `install_manifest.txt` file in the build folder which contains the information where the files have been installed and allows to easily remove them.


=== "explicit"

    ``` bash
        cd <build directory>
        make uninstall
    ```

=== "script"

    ``` bash
        ./holor.sh clean
    ```

!!! tip   
    1. It may be necessary to call the command ``make uninstall`` with superuser privileges (using ``sudo``) depending on how and where the files were installed.
    2. If `install_manifest.txt` was deleted, the installed files must be manually removed by the user. 





## Method 2. Installation without CMake
HolorLib can also be used by simply copying the header files into a project.

``` bash
   git clone TBD <destination_path>
   cd <destination path>
   cp include <project_path>
```

### How to use it in a project
Once the header files are copied in the C++ project, HolorLib API is accessible with a simple include
``` cpp
    #include "<path_to_local_include>/holor/holor_full.h"
```


### Deleting the installation
Since there is no installation, only the header files copied in the project need to be removed.




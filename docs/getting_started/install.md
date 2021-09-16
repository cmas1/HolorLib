# Installation


## Before Installing
HolorLib is a header only library. Therefore the library itself does not need to be compiled to be used in a project. However, the header files are written using [C++20](https://en.cppreference.com/w/cpp/20) features and we plan to include more of these features as they become available in compilers. Therefore, a project using HolorLib should be compiled using a compiler that supports [C++20](https://en.cppreference.com/w/cpp/20).
We did not perform a thorough test to verify which compilers can be used with this library. We successfully compiled all examples and tests shipped in this library using `clang++12`, but we failed to compile them with `g++-10` due to a compiler bug when using concepts in partial template specializations. 



## Method 1. Installation with CMake (Recommended)

!!! warning "Requirements"
    CMake 3.12 or above

HolorLib is shipped as a CMake package, which allows it to be easily installed and used by other CMake projects. HolorLib offers two options to 



``` bash
   git clone TBD <clone_destination>
```

Then go into the folder, create the build directory (where the cmake files will be generated), and build the project with cmake

``` bash
   cd <clone_destination>/Holor
   mkdir build && cd build
   cmake .. -[options]
```

There are a few CMake options that can be passed:

* ``-DCMAKE_INSTALL_PREFIX=<path>``. This option can be used to specify the location where the header files will be copied. By default the headers are copied in ``\user\local\install``. If there is no specific reaso to change this, the option can be omitted.
* ``-DDEFINE_HOLOR_ASSERT_LEVEL=<Assertion level>``. This option is used to control what exceptions can be thrown by the HolorLib code. TThere are three levels:
   
   - ``AssertionLevel::release`` is the default setting, and it contains few checks that may throw exceptions (for example, accessing an out of index element of a container).
   - ``AssertionLevel::no_checks`` this setting removes all checks
   - ``AssertionLevel::debug`` this setting enables all checks, including the checks that are added for debuggin. 

!!! tip
    We advise to not use the ``DDEFINE_HOLOR_ASSERT_LEVEL`` option.


With the cmake files generated, it is possible to review all options and reconfigure the project using the *CMake curses* interface

``` bash
   ccmake .
```

Finally, the library can be installed by calling

``` bash
   cmake --install .
```

!!! note
    Depending on the destination where the header files will be copied, this last command may require superuser privileges. In that case call it with `#!bash sudo`.


### How to use it in a project

### Deleting the installation
During the installation with these method, CMake create an ``install_manifest.txt`` file in the build folder which contains the information where the files have been installed.
Therefore, to uninstall the library simply do

``` bash
   cd <build directory>
   make uninstall
```
!!! tip   
    It may be necessary to call the command ``make uninstall`` with superuser privileges (using ``sudo``) depending on how and where the files were installed.

Finally, the build folder with all its content can be deleted.




## Method 3. Installation using script (Recommended)
!!! warning "Requirements"
    CMake 3.12 or above


To simplify installation, and if the user does not need to modify any of the CMake options listed in :ref:`install2`, we advise to install the library by using the provided script ``holor.sh``, which will call all the appropriate cmake commands.

First, download the repository

``` bash
   git clone TBD <clone_destination>
   cd <clone_destination>/Holor
```

Make sure that the script ``holor.sh`` is executable and verify its options by calling

``` bash
   ./holor.sh --help
```

From the help the user can see that this script provides a simple interface to not only install the library, but also to build some additional components such as examples and tests (described in :ref:`optional`). 


Install the library by simply calling

``` bash
   ./holor.sh install
```




### Deleting the installation
If the library was installed via the ``holor.sh`` script, all the installed and build files and optional components can be removed with the same script, calling

``` bash
   ./holor.sh clean
```



## Method 2. Installation without CMake
HolorLib is a header-only library, therefore it can be used by simply copying the header files into your project

``` bash
   git clone TBD <destination_path>
   cd <destination path>
   cp include <project_path>
```

### How to use it in a project
Once the header files are copied in your C++ project, you can use it with a simple include
``` cpp
    #include "<path_to_local_include>/holor/holor_full.h"
```


### Deleting the installation
Deleting the HolorLib simply amounts to deleting the HolorLib header files copied in the project.




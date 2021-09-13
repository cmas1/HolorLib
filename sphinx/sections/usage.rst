Using HolorLib in a project
===========================



Method 1. Copying header files
-------------------------------
HolorLib can be used in a project by simply copying the header files from the folder ``include`` to the directory of the project.
The Holor containers can then be used with a simple include

.. code-block:: cpp

    #include "relative_path_to_include/holor/holor_full.h"




Method 1. Import CMake Package (recommended)
--------------------------------------------
When HolorLib is installed following :ref:`install1` or :ref:`install2`, a CMake project is exported and can be used in other CMake projects, linking it to the executables that use it

.. code-block:: cmake

    find_package(Holor REQUIRED)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    add_executable(my_program my_program.cpp) #this is just an example of an executable using HolorLib
    target_link_libraries(my_program PUBLIC Holor::Holor)


In the code, in this simple example in the file ``my_program.cpp``, the Holor containers can be used with a simple include 

.. code-block:: cpp

    #include <holor/holor_full.h>
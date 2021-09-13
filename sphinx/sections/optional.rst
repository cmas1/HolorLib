.. _optional:

Optional components
=====================

The library also contains some optional modules that can be built after the :ref:`installation`:

* **examples**: these are some examples that demonstrate few uses of the library, such as how to construct and slice Holors.
* **tests**: these are some tests that check the behavior of the library.
* **benchmarks**: there are some benchmarks to evalaute the performance of the Holor containers.
* **documentation**: this same documentation can be built locally to be accessed offline.


Examples
^^^^^^^^

Method 1. Building with CMake
-----------------------------
The compilation of the examples is enabled by setting the cmake option ``-DHOLOR_BUILD_EXAMPLES=ON``. Optionally, one can specify the compiler to be used with the cmake option ``-DCMAKE_CXX_COMPILER``, for example if the default system compiler does not support `C++20`_.

.. code-block:: bash

    cd build
    cmake .. -DHOLOR_BUILD_EXAMPLES=ON [-DCMAKE_CXX_COMPILER=...]
    make



Method 2. Building with script
------------------------------
The examples can be compiled using the script ``holor.sh``. Optionally, one can specify the compiler to be used with argument ``--compiler``, for example if the default system compiler does not support `C++20`_.

.. code-block:: bash

    ./holor.sh build_examples [--compiler ...]

The compiled binaries are located  in the directory ``build/bin``.


--------------


Tests
^^^^^

.. admonition:: Requirements

    The tests require gtest.




Method 1. Building with CMake
-----------------------------
The compilation of the tests is enabled by setting the cmake option ``-DHOLOR_BUILD_TESTS=ON``. Optionally, one can specify the compiler to be used with the cmake option ``-DCMAKE_CXX_COMPILER``, for example if the default system compiler does not support `C++20`_.

.. code-block:: bash

    cd build
    cmake .. -DHOLOR_BUILD_TESTS=ON [-DCMAKE_CXX_COMPILER=...]
    make



Method 2. Building with script
------------------------------
The tests can be compiled using the script ``holor.sh``. Optionally, one can specify the compiler to be used with argument ``--compiler``, for example if the default system compiler does not support `C++20`_.

.. code-block:: bash

    ./holor.sh build_tests [--compiler ...]

The compiled binaries are located  in the directory ``build/bin``.



--------------



Benchmarks
^^^^^^^^^^
.. admonition:: Requirements

    BLABLA 



Method 1. Building with CMake
-----------------------------
The compilation of the benchmarks is enabled by setting the cmake option ``-DHOLOR_BUILD_BENCHMARKS=ON``. Optionally, one can specify the compiler to be used with the cmake option ``-DCMAKE_CXX_COMPILER``, for example if the default system compiler does not support `C++20`_.

.. code-block:: bash

    cd build
    cmake .. -DHOLOR_BUILD_BENCHMARKS=ON [-DCMAKE_CXX_COMPILER=...]
    make



Method 2. Building with script
------------------------------
The benchmarks can be compiled using the script ``holor.sh``. Optionally, one can specify the compiler to be used with argument ``--compiler``, for example if the default system compiler does not support `C++20`_.

.. code-block:: bash

    ./holor.sh build_benchmarks [--compiler ...]

The compiled binaries are located  in the directory ``build/bin``.




-----------------




Documentation
^^^^^^^^^^^^^

.. admonition:: Requirements

    * sphinx
    * breathe
    * materialdesign theme

The documentation can be built using the following command

.. code-block:: bash

    ./holor.sh docs


The documentation, in html format, will be generated in the ``docs`` folder. To consult it, open the ``index.html`` file in a web browser.




---------------



Removing all optional components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Calling the script ``./holor.sh clean`` will not only uninstall the library, but it will also delete all the optional components, if they were built.

.. _C++20: https://en.cppreference.com/w/cpp/20
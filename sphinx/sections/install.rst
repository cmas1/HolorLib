Installation
============


Before Installing
------------------
HolorLib is a header only library. Therefore the library itself does not need to be compiled to be used in a project. However, the header files are written using **C++20** features and we plan to include more of these features as they become available in compilers. Therefore, a project using HolorLib should be compiled using a compiler that supports C++20.
We did not perform a thorough test to verify which compilers can be used with this library. We successfully compiled all examples and tests shipped in this library using `clang++12`, but we failed to compile them with `g++-10` due to a compiler bug when using concepts in partial template specializations. 



Method 1. Installation without using CMake
------------------------------------------
HolorLib is a header-only library, therefore it can be used by simply copying the header files into your project.
First, download the library from the repository

.. code-block:: bash

   git clone TBD 

Then, copy the ``include`` folder into your project.

Deleting the installation
^^^^^^^^^^^^^^^^^^^^^^^^^
Simply delete the HolorLib header files that you copied in your project.





Method 2. Installation using CMake (Recommended)
------------------------------------------------
.. admonition:: Requirements

   CMake 3.12 or above


HolorLib is shipped as a CMake package, which allows it to be easily installed and used by other CMake projects.
To install the library, first download it from the repository

.. code-block:: bash

   git clone TBD <clone_destination>

Then go into the folder, create the build directory (where the cmake files will be generated), and build the project with cmake

.. code-block:: bash

   cd <clone_destination>/Holor
   mkdir build && cd build
   cmake .. -[options]

There are a few CMake options that can be passed:

* ``-DCMAKE_INSTALL_PREFIX=<path>``. This option can be used to specify the location where the header files will be copied. By default the headers are copied in ``\user\local\install``. If there is no specific reaso to change this, the option can be omitted.
* ``-DDEFINE_HOLOR_ASSERT_LEVEL=<Assertion level>`. This option is used to control what exceptions can be thrown by the HolorLib code. TThere are three levels:
   
   - ``AssertionLevel::release`` is the default setting, and it contains few checks that may throw exceptions (for example, accessing an out of index element of a container).
   - ``AssertionLevel::no_checks`` this setting removes all checks
   - ``AssertionLevel::debug`` this setting enables all checks, including the checks that are added for debuggin. 

We advise to not use the ``DDEFINE_HOLOR_ASSERT_LEVEL`` option.


With the cmake files generated, it is possible to review all options and reconfigure the project using the *CMake curses* interface

.. code-block:: bash

   ccmake .

Finally, the library can be installed by calling

.. code-block:: bash

   cmake --install .

.. note::

   Depending on the destination where the header files will be copied, this last command may require superuser privileges. In that case call it with ``sudo``.


Deleting the installation
^^^^^^^^^^^^^^^^^^^^^^^^^
During the installation with these method, CMake create an ``install_manifest.txt`` file in the build folder which contains the information where the files have been installed.
Therefore, to uninstall the library simply do

.. code-block:: bash

   cd <build directory>
   make uninstall

.. note::
   
   It may be necessary to call the command ``make uninstall`` with superuser privileges (using ``sudo``) depending on how and where the files were installed.

Finally, the build folder with all its content can be deleted.



Method 3. Installation using script (Recommended)
-------------------------------------------------
.. admonition:: Requirements

   CMake 3.12 or above


To simplify installation, and if the user does not need to modify any of the CMake options listed in Method 2., we advise to install the library by using the provided script ``holor.sh``, which will call all the appropriate cmake commands.

First, download the repository

.. code-block:: bash

   git clone TBD <clone_destination>
   cd <clone_destination>/Holor

Make sure that the script ``holor.sh`` is executable and verify its options by calling

.. code-block:: bash

   ./holor.sh --help

From the help the user can see that this script provides a simple interface to not only install the library, but also to build some additional components such as examples and tests. (ADD REFERENCE TO SECTION) 


Install the library by simply calling

.. code-block:: bash

   ./holor.sh install



Deleting the installation
^^^^^^^^^^^^^^^^^^^^^^^^^
If the library was installed via the ``holor.sh`` script, all the installed and build files and optional components can be removed with the same script, calling

.. code-block:: bash

   ./holor.sh clean
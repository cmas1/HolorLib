HolorLib
=============

This is a C++ template, header-only library that implements generic multi-dimensional containers. 
The name of the library pays homage to the work of Parry Moon and Domina Eberle Spencer, who coined the term *Holor*:

   The word holor indicates a mathematical entity that is made up of one or more independent quantities. Examples of holors are complex numbers, vectors, matrices, tensors, and other hypernumbers.
   
   --- Parry Moon and Domina Eberle Spencer  (**Theory of Holors: a generalization of tensors.** [#f1]_)

Namely, Moon and Spencer define holors (from the Greek *ὅλος*, a whole) as a collection of indipendent elements, or merates (from the Greek *μέρος*, a part). These mathematical objects have two fundamental qualities:

- **valence** :math:`N`, which indicates the **number of dimensions** of the entity.
- **plethos** :math:`n`, which indicates the **number of elements in a dimension** of the entity.
   
A simple example of a holor is a 3D matrix :math:`A_{ijk}`:, with three elements in each direction (:math:`N=3` and :math:`n=3` along all axes), as shown in this figure

.. image:: ./images/3dmatrix_example.png
   :scale: 20%
   :alt: Holor example: a 3D matrix
   :align: center
   
The elements of this holor are identified by the three indices, :math:`i`, :math:`j` and :math:`k`. 

The theory of Holors developed by Moon and Spencer not only provides a generic notation for referring to such collection of elements, but it also generalizes algebraic operations like addition and product, transformations and calculus with these entities. They also provide definitions to important classes of holors, like tensors, akinetors, oudors. 

This library does not aim to implement these concepts: **HolorLib is not a mathematical library**. HolorLib only implements generic multidimensional containers, characterized by a number of dimensions (aka the *valence*) and by a number of elements along each dimension (aka the *plethos*). The elements stored in this container may be objects of any type, not just numbers or other mathematical entities. We call this containers *holors* because inspired by Moon and Spencer's work.
HolorLib provides fundamental operations to manipulate and access these containers, such as indexing and slicing. Other operations to manipulate the containers and their content, such as addition or product, are typically application specific and are not implemented in this library interface.


.. admonition:: Thank you

   If you are using Holor as part of your research, teaching, or other activities, we would be grateful if you could star
   the repository.


Documentation Content
---------------------

.. toctree::
   :caption: getting started
   :maxdepth: 1

   sections/holor
   sections/install
   sections/optional
   sections/usage

.. toctree::
   :caption: Examples
   :maxdepth: 1

   sections/cpp_examples


.. toctree::
   :caption: C++ documentation
   :maxdepth: 1

   sections/cpp_docs_index

.. toctree::
   :caption: Changes and license
   :maxdepth: 1

   sections/credits
   sections/changelog



.. [#f1] Parry Moon and Domina Eberle Spencer, *Theory of Holors: a generalization of tensors*, Cambridge, England: Cambridge University Press, 1986
The Holor Library
=================

This is a C++ template, header-only library that implements generic multi-dimensional containers. 
The name of the library pays homage to the work of Parry Moon and Domina Eberle Spencer, who coined the term *Holor*:

   The word holor indicates a mathematical entity that is made up of one or more independent quantities. Examples of holors are complex numbers, vectors, matrices, tensors, and other hypernumbers.
   
   --- Parry Moon and Domina Eberle Spencer  (**Theory of Holors: a generalization of tensors.** [#f1]_)

Namely, Moon and Spencer define holors (from the Greek *ὅλος*, a whole) as a collection of indipendent elements, or merates (from the Greek *μέρος*, a part). These mathematical objects have two fundamental qualities:

- **valence** :math:`N`, which indicates the **number of dimensions** of the entity.
- **plethos** :math:`n`, which indicates the **number of elements in a dimension** of the entity.
   
A simple example of a holor is a 3D matrix:

.. image:: ./images/3dmatrix_example.png
   :scale: 20%
   :alt: Holor example: a 3D matrix
   :align: center
   
The 3D matrix :math:`A_{ijk}` in figure has valence :math:`N=3` and plethos 3 along all axes.

If you are using Holor as part of your research, teaching, or other activities, we would be grateful if you could star
the repository and cite our work as follows...


.. toctree::
   :maxdepth: 2

   sections/holor
   sections/install
   sections/optional
   sections/usage
   sections/cpp_examples
   sections/cpp_docs_index
   sections/credits
   sections/changelog



.. [#f1] Parry Moon and Domina Eberle Spencer, *Theory of Holors: a generalization of tensors*, Cambridge, England: Cambridge University Press, 1986
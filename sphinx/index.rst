The Holor Library
================

This is a C++ template, header-only library that implements generic multi-dimensional containers. 
The name of the library pays homage to the work of Parry Moon and Domina Eberle Spencer, who coined the term *Holor*:

   The word holor (from the Greek *ὅλος*, a whole) indicates a mathematical entity that is made up of one or more independent quantities, called merates (from the Greek *μέρος*, a part). Examples of holors are complex numbers, vectors, matrices, tensors, and other hypernumbers.
   
   --- Parry Moon and Domina Eberle Spencer (**Theory of Holors: a generalization of tensors.** Cambridge, England: Cambridge University Press, 1986.)

Monn and Spencer characterize holors with respect to two qualities:

   - their **valence** :math:`N`, which indicates the **number of dimensions** of the entity.
   - their **plethos** :math:`n`, which indicates the **number of elements in a dimension** of the entity.
   
The Holor library was given its name to pay omage to Mann and Spencer's Theory of Holors, but it is not a mathematical library. An HolorCon is a generic multi-dimensional container endowed with an interface that allows to easiliy slice it and index its elements.


.. image:: ./images/3dmatrix_example.png
  :width: 20%

If you are using Holor as part of your research, teaching, or other activities, we would be grateful if you could star
the repository and cite our work as follows...



Contents:

   getting started
   install
      building Examples, tests and benchmarks
      build the documentation
   What's a Holor container
   Examples
   C++ documentation
   credits
   Changelog



.. toctree::
   :maxdepth: 1

   sections/holor
   sections/install
   sections/optional
   sections/usage
   sections/cpp_examples
   sections/cpp_docs_index
   sections/credits
   sections/changelog

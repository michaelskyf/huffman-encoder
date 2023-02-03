Index
=====

Welcome to ppk-project-huffman's documentation!

.. image:: gplv3.png
   :width: 150
   :alt: GPLv3
   :target: https://www.gnu.org/licenses/gpl-3.0.html

.. Warning:: This project was made for a homework. **Use with caution!**

Short summary of the project
============================

This project can encode and decode files using Huffman coding.

History of the project
======================

This project was made for the practical exam of PPK (Basics of Computer Programming) class
in the Silesian University of Technology. During the classes we had to pick one of fifteen different projects
and finish them before the end of the semester. Additionally we also had to write a documentation
for the project, which is why I write this in the first place. Thanks to that I finally got myself together
and expanded my previous Cmake-based build system by including an automated documentation generation. 

Lessons learned
===============

Unfortunately, I've made two big mistakes while making the project.
The first mistake was creating bloated, monolithic functions ( encode() and decode() ) and the second one was not using TDD (Test Driven Development).
These two errors made me spend quite a long time debugging and testing the application with an unsatisfactory final result.

Links
=====

* `Project's page on GitHub <https://github.com/michaelskyf/ppk-project-huffman>`_
* `Project's documentation on GitHub Pages <https://michaelskyf.github.io/ppk-project-huffman>`_ (**better than the pdf version**)

Documentation
=============

.. toctree::
   :caption: Building the Project
   :maxdepth: 2
   :hidden:

   building/dependencies.rst
   building/compiling.rst
   building/tests.rst
   building/documentation.rst

.. toctree::
   :caption: Usage
   :maxdepth: 2
   :hidden:

   usage/compression.rst
   usage/decompression.rst

.. toctree::
   :caption: Source code documentation
   :maxdepth: 2
   :hidden:

   source_code/index.rst
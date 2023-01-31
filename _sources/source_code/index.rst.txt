Source code documentation
=========================

The program is divided into 3 files: main.cpp, huffman.cpp and huffman.hpp.

main.cpp
--------

Parses arguments given on the command line and according to them executes compress(), or decompress().
This file also has functions write_huffman_tree_json() and read_huffman_tree_json() which save/read the dictionary to/from a file.

huffman.hpp
-----------

Declares HuffmanDictionary class and its internal structcure.

huffman.cpp
-----------

Defines methods of class HuffmanDictionary.

:doc:`Doxygen source code documentation <source>` 
-------------------------------------------------

.. toctree::
   :caption: Doxygen source code documentation
   :maxdepth: 2
   :hidden:

   source.rst
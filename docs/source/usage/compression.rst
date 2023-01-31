Compression
===========

To compress a file you need to:

* Pass the name of the file that you want to compress (-i, --input)

* Pass the name of the compressed file (-o, --output)

* Pass the name of the file where the dictionary will be written (-s, -d, --dictionary)

* Set the mode to compression (-t, -m, --mode -> c)

Example:

.. code-block:: bash

	./bin/huffman-compression -i input.txt -o compressed.txt -d dictionary.txt -m c
Decompression
=============

To decompress a file you need to:

* Pass the name of the file that you want to decompress (-i, --input)

* Pass the name of the decompressed file (-o, --output)

* Pass the name of the file containing dictionary (-s, -d, --dictionary)

* Set the mode to decompression (-t, -m, --mode -> d)

Example:

.. code-block:: bash

	./bin/huffman-compression -i compressed.txt -o decompressed.txt -d dictionary.txt -m d
# Huffman Compression

### How to run the code
1. Download all the files in the same folder.
2. Overwrite the input.txt file with the data you wish to compress
3. You can change the compression type as per your choice: Open the compression.cpp file, and change the value of "const int tree", 2 for Binary, 4 for Quadnary, 8 for Octanary, 16 for Hexanary...
4. Run the following commands:
- g++ -o compression compression.cpp
- compression input.txt
- g++ -o decompression decompression.cpp
- decompression input-reyaan_compressed_it.bin


### .bin file
1. 3txt (3 is the size of the type, and txt is the type of file)
2. a0011NULL (a is character, 0011 is variable length code, NULL is to know that one character has ended)
3. b0101NULL
4. c0011NULL
5. NULL      (NULL if appears twice, we know the codes are ended)
6. padding   (Padding)
7. NULL      (Null to know Padding is over)
8. 000000000 11111111 00000000 11111111 (The data)

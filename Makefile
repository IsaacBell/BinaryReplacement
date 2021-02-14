all : binary_replacement
# all : compress decompress binary_replacement

binary_replacement : BinaryReplacement.cpp
	g++ -std=c++14 BinaryReplacement.cpp src/BinaryStringTranslation.cpp -o BinaryReplacement	

# compress : Compressor.cpp
# 	g++ -std=c++14 Compressor.cpp -o compress

# decompress : Decompressor.cpp
# 	g++ -std=c++14 Decompressor.cpp -o decompress

clean :
	# @rm -f compress
	# @rm -f decompress
	@rm -f binary_replacement

.PHONY : all clean

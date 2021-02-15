all : my_frame
# all : compress decompress binary_replacement

my_frame : MyFrame2.cpp
	g++ -std=c++14 src/MyFrame2.cpp `wx-config --libs --cxxflags` -o my_frame

# binary_replacement : BinaryReplacement.cpp
# 	g++ -std=c++14 BinaryReplacement.cpp src/BinaryStringTranslation.cpp -o BinaryReplacement	

# compress : Compressor.cpp
# 	g++ -std=c++14 Compressor.cpp -o compress

# decompress : Decompressor.cpp
# 	g++ -std=c++14 Decompressor.cpp -o decompress

clean :
	# @rm -f compress
	# @rm -f decompress
	@rm -f binary_replacement

.PHONY : all clean

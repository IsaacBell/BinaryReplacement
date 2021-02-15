#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

class BinaryReplacement {
private:
  void write_from_uChar(unsigned char,unsigned char&,int,FILE*);

  int this_is_not_a_folder(char*);
  long int size_of_the_file(char*);
  void count_in_folder(std::string,long int*,long int&,long int&);

  void write_file_count(int,unsigned char&,int,FILE*);
  void write_file_size(long int,unsigned char&,int,FILE*);
  void write_file_name(char*,std::string*,unsigned char&,int&,FILE*);
  void write_the_file_content(FILE*,long int,std::string*,unsigned char&,int&,FILE*);
  void write_the_folder(std::string,std::string*,unsigned char&,int&,FILE*);
public:
  void translate(std::string binary);
};

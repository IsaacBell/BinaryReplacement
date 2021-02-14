#include <map>
#include <string>
#include <iostream>
#include "BinaryStringTranslation.h"

std::map<std::string, std::string> BinaryStringTranslation::swap_list = {
  {"0000", "---- "},
  {"0001", "---2"}, 
  {"0010", "--2-"}, 
  {"0011", "--22"}, 
  {"0100", "-2--"}, 
  {"0101", "-2-2"}, 
  {"0110", "-22-"}, 
  {"0111", "-222"}, 
  {"1000", "2---"}, 
  {"1001", "2--2"}, 
  {"1010", "2-2-"}, 
  {"1011", "2-22"}, 
  {"1100", "22--"}, 
  {"1101", "22-2"}, 
  {"1110", "222-"}, 
  {"1111", "2222"}
};

std::string BinaryStringTranslation::translateBinary(std::string input)
{
  std::map<std::string, std::string>::iterator it = swap_list.find(input);
  std::cout<<"\n"<<it->second<<"\n";
  // If the string is in the swap list, return the swapped output
  // Otherwise, return the original input
  return it != swap_list.end() ? it->second : input;
};
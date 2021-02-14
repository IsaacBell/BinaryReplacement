#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <map>

#include "src/BinaryStringTranslation.h"

void write_from_uChar(unsigned char,unsigned char&,int,FILE*);

int this_is_not_a_folder(char*);
long int size_of_the_file(char*);
void count_in_folder(std::string,long int*,long int&,long int&);

void write_file_count(int,unsigned char&,int,FILE*);
void write_file_size(long int,unsigned char&,int,FILE*);
void write_file_name(char*,std::string*,unsigned char&,int&,FILE*);
void write_the_file_content(FILE*,long int,std::string*,unsigned char&,int&,FILE*);
void write_the_folder(std::string,std::string*,unsigned char&,int&,FILE*);

/*
Below function is used for writing the uChar to compressed file
It does not write it directly as one byte instead it mixes uChar and current byte,
writes 8 bits of it and puts the rest to curent byte for later use
*/
void write_from_uChar(
  unsigned char uChar,
  unsigned char &current_byte,
  int current_bit_count,
  FILE *fp_write
){
  current_byte<<=8-current_bit_count;
  current_byte|=(uChar>>current_bit_count);
  fwrite(&current_byte,1,1,fp_write);
  current_byte=uChar;   
}

/*
Below function is writing number of files we re going to translate inside current folder to compressed file's 2 bytes

It is done like this to make sure that it can work on little, big or middle-endian systems
*/
void write_file_count(
  int file_count,
  unsigned char &current_byte,
  int current_bit_count,
  FILE *compressed_fp
){
  unsigned char temp=file_count%256;
  write_from_uChar(temp,current_byte,current_bit_count,compressed_fp);
  temp=file_count/256;
  write_from_uChar(temp,current_byte,current_bit_count,compressed_fp);
}

// Unused in this iteration
void write_file_size(
  long int size,
  unsigned char &current_byte,
  int current_bit_count,
  FILE *compressed_fp
){
  for(int i=0;i<8;i++){
    write_from_uChar(size%256,current_byte,current_bit_count,compressed_fp);
    size/=256;
  }
}

// This function writes bytes that are translated from current input file's name to the compressed file.
void write_file_name(
  char *file_name,
  std::string *str_arr,
  unsigned char &current_byte,
  int &current_bit_count,
  FILE *compressed_fp
){
    write_from_uChar(strlen(file_name),current_byte,current_bit_count,compressed_fp);
    char *str_pointer;
    for(char *c=file_name;*c;c++){
        str_pointer=&str_arr[(unsigned char)(*c)][0];
        while(*str_pointer){
            if(current_bit_count==8){
                fwrite(&current_byte,1,1,compressed_fp);
                current_bit_count=0;
            }
            switch(*str_pointer){
                case '1':current_byte<<=1;current_byte|=1;current_bit_count++;break;
                case '0':current_byte<<=1;current_bit_count++;break;
                default:std::cout<<"An error has occurred\nProcess has been aborted";
                exit(2);
            }
            str_pointer++;
        }
    }
}

// Translate and write bytes from current input file to output file.
void write_the_file_content(
  FILE *original_fp,
  long int size,
  std::string *str_arr,
  unsigned char &current_byte,
  int &current_bit_count,
  FILE *compressed_fp
){
  unsigned char *temp,x;
  temp=&x;
  char *str_pointer;
  fread(temp,1,1,original_fp);
  // std::cout<<"\nCurrent byte: "<<*temp;
  for(long int i=0;i<size;i++){
    str_pointer=&str_arr[x][0];
    while(*str_pointer) {
      if(current_bit_count == 8){
        fwrite(&current_byte,1,1,compressed_fp);
        current_bit_count=0;
      }
      switch(*str_pointer){
        case '1':current_byte<<=1;current_byte|=1;current_bit_count++;break;
        case '0':current_byte<<=1;current_bit_count++;break;
        default:std::cout<<"An error has occurred\nProcess has been aborted";
        exit(2);
      }
      str_pointer++;
    }
    fread(temp,1,1,original_fp);
  }
}

int this_is_not_a_folder(char *path){
  DIR *temp=opendir(path);
  if(temp){
    closedir(temp);
    return 0;
  }
  return 1;
}

long int size_of_the_file(char *path){
  long int size;
  FILE *fp=fopen(path,"rb");
  fseek(fp,0,SEEK_END);
  size=ftell(fp);
  printf ("Size: %ld bytes.\n",size);
  fclose(fp);
  return size;
}

// Counts usage frequency of bytes inside a folder given folder path as input
void count_in_folder(
  std::string path,
  long int *number,
  long int &total_size,
  long int &total_bits
) {
    FILE *original_fp;
    path+='/';
    DIR *dir=opendir(&path[0]),*next_dir;
    std::string next_path;
    total_size+=4096;
    total_bits+=16; //for file_count
    struct dirent *current;
    while((current=readdir(dir))){
      if(current->d_name[0]=='.'){
          if(current->d_name[1]==0)continue;
          if(current->d_name[1]=='.'&&current->d_name[2]==0)continue;
      }
      total_bits+=9;

      for(char *c=current->d_name;*c;c++){
        // count usage frequency of bytes on the file name (or folder name)
        number[(unsigned char)(*c)]++;
      }

      next_path=path+current->d_name;

      if((next_dir=opendir(&next_path[0]))){
        closedir(next_dir);
        count_in_folder(next_path,number,total_size,total_bits);
      }
      else{
        long int size;
        unsigned char *temp,x;
        temp=&x;
        total_size+=size=size_of_the_file(&next_path[0]);
        total_bits+=64;

        original_fp=fopen(&next_path[0],"rb");

        fread(temp,1,1,original_fp);
        for(long int j=0; j<size; j++){
          // count usage frequency of bytes inside the file
          number[x]++;
          fread(temp,1,1,original_fp);
        }
        fclose(original_fp);
      }
  }
  closedir(dir);
}

void write_the_folder(
  std::string path,
  std::string *str_arr,
  unsigned char &current_byte,
  int &current_bit_count,FILE *compressed_fp
){
  FILE *original_fp;
  path+='/';
  DIR *dir=opendir(&path[0]),*next_dir;
  std::string next_path;
  struct dirent *current;
  int file_count=0;
  long int size;

  while((current=readdir(dir))){
    if(current->d_name[0]=='.'){
      if(current->d_name[1]==0)continue;
      if(current->d_name[1]=='.'&&current->d_name[2]==0)continue;
    }
    file_count++;
  }
  rewinddir(dir);
  write_file_count(file_count,current_byte,current_bit_count,compressed_fp);

  while((current=readdir(dir))){
    // if current is a file
    if(current->d_name[0]=='.'){
        if(current->d_name[1]==0)continue;
        if(current->d_name[1]=='.'&&current->d_name[2]==0)continue;
    }

    next_path=path+current->d_name;
    if(this_is_not_a_folder(&next_path[0])){

        original_fp=fopen(&next_path[0],"rb");
        fseek(original_fp,0,SEEK_END);
        size=ftell(original_fp);
        rewind(original_fp);

        if(current_bit_count==8){
          // std::cout<<"Current byte: "<<current_byte<<"\n";
          fwrite(&current_byte,1,1,compressed_fp);
          current_bit_count=0;
        }
        current_byte<<=1;
        current_byte|=1;
        current_bit_count++;

        write_file_size(size,current_byte,current_bit_count,compressed_fp);
        write_file_name(current->d_name,str_arr,current_byte,current_bit_count,compressed_fp);
        write_the_file_content(original_fp,size,str_arr,current_byte,current_bit_count,compressed_fp);
        fclose(original_fp);
    }
    else {
      // if current is a folder
      if(current_bit_count == 8) {
          fwrite(&current_byte,1,1,compressed_fp);
          current_bit_count=0;
      }
      current_byte<<=1;
      current_bit_count++;

      write_file_name(current->d_name,str_arr,current_byte,current_bit_count,compressed_fp);

      write_the_folder(next_path,str_arr,current_byte,current_bit_count,compressed_fp);
    }
  }
  closedir(dir);
}

int main(int argc,char *argv[]){
  long int number[256];
  long int total_bits=0;
  int letter_count=0;

  std::string output;

  if (argc==1){
    std::cout<<"Missing file name\n\nTry './archive {{file_name}}'\n";
    return 0;
  }
  for(long int *i=number;i<number+256;i++){                       
    *i=0;
  }

  std::string scompressed;
  FILE *original_fp,*compressed_fp;

  for(int i=1;i<argc;i++){
    //checking for wrong input
    if(this_is_not_a_folder(argv[i])){
      original_fp=fopen(argv[i],"rb");
      if(!original_fp){
        std::cout<<argv[i]<<" - file does not exist\nProcess has been terminated\n";
        return 0;
      }
      fclose(original_fp);
    }
  }

  scompressed=argv[1];
  scompressed += ".output.txt";

  // these are temp variables to take input from the file
  unsigned char *temp,x;
  temp=&x;

  long int total_size=0,size;
  total_bits+=16+9*(argc-1);

  for(int current_file=1;current_file<argc;current_file++){
    for(char *c=argv[current_file];*c;c++) {
      // count usage frequency of unique bytes on the file name (or folder name)
      // irrelevant for now, this is just my usual pattern
      // I just felt like keeping it in
      number[(unsigned char)(*c)]++;
    }

    if(this_is_not_a_folder(argv[current_file])){
      total_size+=size=size_of_the_file(argv[current_file]);
      total_bits+=64;

      original_fp=fopen(argv[current_file],"rb");
      fread(temp,1,1,original_fp);

      int size_limit = std::min(size, (long int) 4);
      std::string chars = "";

      // store first 4 chars from text file
      for(long int j=0;j<size_limit;j++) {
        chars.push_back(*temp);
        number[x]++;
        fread(temp,1,1,original_fp);
      }
      BinaryStringTranslation translater;
      output = translater.translateBinary(chars);
      std::cout<<"output: "<<output<<"\n";
      fclose(original_fp);
    }
    else{
      std::string temp=argv[current_file];
      count_in_folder(temp,number,total_size,total_bits);
    }
  }

  for(long int *i=number;i<number+256;i++){                 
        if(*i){
    letter_count++;
    }
  }

  std::ofstream out(scompressed);
  out << output;
  out.close();

  return 0;
}
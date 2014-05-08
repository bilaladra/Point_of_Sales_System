// Author: S. Renk 
// random access file engine
// file: RFile.h
#include <iostream>
#include <fstream>
using namespace std;

class RFile
{ // general purpose random access file object
   fstream fio; 
   int recCount, recSize;
   int status;
 public:
   RFile(char * filename, int size);
   RFile();
  ~RFile();
   bool Fopen(char * filename, int size);
   void Fclose();
   bool read(int recNbr, void * blob); // read a rec
   bool update(int recNbr, void * blob);// rewrite a rec
   bool write(void * blob);            // add new rec
   int count(){return recCount;}       // # of allocated recs
   int stat(){return status;}          // -1 not open, 0 open,1 open failure
};                                     

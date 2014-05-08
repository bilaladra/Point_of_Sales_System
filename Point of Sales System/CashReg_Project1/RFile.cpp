// Author: S. Renk 
// Random Access file engine
// file: RFile.cpp

#include "RFile.h"

RFile::RFile(char *filename, int size)
{ // open file for I/O & initialize
  fio.open(filename, ios::in | ios::out | ios::binary);
  recSize = size;
  if (!fio)
  {  status = 1;  // open failure
     recCount = 0;
  }
  else
  {  fio.seekg(0, ios::end);  // goto eof
     recCount = fio.tellg()/recSize;
     status = 0;
  }
}

RFile::RFile()
{ 
	status = -1; // file not open
}

RFile::~RFile()
{ // close the file
  if (status == 0)
      fio.close();
}

bool RFile::Fopen(char *filename, int size)
{ // open file for I/O & initialize
  bool rtn = true;
  
  fio.open(filename, ios::in | ios::out | ios::binary);
  recSize = size;
  if (!fio)
  {  status = 1;  // open failure
     recCount = 0;
	 rtn = false;
  }
  else
  {  fio.seekg(0, ios::end);  // goto eof
     recCount = fio.tellg()/recSize;
     status = 0;
  }
  return rtn;
}

void RFile::Fclose()   // close file & set status
{
  fio.close();
  status = -1;         // set to not open
  recCount = recSize = 0; // clear all
}

bool RFile::read(int recNbr, void * blob)
{ // read some data from rec recNbr in the file
  bool rtn = true;

  if (recNbr > recCount-1 || recNbr < 0 || status )
     rtn = false;
  else
  {  fio.seekg(recNbr * recSize);  // find record
     fio.read((char *)blob, recSize);
  }
  return rtn;
}

bool RFile::update(int recNbr, void * blob)
{ // rewrite existing rec recNbr in the file
  bool rtn = true;

  if (recNbr > recCount-1 || recNbr < 0 || status)
     rtn = false;
  else
  {  fio.seekp(recNbr * recSize);  // find record
     fio.write((char *)blob, recSize);
  }
  return rtn;
}

bool RFile::write(void * blob)
{ // write some data to the end of file
  bool rtn = true;

  if (status != 0)
     rtn = false;
  else
  {  fio.seekp(0, ios::end);  // find eof
     fio.write((char *)blob, recSize);
     recCount++;
  }
  return rtn;
}

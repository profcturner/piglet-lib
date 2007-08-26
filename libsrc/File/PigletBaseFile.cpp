/**
** Piglet Productions
**
** FileName       : PigletBaseFile.cpp
**
** Implements     : PigletBaseFile
**
** Description
**
**
**
**
** Initial Coding : Colin Turner
**
** Date           : 11th May 1999
**
**
** Copyright applies on this file, and distribution may be limited.
*/

/*
** Revision 1.00
**
*/

#include <PigletFile.h>

PigletBaseFile::PigletBaseFile()
{
  memset(this, 0, sizeof(PigletBaseFile));
}


PigletBaseFile::PigletBaseFile(char * filename, char * access, int share)
{
  File = Open(filename, access, share);
}


int PigletBaseFile::Seek(long int offset, int whence)
{
  return(fseek(File, offset, whence));
}


long int PigletBaseFile::GetOffset()
{
  return(ftell(File));
}


long int GetLength()
{
  long current = Tell();
  long end;

  Seek(0, SEEK_END);
  end = Tell();

  Seek(current, SEEK_SET);

  return(end);
}


int PigletBaseFile::Open(char * filename, char * access, int share)
{
  Close();
  File = _fsopen(filename, access, share);
}
  

int PigletBaseFile::Close()
{
  if(File) fclose(File);
}


void PigletBaseFile::SignalError(int errorCode)
{
  Error = errorCode;
}


void ClearError()
{
  Error = 0;
}
  

int GetError()
{
  return(Error);
}


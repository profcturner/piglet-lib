/**
** Piglet Productions
**
** FileName       : PigletFile.h
**
** Defines        : PigletFile
**
** Description
**
**
**
**
** Initial Coding : Colin Turner
**
** Date           : 6th May 1999
**
**
** Copyright applies on this file, and distribution may be limited.
*/

/*
** Revision 1.00
**
*/

#ifndef PigletFile_H
#define PigletFile_H

// Standard runtime library includes

// Piglet Productions includes

#include <PigletDefs.h>

class PigletBaseFile;
class PigletReadFile;
class PigletWriteFile;
class PigletFile;


class PigletBaseFile
{
  // Data
private:

  FILE * File;
  int    Error;
  int    Status;

  // Services
public:

  PigletBaseFile();
  PigletBaseFile(char * filename, char * access, int share);

  int      Seek(long int offset, int whence);
  long int GetOffset();
  long int GetLength();

  virtual int Open(char * filename, char * access, int share);
  virtual int Close();

  // Error Handling
  virtual void SignalError(int errorCode);
  virtual void ClearError();
  int GetError();

};


class PigletReadFile : public PigletBaseFile
{
  // Services
public:

  PigletReadFile(char * filename, char * access = "rb", SH_DENYWR);
  // Byte input
  int GetC();

  // Block input
  size_t Read(void * buffer, size_t elsize, size_t nelem);

  // Formatted input
  int Scanf(const char * format, ...);
  
};


class PigletWriteFile : public PigletBaseFile
{
  // Services
public:

  PigletWriteFile(char * filename, char * access = "wb", SH_DENYRW);
  // Byte input
  int PutC(int c);

  // Block input
  size_t Write(void * buffer, size_t elsize, size_t nelem);

  // Formatted input
  int Printf(const char * format, ...);

};


class PigletFile : public PigletReadFile, public PigletWriteFile
{
  // Services
public:

  PigletFile(char * filename, char * access = "r+b", SH_DENYRW);
};
































































































































class PigletFile
{
  // Data
protected:
  FILE * File;
  int    Error;

  // Services
public:

  // Byte input / output
  int GetC();
  int PutC();

  // Buffer input / output
  size_t Read(void * buffer, size_t bread, size_t nread = 1);
  size_t Write(void * buffer, size_t bread, size_t nread = 1);

  // Formatted input / output
  int Printf(const char * format, ...);
  int Scanf(const char * format, ...);

  // Basic stuff
  int  Seek(long offset, int whence);
  long Tell();

  int Close();

  int Open(char * filename, char * mode, char * share);

  int GetStatus();
  int GetError();

  virtual void SignalError();
  virtual void ClearError();


  // Implementation

  // Enumeration

};


#endif // (PigletFile_H)

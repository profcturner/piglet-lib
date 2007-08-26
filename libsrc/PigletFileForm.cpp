/**
** Piglet Productions
**
** FileName       : PigletFileForm.cpp
**
** Implements     : PigletFile (*)
**
** Description
**
** Implements the formatted file IO commands for the PigletFile class.
**
** Initial Coding : Colin Turner
**
** Date           : 8th August 1999
**
**
** Copyright applies on this file, and distribution may be limited.
*/

/*
** Revision 1.00
**
*/


#include <PigletFile.h>

                               
int Printf(const char * format, ...)
{
  int success;

  if(!Open) return (-1);

  va_list ArgList;
  va_start(ArgList, format);
  success = vfprintf(File, format, ArgList);
  va_end(ArgList);

  return(success);
}


int Scanf(const char * format, ...)
{
  int success;

  if(!Open) return(-1);

  va_list ArgList;
  va_start(ArgList, format);
  success = vfscanf(File, format, ArgList);
  va_end(ArgList);

  return(success);
}


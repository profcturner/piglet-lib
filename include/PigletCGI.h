/*
** Piglet Productions
**
** FileName       : CGI.H
**
** Defines        : PigletCGI
**
** Description
**
** PigletCGI is an object which decodes and loads variables passed to a CGI
** script.
**
**
** Initial Coding : Colin Turner
**
** Date           : 25th December 1998
**
**
** Copyright applies on this file, and distribution may be limited.
** Copyright 1998
*/

/*
** Revision 1.00
**
*/

#ifndef Piglet_CGI_H
#define Piglet_CGI_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
** The flag settings which can be assigned to the object
*/
                                                                     
#define PigletCGI_NoGet  0x00000001L  // Disallow GET Request Method
#define PigletCGI_NoPost 0x00000002L  // Disallow POST Request Method


class PigletCGI
{
  // Data

private:

  char * Parameters;          // Pointer to space which stores all parameters
  
  char * EnvRequestMethod;    // Environment variable REQUEST_METHOD
  char * EnvQueryString;      // Environment variable QUERY_STRING
  char * EnvPathInfo;         // Environment variable PATH_INFO
  char * EnvContentLength;    // Environment variable CONTENT_LENGTH
  char * EnvContentType;      // Environment variable CONTENT_TYPE

  char * ParsedParameter;     // Last stored decoded parameter

  unsigned long MaxMemory;    // Maximum memory allocation allowed for class
  unsigned long MemRequired;  // Memory required for parameter space
  int Encoded;                // Whether or not the data is encoded

  int Error;                  // Last error which occured

  int Enabled;                // Whether the class should honour requests.

  char * CurrentParameter;    // Pointer to the current parameter being parsed

  long   Flags;               // Internal class Flags

  // Services

public:
  
  PigletCGI();
  PigletCGI(unsigned long flags, unsigned long maxMemory);
  ~PigletCGI();

  char * GetVariable(char * variable);
  char * GetVariable(char * variable, char * buffer);

  int    GetFirstVariable(char * variable, char * value);
  int    GetNextVariable(char * variable, char * value);

  unsigned long GetVariableLength(char * variable);

  unsigned long GetParameterSpaceLength();
  char * GetParameterSpace();

  void PrintDebugInfo();

  int  GetError();
  void ClearError();

  // Implementation

private:
  
  int    Constructor(unsigned long flags, unsigned long maxMemory);
  int    LoadParameters();

  char * GetVariablePosition(char * variable);
  int    DecodeUntil(char ** from, char * to, char until);

  void   SignalError(int error);

  // Enumeration
};

#endif

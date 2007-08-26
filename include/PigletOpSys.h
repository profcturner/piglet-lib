/**
** Piglet Productions
**
** FileName       : OpSys.h
**
** Defines        : Piglet_OpSys
**
** Description
**
** Operating System detection and time slicing.
**
**
** Initial Coding : Colin Turner
**
** Date           : 3rd Febuary 1999
**
**
** Copyright applies on this file, and distribution may be limited.
** Copyright 1999 Colin Turner
*/

/*
** Revision 1.00
**
*/

#ifndef PigletOpSys_H
#define PigletOpSys_H

#include <i86.h>
#include <string.h>


class PigletOpSys
{
  // Data

private:

  int TimeSliceEnabled;
  int Enabled;

  int OpSysCode;          // Unique operating system identifier

  unsigned short OpSysMajVer;
  unsigned short OpSysMinVer;

  char OpSysName[20];
  char OpSysVersion[20];

  // Services

public:

  void TimeSlice();
  void DisableMultiTasker();
  void SetMultiTasker();

  char * GetName();
  char * GetVersion();

  PigletOpSys();


  // Implementation

private:

  PigletOpSys();
  //~PigletOpSys();

  void DetectMultiTasker();

  // Enumeration

public:

  enum{
    OS_None = 0,
    OS_DOS,
    OS_OS2,
    OS_Win16,
    OS_Win32,
    OS_WinNT,
    OS_DESQview,
    OS_DoubleDOS
  };

};

#endif // (PigletOpSys_H)

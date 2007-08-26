/**
** Piglet Productions
**
** FileName       : PigletMemWatch.h
**
** Defines        : PigletMemoryWatch
**
** 
**
** Description
**
**
**
**
** Initial Coding : Colin Turner
**
** Date           : ??
**
**
** Copyright applies on this file, and distribution may be limited.
*/

/*
** Revision 1.00
**
*/

#ifndef PigletMemWatch_H
#define PigletMemWatch_H

class PigletMemoryWatcher
{

  unsigned long MemAvailable;
  unsigned long MaxAvailable;

  public:

	 PigletMemoryWatcher();
	 ~PigletMemoryWatcher();
};

#endif // (PigletMemWatch_H)
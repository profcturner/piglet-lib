/**
** Piglet Productions
**
** FileName       : PigletMemWatch.cpp
**
** Implements     : PigletMemoryWatch
**
** Description
**
**
**
**
** Initial Coding : Colin Turner
**
** Date           : 5/2/1999
**
**
** Copyright applies on this file, and distribution may be limited.
*/

/*
** Revision 1.00
**
*/



#include <stdio.h>
#include <PigletMemWatch.h>

#if defined(__BORLANDC__)
#include <alloc.h>
#include <share.h>
#elif defined(__WATCOMC__)
#include <malloc.h>
#include <share.h>
#else
#error Unsupported Compiler
#endif

PigletMemoryWatcher::PigletMemoryWatcher()
{
  #if defined(__BORLANDC__)
  MemAvailable = coreleft();
  MaxAvailable = 0;
  #elif defined(__WATCOMC__)
  MemAvailable = _memavl();
  MaxAvailable = _memmax();
  #endif
}


PigletMemoryWatcher::~PigletMemoryWatcher()
{
  FILE *fp;
  #if defined(__BORLANDC__)
  if((MemAvailable != coreleft()) || (MaxAvailable != 0)){
   fp = _fsopen("MEMWATCH.ERR", "wb", SH_DENYNONE);
   if(fp){
    fprintf(fp, "Start Mem : %9lu; End Mem : %9lu\n\r", MemAvailable, coreleft());
   }
   printf("\n! WARNING ! Potential Memory Leak\n");
   printf("Start Mem : %9lu; End Mem : %9lu\n", MemAvailable, coreleft());
   fclose(fp);
  }
  #elif defined(__WATCOMC__)
  if((MemAvailable != _memavl()) || (MaxAvailable != _memmax())){
   fp = _fsopen("MEMWATCH.ERR", "wb", SH_DENYNO);
   if(fp){
    fprintf(fp, "Start Mem : %9lu; End Mem : %9lu\n\r", MemAvailable, _memavl());
    fprintf(fp, "Start Max : %9lu; End Max : %9lu\n\r", MaxAvailable, _memmax());
   }
   printf("\n! WARNING ! Potential Memory Leak\n");
   printf("Start Mem : %9lu; End Mem : %9lu\n", MemAvailable, _memavl());
   printf("Start Max : %9lu; End Max : %9lu\n", MaxAvailable, _memmax());
   fclose(fp);
  }
  #endif
}

/**
** Piglet Productions
**
** FileName       : PigletPreamble.h
**
** Description
**
** This file is intended to be included by all future revisions of Piglet
** Productions header files. It will hopefully make future code more
** immediately portable - to GNU C and other platforms.
**
**
** Initial Coding : Colin Turner
**
** Date           : 19th August 2000
**
**
** Copyright applies on this file, and distribution may be limited.
*/

/*
** Revision 1.00
**
*/

#ifndef Piglet_Premable_H
#define Piglet_Preamble_H

#include <stdio.h>

// Specify GNU modifications
#ifdef __GNUC__
  #define stricmp  strcasecmp
  #define strnicmp strncasecmp
#endif

// Specify WATCOM modifications
#ifdef __WATCOMC__
  #define SH_DENYNONE SH_DENYNO
#endif

// Global Piglets function prototypes

FILE * PigletRetryOpen(const char * name, const char * mode, int tries=5);


#endif // Piglet_Preamble_H



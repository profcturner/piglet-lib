/****************************************************************************/
/*                                                                          */
/* Command Line Parameter Parsing                                           */
/*                                                                          */
/* Copyright 1996,1997 Colin Turner                                         */
/*                                                                          */
/* Revision 001                                                             */
/*                                                                          */
/* Associated Documentation, PARAM.DOC                                      */
/*                                                                          */
/* Limited Distribution                                                     */
/*                                                                          */
/****************************************************************************/

#ifndef G_PARAM_H
#define G_PARAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

// Are we extracting from command line or environment ?
// These are used where a mode value is passed to the function.

#define PARAMC  0
#define PARAME  1
#define PARAMCE 2

int  ParamGetPath(char *parameter, char *path);
int  CheckParameter(char *parameter, char *test);
int  CheckParameter(char *parameter, char *test, int *value, int mode);
int  CheckParameter(char *parameter, char *test, long *value, int mode);
int  CheckParameter(char *parameter, char *test, char *value, int mode);
int  CheckAlienParam(char *parameter, char *v1, ...);
void CapatilizeParam(int argc, char *argv[]);

#endif

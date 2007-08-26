/****************************************************************************/
/*                                                                          */
/* General GOLD Library header file                                         */
/*                                                                          */
/* Copyright 1998 Colin Turner                                              */
/*                                                                          */
/* Revision 001                                                             */
/*                                                                          */
/* Associated Documentation, GOLDLIB.DOC                                    */
/*                                                                          */
/* Limited Distribution                                                     */
/*                                                                          */
/****************************************************************************/

#ifndef G_CORE_H
#define G_CORE_H

/*
** General includes
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** Compiler specific includes
*/

#ifdef __WATCOMC__
  #include <i86.h>
#endif // (__WATCOMC__)

/*
** General structures and small classes
*/

class Resolve
{
  private:
    char * Redir[26];

  public:
    Resolve();
    ~Resolve();
    void Refresh();
    char * Tname(char *original, char *buffer);

  private:
    void Fill();
    void Empty();
};


/*
** Compiler specific structures
*/

#ifdef __WATCOMC__

struct COUNTRY{
   short co_date;             /* date format */
   char  co_curr[5];           /* currency symbol */
   char  co_thsep[2];          /* thousands separator */
   char  co_desep[2];          /* decimal separator */
   char  co_dtsep[2];          /* date separator */
   char  co_tmsep[2];          /* time separator */
   char  co_currstyle;         /* currency style */
   char  co_digits;            /* significant digits in currency */
   char  co_time;              /* time format */
   long  co_case;              /* case map */
   char  co_dasep[2];          /* data separator */
   char  co_fill[10];          /* filler */
};

#endif // (__WATCOMC__)

// COUNTRY.CPP

int PigletGetCountry(struct COUNTRY * Country);

// CHKFILE.CPP

struct find_t *CheckFile(char * Filename);


#endif // (G_CORE_H)

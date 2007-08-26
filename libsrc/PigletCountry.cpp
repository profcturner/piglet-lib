/**
** Piglet Productions
**
** FileName       : PigletCountry.cpp
**
** Description
**
** Allows programs to obtain country dependent information. Works for Watcom
** and for Windows32.
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

#include <PigletCore.h>

#if defined(__NT__)
#include <windows.h>
#endif

#if defined(__BORLANDC__)
#include <dos.h>
#endif

/*
**    PigletGetCountry
**
** A function to extract country related information from
** the Operating System. This simply calls a similar
** function in Borland C++, or executes an appropriate
** interrupt in Watcom C++.
**
**    Parameters
**
**    Country   A pointer to a country structure, as
**              defined in g_core.h. To be filled.
**
**    Returns
**
**    0 on failure, non zero otherwise.
**
*/
int PigletGetCountry(COUNTRY * Country)
{
  #if defined(__NT__)
  char TinyBuffer[20];

  memset(Country, 0, sizeof(COUNTRY));
  if(GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDATE, TinyBuffer, 20))
    Country->co_date = atoi(TinyBuffer);
  GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, Country->co_curr, 5);
  GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, Country->co_thsep, 2);
  GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL,  Country->co_desep, 2);
  GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDATE,     Country->co_dtsep, 2);
  GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIME,     Country->co_tmsep, 2);
  if(GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, TinyBuffer, 20))
    Country->co_time = atoi(TinyBuffer);

  return(1);
  #else // (__NT__)
    #if defined(__WATCOMC__)
    union REGPACK  regs;

    regs.h.ah=0x38;
    regs.h.al=0;

    regs.w.ds = FP_SEG(Country);
    regs.w.dx = FP_OFF(Country);

    intr(0x21, &regs);

    return((regs.x.flags & INTR_CF) !=0);
    #elif defined(__BORLANDC__)
      return(country(0, Country)!=NULL);
    #else
      #error unsupported compiler
    #endif
  #endif // (__NT__)  
}

/**
** Piglet Productions
**
** FileName       : PigletChkFile.cpp
**
** Description
**
** Provides a single function, CheckFile which is a Novell friendly check
** to see if a file exists.
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
#include <dos.h>
#include <mem.h>

#ifdef PigletLibDebug
#include <PigletLog.h>
extern class PigletLog Trace;
#endif


/*
**    CheckFile
**
** Function to check for the existance of a file. This function is
** Netware friendly (it finishes all initiated searches).
**
**    Parameters
**
**    filename  The filename to check for.
**
**    Returns
**
**    A pointer to a find_t structure on a find, or NULL on failure.
*/
struct find_t *CheckFile(char * filename)
{
  static struct find_t found;
  struct find_t dummy;

  #ifdef g_LibDebug
  Trace.Printf('-', "%tCheckFile: check_file(%s) entered\n", filename);
  #endif

  if(!_dos_findfirst(filename, 0, &dummy)){
    // We found at least one matching filename, preserve the real details
    memcpy(&found, &dummy, sizeof(struct find_t));
    while(!_dos_findnext(&dummy));
    return(&found);
  }
  else return(NULL);
}

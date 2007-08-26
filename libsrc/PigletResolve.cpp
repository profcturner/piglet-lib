/****************************************************************************/
/*                                                                          */
/* Truename Resolver - Works with "broken" MS Clients                       */
/*                                                                          */
/* Copyright 1996,1997 Colin Turner                                         */
/*                                                                          */
/* Revision 002                                                             */
/*                                                                          */
/* Associated Documentation, GOLDLIB.DOC                                    */
/*                                                                          */
/* Limited Distribution                                                     */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* Revision 2; 18th August 1998                                             */
/*                                                                          */
/* * Made compatible with Watcom, 16 and 32 bit DOS                         */
/*                                                                          */
/****************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <PigletCore.h>


Resolve::Resolve()
{
  // Only constructor, prepares pointers for use and fetches the redir table
  int loop;
  for(loop=0; loop<26; loop++) Redir[loop]=NULL; // Zero pointers
  Fill();
}


Resolve::~Resolve()
{
  // Deallocate memory
  Empty();
}


void Resolve::Refresh()
{
  Fill();
}


void Resolve::Fill()
{
  // This function first destroys any existing redirection table, and
  // then constructs a new one. Any drive which is redirected will have
  // a string allocated for that name, and attached to the character array
  // above.
  Empty();
  int index=0, quit=0;
  char remote[128];
  char local[16];
  union REGS r, or;
  SREGS s;

  while(!quit){
    r.h.ah=0x5F;
    r.h.al=0x02;
    s.ds  =FP_SEG(local);
    s.es  =FP_SEG(remote);
    #if defined(__386__) && defined(__DOS__)
    r.x.ebx=index;
    r.x.esi=FP_OFF(local);
    r.x.edi=FP_OFF(remote);
    #else
    r.x.bx=index;
    r.x.si=FP_OFF(local);
    r.x.di=FP_OFF(remote);
    #endif



    #if defined(__386__) && defined(__DOS__)
      int386x(0x21, &r, &or, &s);
    #else
      int86x(0x21, &r, &or, &s);
    #endif

    if(or.x.cflag != 0) quit=1; // No more items, or something dreadful ;-)
    else
    {
      if(or.h.bl == 0x04 && (!(or.h.bh & 0x01))){
        // Valid drive redirection entry
        // Allocate space for the correct pointer
        Redir[local[0] - 'A'] = new char[strlen(remote)+1];
        strcpy(Redir[local[0] - 'A'], remote);
      }
      index++;
    }
  }
}

void Resolve::Empty()
{
  int loop;
  for(loop=0; loop<26; loop++){
    delete Redir[loop]; // Destroy allocation
    Redir[loop]=NULL;
  }
}

char *Resolve::Tname(char *original, char *buffer)
{
  char compose[128];
  REGS r, or;
  SREGS s;
  /*
  // First let's check if we have a complete path, that is, one with a
  // drive letter, colon, and backslash.
  if(toupper(original[1])==':' && toupper(original[2])=='\\'){
    // Indeed, a full path, we might as well look for a redirection and
    // if not found leave it as is.
    if(Redir[toupper(original[0]) - 'A']){
      // There is an entry in the redirection table, happy days
      strcpy(buffer, Redir[toupper(original[0]) - 'A']);
      strcat(buffer, original+3);
      return(buffer); // Good enough ;-)
    }
  } */

  // Ok, no joy so far, we better do a correct "truename" call.
  *compose=0;

  r.h.ah=0x60;

  #if defined(__386__) && defined(__DOS__)
  r.x.esi=FP_OFF(original);
  r.x.edi=FP_OFF(compose);
  #else
  r.x.si=FP_OFF(original);
  r.x.di=FP_OFF(compose);
  #endif
  s.ds=FP_SEG(original);
  s.es=FP_SEG(compose);
  #if defined(__386__) && defined(__DOS__)
  int386x(0x21, &r, &or, &s); // Plain vanilla truename ;-)
  #else
  int86x(0x21, &r, &or, &s); // Plain vanilla truename ;-)
  #endif
  strcpy(buffer, compose);

  // We better apply the microsoft client fix. We could farm this to a
  // sub function as above, but I'd rather avoid the function call overhead
  if(compose[1]==':' && compose[2]=='\\'){
    // Indeed, a full path, we might as well look for a redirection and
    // if not found leave it as is.
    if(Redir[compose[0] - 'A']){
      // There is an entry in the redirection table, happy days
      strcpy(buffer, Redir[compose[0] - 'A']);
      strcat(buffer, compose+2);

    }
  }
  return(buffer); // Better be ok by now.
}




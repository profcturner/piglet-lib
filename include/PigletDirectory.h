/***************************************************************************
*                                                                          *
* Piglet Productions - File Directory Class                                *
*                                                                          *
* (c) 1996-98 Colin Turner                                                 *
*                                                                          *
* FileName        : P_DIRECT.H                                             *
* Defined Classes : PigletDirectory <- PigletDoubleSList                   *
*                 : PigletDirectoryItem                                    *
*                                                                          *
* This class creates, and if need be refreshes a linked list of items in a *
* disk directory. The class is basically on top of a linked list, with all *
* that this implies.                                                       *
*                                                                          *
***************************************************************************/

/*
** Revision 1.00
**
** Colin Turner 1996
*/                                                                            

/*
** Revision 1.10
**
** Colin Turner 8th Novemeber 1998
**
** Change to new Piglet naming convention.
** Add flexibility to use RTL or JAM mktime function.
**
*/

#ifndef Piglet_Direct_H
#define Piglet_Direct_H

// Standard run time library                         
#include <string.h>
#include <dos.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

// Piglet Productions library                            
#include <PigletDSList.h>

class PigletDirectoryItem;
class PigletDirectory;
class PigletDirectoryIterator;


class PigletDirectoryItem
{
  // Attributes

  private:
    char Name[13];
    long Attributes, UnixStamp, Size;
    int  Time, Date;
  public:
    static PigletDirectoryItem *ZERO;

  // Services

  public:
    char *GetName() { return(Name); }
    long GetAttrib() { return(Attributes); }
    long GetUnix();
    int  GetTime() { return(Time); }
    int  GetDate() { return(Date); }
    long GetSize() { return(Size); }
    char *GetStub(char *buffer);
    char *GetExt(char *buffer);


  // Implementation
  public:
    PigletDirectoryItem();
    int  operator == (PigletDirectoryItem & Item2);
    int  operator <  (PigletDirectoryItem & Item2);

  friend class PigletDirectory;
};


class PigletDirectory : public PigletDoubleSList<PigletDirectoryItem>
{
  // Attributes

  private:
    char   Directory[72];
    char   Match[13];
    time_t UnixStamp;
    int    SearchType;

  // Services

  public:
    void   SetDirectory(char *dirname);
    void   SetMatch(char *matchname);
    time_t GetUnix() { return(UnixStamp); }
    void   Rescan();
    void   SetType(int type) { SearchType=type; }

  // Implementation

  public:
    PigletDirectory();
    PigletDirectory(char *dirname);
    PigletDirectory(char *dirname, char *matchname);
  private:
    void Constructor(char *dir, char *match);
    void AddTrail(char *path);

};


class PigletDirectoryIterator : public PigletDoubleListIterator<PigletDirectoryItem>
{
  // Implementation
  public :
    PigletDirectoryIterator(PigletDirectory & List)
      : PigletDoubleListIterator<PigletDirectoryItem>(List)
    {
    };

};

#endif // (Piglet_Direct_H)

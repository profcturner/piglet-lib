/**
** Piglet Productions
**
** FileName       : PigletDirectory.cpp
**
** Implements     : PigletDirectory <- PigletDoubleSList
**
** Description
**
** A linked list derivative which contains a list of files from a given
** directory (and scans for them).
**
** Initial Coding : Colin Turner
**
** Date           : 1996
**
**
** Copyright applies on this file, and distribution may be limited.
*/

/*
** Revision 1.00
**
*/

#include <PigletDirectory.h>

/****************************************************************************/
/*                                                                          */
/*                           PigletDirectory                                */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/*                    P U B L I C   F U N C T I O N S                       */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                    S E R V I C E  F U N C T I O N S                      */
/****************************************************************************/


void PigletDirectory::SetDirectory(char *dirname)
{
  strcpy(Directory, dirname);
  AddTrail(Directory);
}


void PigletDirectory::SetMatch(char *matchname)
{
  strcpy(Match, matchname);
}


void PigletDirectory::Rescan()
{
  PigletDirectoryItem Store;
  find_t fblock;
  int done=0;
  char buffer[100];

  // First of all, we should empty the current list
  Empty();
  if(strlen(Directory)){
    strcpy(buffer, Directory);
    strcat(buffer, Match);

    done=_dos_findfirst(buffer, SearchType, &fblock);
    while(!done){
      strcpy(Store.Name, fblock.name);
      Store.Attributes = (long) fblock.attrib;
      Store.Size = fblock.size;
      Store.Time = fblock.wr_time;
      Store.Date = fblock.wr_date;
      AddObject(Store);
      done=_dos_findnext(&fblock);
    }
    UnixStamp=time(NULL);
  }
}


/****************************************************************************/
/*              I M P L E M E N T A T I O N   F U N C T I O N S             */
/****************************************************************************/

PigletDirectory::PigletDirectory()
{
  Constructor("", "*.*");
}


PigletDirectory::PigletDirectory(char *dir)
{
  Constructor(dir, "*.*");
}


PigletDirectory::PigletDirectory(char *dir, char *match)
{
  Constructor(dir, match);
}


void PigletDirectory::Constructor(char *dir, char *match)
{
  SearchType=0;
  SetDirectory(dir);
  SetMatch(match);
  Rescan();
}


void PigletDirectory::AddTrail(char *path)
{
  if(!(*path)) return;
  if(path[strlen(path)-1]!='\\') strcat(path, "\\");
}

/****************************************************************************/
/*                                                                          */
/*                            PigletDirectoryItem                               */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/*                    P U B L I C   F U N C T I O N S                       */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                    S E R V I C E  F U N C T I O N S                      */
/****************************************************************************/


PigletDirectoryItem::PigletDirectoryItem()
{
  UnixStamp=0;
}


/*
int PigletDirectoryItem::cmpObject(PigletDirectoryItem& tocompare)
{
  char stub1[14], stub2[14];
  char ext1[5], ext2[5];
  int  dig1=1, dig2=1;
  int  loop=0;
  int  temp;
  GetStub(stub1);
  GetExt(ext1);

  if(GetAttrib()==FA_DIREC && tocompare.GetAttrib()!=FA_DIREC) return(-1);
  if(GetAttrib()!=FA_DIREC && tocompare.GetAttrib()==FA_DIREC) return(1);

  for(loop=0; loop<strlen(stub1); loop++) dig1 = dig1 && isdigit(stub1[loop]);
  if(dig1){
    tocompare.GetStub(stub2);
    tocompare.GetExt(ext2);
    for(loop=0; loop<strlen(stub2); loop++) dig2 = dig2 && isdigit(stub2[loop]);
  }

  if(dig1 && dig2){
    // Special case, both stubs are numbers.
    temp=strcmp(ext1, ext2);
    if(temp) return(temp);
    // Ok, we even have the same extensions
    return(atoi(stub1)-atoi(stub2));
  }
  return(strcmp(this->Name, tocompare.Name));
}
*/


int PigletDirectoryItem::operator < (PigletDirectoryItem & Item2)
{
  char stub1[14], stub2[14];
  char ext1[5], ext2[5];
  int  dig1=1, dig2=1;
  int  loop=0;
  int  temp;
  GetStub(stub1);
  GetExt(ext1);

  #ifdef __BORLANDC__
  if (GetAttrib()==FA_DIREC && Item2.GetAttrib()!=FA_DIREC) return(1);
  if (GetAttrib()!=FA_DIREC && Item2.GetAttrib()==FA_DIREC) return(0);
  #else
  if (GetAttrib()==_A_SUBDIR && Item2.GetAttrib()!=_A_SUBDIR) return(1);
  if (GetAttrib()!=_A_SUBDIR && Item2.GetAttrib()==_A_SUBDIR) return(0);  
  #endif

  for (loop=0; loop<strlen(stub1); loop++) dig1 = dig1 && isdigit(stub1[loop]);
  if (dig1) {
    Item2.GetStub(stub2);
    Item2.GetExt(ext2);
    for (loop=0; loop<strlen(stub2); loop++) dig2 = dig2 && isdigit(stub2[loop]);
  }

  if (dig1 && dig2) {
    // Special case, both stubs are numbers.
    temp = strcmp(ext1, ext2);
    if (temp){
      if(temp < 1) return(1);
      else return(0);
    }
    // Ok, we even have the same extensions
    return((atoi(stub1)-atoi(stub2)) < 1);
  }
  return(strcmp(Name, Item2.Name) < 1);

}


int PigletDirectoryItem::operator == (PigletDirectoryItem & Item2)
{
  return(!strcmp(Name, Item2.Name));
}


// This takes the stub of the filename (before the ".") and returns it.
// Feed in the area to fill, this is returned for convenience.
char *PigletDirectoryItem::GetStub(char *tofill)
{
  char *pos;
  strcpy(tofill, Name);
  pos=strstr(tofill, ".");
  if(pos) *pos=0;
  return(tofill);
}


char *PigletDirectoryItem::GetExt(char *tofill)
{
  char *pos;
  pos=strstr(Name, ".");
  if(pos) strcpy(tofill, pos+1);
  else *tofill=0;
  return(tofill);
}


long PigletDirectoryItem::GetUnix()
{
  struct tm tm;
  if(UnixStamp) return(UnixStamp);

  tm.tm_year = ((Date & 0xF800) >> 9) +80;
  tm.tm_mon  = ((Date & 0x01E0) >> 5) -1;
  tm.tm_mday =  (Date & 0x001F);

  tm.tm_hour =  (Time & 0xF800) >> 11;
  tm.tm_min  =  (Time & 0x07E0) >> 5;
  tm.tm_sec  = ((Time & 0x001F) * 2);

  return(mktime(&tm));
}


PigletDirectoryItem *PigletDirectoryItem::ZERO = (new PigletDirectoryItem);

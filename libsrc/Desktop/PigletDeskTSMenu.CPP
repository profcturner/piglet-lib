/*
** Piglet Productions
**
** FileName       : PigletDeskTSMenu.CPP
**
** Implements     : PigletTagSMenu
**
** Description
**
** See Desktop.H for descriptions and revision history.
**
**
** Initial Coding : Colin Turner
**
** Date           : 1997
**
**
** Copyright applies on this file, and distribution may be limited.
*/


#include <PigletDesktop.h>

void PigletTagSMenu::Set(long item)
{
  PigletScrollLine * Store;
  PigletDoubleVListIterator<PigletScrollLine> Lit(LineList);

  // Get item from list
  Store = Lit.GoToItemNumber(item);

  // Alter the item
  Store->Text[2] = 'þ';
  Store->Status |= 0x01;
  //LineList.AlterObject(item, Store);
}


void PigletTagSMenu::Clear(long item)
{
  PigletScrollLine * Store;
  PigletDoubleVListIterator<PigletScrollLine> Lit(LineList);

  // Get item from list
  Store = Lit.GoToItemNumber(item);

  // Alter the item
  Store->Text[2] = ' ';
  Store->Status &= ~0x01;
  //LineList.AlterObject(item, Store);
}


void PigletTagSMenu::Toggle(long item)
{
  PigletScrollLine * Store;
  PigletDoubleVListIterator<PigletScrollLine> Lit(LineList);

  // Get item from list
  Store = Lit.GoToItemNumber(item);

  // Alter the item
  if(Store->Status & 0x01) Store->Status &= ~0x01; else Store->Status |= 0x01;
  if(Store->Status & 0x01) Store->Text[2] = 'þ'; else Store->Text[2] = ' ';
  //LineList.AlterObject(item, Store);
}


long PigletTagSMenu::Get(long item)
{
  PigletScrollLine * Store;
  PigletDoubleVListIterator<PigletScrollLine> Lit(LineList);

  // Get item from list
  Store=Lit.GoToItemNumber(item);
  return( Store->Status & 0x01);

}
/*


void PigletTagSMenu::SetAll(void)
{

}


void PigletTagSMenu::ClearAll(void)
{

}
*/

// End of File

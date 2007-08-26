/*
** Piglet Productions
**
** FileName       : PigletDeskHMenu.CPP
**
** Implements     : PigletHBarData
**                  PigletHBarMenu
**
** Description
**
** See Desktop.H for descriptions and revision history.
**
**
** Initial Coding : Chris McCandless
**
** Date           : 1995
**
*/

#include <PigletDesktop.h>


/**
**    PigletHBarData
**
** Simple constructor to zero the memory used by the object.
**
**/
PigletHBarData::PigletHBarData(void)
{
  memset(this, 0, sizeof(PigletHBarData));
}


/**
**    operator ==
**
** A required function to use the class in conjunction with PigletDoubleVList.
**
**    Parameters
**
**    compare     Object used for comparison
**
**    Returns
**
**    zero for inequality, non zero otherwise
**
**/
int PigletHBarData::operator == (PigletHBarData & compare)
{
  return(X == compare.X);
}


/**
**    PigletHBarMenu
**
** Constructor which zeroes relevant fields and sets the correct default
** colours for the menu.
**
**    Parameters
**
**    de        Desktop object passed by reference which will handle the menu
**
**    Returns
**
**/
PigletHBarMenu::PigletHBarMenu(PigletDesktop & de)
{
  d               = &de;
  Count           = 0;
  Selected        = 0;
  Old             = 0;
  NormalAttrib    = 0x07;
  HighlightAttrib = 0x71;
};


/**
**    Add
**
** This function is used to add a Vertical Bar Menu to the Horizontal Bar Menu.
**
**    Parameters
**
**    name        The name to display on the horizontal bar menu to trigger
**                this vertical bar menu.
**    x           The x co-ordinate where the desktop class should place the
**                text in name above.
**    zmenu       The vertical bar menu to trigger.
**
**    Returns
**
**    1 on success, 0 on failure (allocation error)
**
**/
int PigletHBarMenu::Add(char * name, int x, PigletVBarMenu & zmenu)
{
  PigletHBarData * ToAdd = new PigletHBarData;

  strcpy(ToAdd->Data, name);
  ToAdd->X = x;
  ToAdd->Menu = &zmenu;
  Count++;

  return(BarList.AddObject(ToAdd));
}


/**
**    Display
**
** This function displays the horizontal bar menu ready to run.
**
**/
void PigletHBarMenu::Display(void)
{
  PigletDoubleVListIterator<PigletHBarData> it(BarList);
  PigletHBarData * ToDisplay;

  int n;

  for (n = 0; n < Count; n++) {
    ToDisplay = it.Current();
    if (Selected == it.GetCurrentNumber()) d->WinWriteString(d->Desk(), ToDisplay->Data, ToDisplay->X, 1, HighlightAttrib);   
    else d->WinWriteString(d->Desk(), ToDisplay->Data, ToDisplay->X, 1, NormalAttrib);
    it++;
  }
}


/**
**    GetOption
**
** This function is used to obtain the selected vertical bar menu, with
** numbering starting from one (1).
**
**/
int PigletHBarMenu::GetOption()
{
  return(Selected);
}


/**
**    Go
**
** This function runs the menu, processing various keypresses such as cursor
** movements internally. It will exit when other keys are pressed, allowing
** the user to take appropriate action and restart the menu.
**
**    Returns
**
**    The keycode which caused the exit of the menu.
**
**/
int PigletHBarMenu::Go(void)
{
  PigletDoubleVListIterator<PigletHBarData> it(BarList);
  PigletHBarData * ToCheck;
  int input      = CUR_RIGHT;
  int MousePress = 0;

  // If this is the first run of the class, we need to set the selected
  // menu to the first, and highlight it.
  if (!Selected) {
    ToCheck = it.GoToStart();
    d->WinWriteString(d->Desk(), ToCheck->Data, ToCheck->X, 1, HighlightAttrib);
    Old = Selected = 1;
  }

  while (input == CUR_RIGHT || input == CUR_LEFT || MousePress) {

    if (Old != Selected) {
      // A new menu has been selected, we neet to close the old menu's window
      ToCheck = it.GoToItemNumber(Old);
      d->WinClose(ToCheck->Menu->GetHandle());
    }

    // Ensure the correct window is open and drawn
    ToCheck = it.GoToItemNumber(Selected);
    d->WinDraw(ToCheck->Menu->GetHandle());
    KeyPress = input = ToCheck->Menu->Go();
    MousePress = 0;
    Old = Selected;

    if (d->MousePress) {
      MousePress = 1;

      if (d->MouseY == 0) {
        int sx,ex;
        PigletHBarData * MouseToCheck;
        PigletDoubleVListIterator<PigletHBarData> MouseIt(BarList);
        int n;

        for (n = 0; n < BarList.GetItemsInContainer(); n++) {
          MouseToCheck = MouseIt.Current();
          sx = MouseToCheck->X - 1;
          ex = sx + strlen(MouseToCheck->Data) - 1;
          //MouseIt.RefreshNumber();
          if (d->MouseX >= sx && d->MouseX <= ex) Selected=(int) MouseIt.GetCurrentNumber();
          MouseIt++;
        }
      }
    }

    if (input == CUR_RIGHT || input == CUR_LEFT || MousePress) {
      // Dehighlight the text of the old menu
      ToCheck = it.GoToItemNumber(Old);
      d->WinWriteString(d->Desk(), ToCheck->Data, ToCheck->X, 1, NormalAttrib);
    }

    if (input == CUR_RIGHT) {         // Take a step to the right
      Selected++;
      if (Selected > Count) Selected = 1;
    }

    if (input == CUR_LEFT) {          // and a jump to the left...
      Selected--;
      if (Selected < 1) Selected = Count;
    }

    if (input == CUR_RIGHT || input == CUR_LEFT || MousePress) {
      // Highlight the text of the new menu
      ToCheck = it.GoToItemNumber(Selected);
      d->WinWriteString(d->Desk(), ToCheck->Data, ToCheck->X, 1, HighlightAttrib);
    }

  }
  return KeyPress;

}

// End of file

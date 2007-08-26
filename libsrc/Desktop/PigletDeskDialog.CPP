/**
** Piglet Productions
**
** FileName       : PigletDeskDialog.cpp
**
** Implements     : PigletDialogData
**                  PigletDialog
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
**
** Copyright applies on this file, and distribution may be limited.
*/

#include <PigletDesktop.h>


/**
**    Add
**
** This member function adds a button to a dialog.
**
**    Parameters
**
**    text    The text to be used on the Dialog Button
**            note that ^ characters can encapsulate the hot key.
**
**    x, y    Relative (Window) coordinates for the button to be placed.
**
**    Returns
**
**    1 on success, 0 on failure (memory alloc)
**/
int PigletDialog::Add(char * text, int x, int y)
{
  PigletDialogData * ToAdd = new PigletDialogData;

  if(!ToAdd) return(0);

  ToAdd->Text = new char[strlen(text)+1];
  strcpy(ToAdd->Text, text);

  int n=0;
  while (text[n]!='^' && n!=strlen(text)) n++;
  if (text[n]=='^' && text[n+2]=='^') ToAdd->Letter=text[++n];

  ToAdd->X = x;
  ToAdd->Y = y;
  Count++;

  return DialogList.AddObject(ToAdd);
}


/**
**    Display
**
** This function draws the contents of the dialog box (but does not actually
** run any selection code).
**
**/
void PigletDialog::Display(void)
{
  PigletDoubleVListIterator<PigletDialogData> it(DialogList);

  long n;
  PigletDialogData * ToDraw;

  for (n = 0; n < DialogList.GetItemsInContainer(); n++) {
    ToDraw = it.Current();
    if (Pos==(int) it.GetCurrentNumber()) {
      char * tstring;
      char * temptext = new char[strlen(ToDraw->Text)+1];
      strcpy(temptext, ToDraw->Text);

      tstring = strtok(temptext,"^");
      d->WinWriteString(Handle, tstring, ToDraw->X, ToDraw->Y, HAttr);
      tstring = strtok(NULL,"^");
      d->WinWriteString(Handle, tstring, HAttr);
      tstring = strtok(NULL,"^");
      d->WinWriteString(Handle, tstring, HAttr);

      delete temptext;
    } else {
      char * tstring;
      char * temptext = new char[strlen(ToDraw->Text)+1];
      strcpy(temptext, ToDraw->Text);

      tstring = strtok(temptext,"^");
      d->WinWriteString(Handle, tstring, ToDraw->X, ToDraw->Y, NAttr);
      tstring = strtok(NULL,"^");
      d->WinWriteString(Handle, tstring, LAttr);
      tstring = strtok(NULL,"^");
      d->WinWriteString(Handle, tstring, NAttr);

      delete temptext;
    }
    it++;
  }
}


/**
**    Go
**
** This function runs the dialog box along with its selection code. It
** internally handles many navigational keys, but will exit on other keypresses
** to allow for custom handling by the user.
**
**    Returns
**
**    The keypress which caused the dialog to exit
**/
int PigletDialog::Go(void)
{
  PigletWindow * Win = d->FindWindow(Handle);
  int c;

  do {
    Display();
    c=d->GetKey(0);

    if (d->Mouse) {
      if (d->MousePress) {
        int n;

        PigletDialogData * ToCheck;
        PigletDoubleVListIterator<PigletDialogData> it(DialogList);

        for (n = 0; n < DialogList.GetItemsInContainer(); n++) {
          ToCheck = it.Current();
          if (d->MouseX >= ToCheck->X + Win->X1 - 1 && d->MouseX <= ToCheck->X + Win->X1 - 4 + strlen(ToCheck->Text) && d->MouseY == ToCheck->Y + Win->Y1 - 1) {
            int OldPos = Pos;
            d->MousePress = 0;
            Pos = (int) it.GetCurrentNumber();
            if (Pos == OldPos) c = 13;
          }
          it++;
        }
      }
    }

    PigletDoubleVListIterator<PigletDialogData> it(DialogList);
    int n;
    PigletDialogData * ToCheck;
    for (n = 0; n < DialogList.GetItemsInContainer(); n++) {
      ToCheck = it.Current();
      if (toupper(ToCheck->Letter) == (char) toupper(c)) {
        Pos = (int) it.GetCurrentNumber();
        c = 13;
      }
      it++;
    }

    if (c == CUR_RIGHT || c == CUR_UP || c == TAB) {
      Pos++;
      if (Pos > Count) Pos=1;
    }
    if (c == CUR_LEFT || c == CUR_DOWN || c == SHIFT_TAB) {
      Pos--;
      if (Pos < 1) Pos = Count;
    }

  } 
  while (c == CUR_UP || c == CUR_DOWN || c == CUR_LEFT || c == CUR_RIGHT || c == TAB  || c == SHIFT_TAB);

  return c;
}


/**
**    PigletDialog
**
** The constructor for the dialog box.
**
**    Parameters
**
**    Returns
**
**/
PigletDialog::PigletDialog(PigletDesktop & desktop, int WindowHandle, char NormalAttr, char LetterAttr, char HighlightAttr)
{
  d      = &desktop;
  Handle = WindowHandle;
  NAttr  = NormalAttr;
  LAttr  = LetterAttr;
  HAttr  = HighlightAttr;
  Pos    = 1;
  Count  = 0;
}


PigletDialogData::PigletDialogData(void)
{
  memset(this, 0, sizeof(PigletDialogData));
}


int PigletDialogData::operator == (PigletDialogData & compare)
{
  return(!strcmp(compare.Text, Text));
}


// End of File

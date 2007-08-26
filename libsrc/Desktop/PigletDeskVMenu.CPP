/*
** Piglet Productions
**
** FileName       : PigletDeskVMenu.CPP
**
** Implements     : PigletVBarData
**                  PigletVBarMenu
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

PigletVBarData::PigletVBarData(void)            // Constructor
{
  memset(this, 0, sizeof(class PigletVBarData));
}


int PigletVBarData::operator == (PigletVBarData & compare)
{
  return(!strcmp(compare.Text, Text));
}


PigletVBarMenu::PigletVBarMenu(PigletDesktop & de, int w_handle, int sx, int sy)
// de - desktop
// w_handle - see handle
// sx,sy - see x,y
{
  Handle=w_handle;
  d=&de;
  NormalAttrib=de.PigletColours.VBNormalAttr; //0x07;
  HighlightAttrib=de.PigletColours.VBHighlightAttr; //0x71;
  Count=0;
  Widest=0;
  Selected=Old=1;
  X=sx;
  Y=sy;
}


void PigletVBarMenu::Add(char * menu, char * description)
// menu - Menu bar text
// description - status line
{
  PigletVBarData * ToAdd = new PigletVBarData;
  PigletWindow * Win = d->FindWindow(Handle);

  strcpy(ToAdd->Text, menu);
  strcpy(ToAdd->Describe, description);
  if (strlen(ToAdd->Describe) > 80) ToAdd->Describe[80] = '\0';

  int maxlength = Win->X2 - Win->X1 - X;
  if (strlen(ToAdd->Text) > maxlength) ToAdd->Text[maxlength]='\0';

  BarList.AddObject(ToAdd);
  if (strlen(ToAdd->Text) > Widest ) Widest = strlen(ToAdd->Text);
  Count++;
}


void PigletVBarMenu::Display(void)
{
  PigletDoubleVListIterator<class PigletVBarData> it(BarList);
  PigletVBarData * ToDisplay;

  int m;
  for (m = 0; m < Count; m++) {
    ToDisplay = it.Current();
    d->WinWriteString(Handle, ToDisplay->Text, X, Y+m, NormalAttrib); //ColorMsg(w.x1+x,w.y1+y+m,normal_attrib,temp.text);
    d->WinFillRowChar(Handle,X+strlen(ToDisplay->Text),Y+m,Widest-strlen(ToDisplay->Text),NormalAttrib,' ');
    it++;
  }
}


int PigletVBarMenu::Go(void)
{
  PigletWindow * Win = d->FindWindow(Handle);
  // Returns the actual keypress

  if (!Selected) Selected = Old;

  int m;
  PigletDoubleVListIterator<class PigletVBarData> it(BarList);
  PigletVBarData * ToCheck;
  for (m = 0; m < Count; m++) {
    ToCheck = it.Current();
    if (m == Selected - 1) {
      d->ColorMsg(Win->X1 + X, Win->Y1 + Y + m, HighlightAttrib, ToCheck->Text);
      d->FillRowChar(Win->X1 + X + strlen(ToCheck->Text), Win->Y1 + Y + m, Widest-strlen(ToCheck->Text), HighlightAttrib, ' ');

      d->StatusLine(ToCheck->Describe);
    }
    else
    {
      d->ColorMsg(Win->X1+X, Win->Y1+Y+m, NormalAttrib, ToCheck->Text);
      d->FillRowChar(Win->X1+X+strlen(ToCheck->Text), Win->Y1+Y+m, Widest-strlen(ToCheck->Text), NormalAttrib, ' ');
    }
    it++;
  }
  int n;
//  int MousePress;
  // xxxxxx
  do {
    //   MousePress=0;
    n = d->GetKey(0);
    Old = Selected;

    if (d->Mouse) {
      if (d->MousePress) {
        int sx=X;
        int ex=sx+Widest;
        int X1, Y1;
        /*
        PigletWindow comp_window,w;
        comp_window.Handle = Handle;
        d->WindowList.FindObject(comp_window, w);
        */
        X1=Win->X1-2;
        Y1=Win->Y1-1;
        sx+=X1+1;
        ex+=X1;

        if (d->MouseX >= sx && d->MouseX <=ex) {
          PigletDoubleVListIterator<PigletVBarData> mouse_it(BarList);
          int m;

          for (m = 0; m < BarList.GetItemsInContainer(); m++) {
            mouse_it.RefreshNumber();
            if (d->MouseY == Y+Y1-1+mouse_it.GetCurrentNumber()) {
//        MousePress=1;
              d->MousePress=0;
              Selected=(int) mouse_it.GetCurrentNumber();
              if (Selected == Old) n = 13;
            }
            mouse_it++;
          }
        }
      }
    }


    if (n == CUR_DOWN) {   // Move the bar down
      Old = Selected++;
      if (Selected > Count) Selected =1;
    }
    if (n == CUR_UP) {   // Move the bar up
      Old = Selected--;
      if (Selected<1) Selected = Count;
    }
    if (n == HOME) {   // Move the bar up
      Old = Selected;
      Selected = 1;
    }
    if (n == END) {   // Move the bar up
      Old = Selected;
      Selected = Count;
    }

    if (Old != Selected) {
      ToCheck = it.GoToItemNumber(Old);
      d->ColorMsg(Win->X1 + X, Win->Y1+Y+Old-1, NormalAttrib, ToCheck->Text);
      d->FillRowChar(Win->X1+X+strlen(ToCheck->Text), Win->Y1+Y+Old-1, Widest-strlen(ToCheck->Text), NormalAttrib, ' ');
//    }
//    if(n==13 || n==CUR_DOWN || n==CUR_UP ||  n==HOME || n==END  || MousePress )
//    {
      ToCheck = it.GoToItemNumber(Selected);
      d->ColorMsg(Win->X1+X, Win->Y1+Y+Selected-1, HighlightAttrib, ToCheck->Text);
      d->FillRowChar(Win->X1+X+strlen(ToCheck->Text),Win->Y1+Y+Selected-1, Widest-strlen(ToCheck->Text), HighlightAttrib, ' ');
      // Status Line
      d->StatusLine(ToCheck->Describe);
    }

  } while (n==CUR_UP || n==CUR_DOWN || n==HOME || n==END);

  return n;
}



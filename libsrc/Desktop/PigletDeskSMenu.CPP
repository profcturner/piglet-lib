/*
** Piglet Productions
**
** FileName       : PigletDeskSMenu.CPP
**
** Implements     : PigletScrollCols
**                  PigletScrollLine
**                  PigletScrollMenu
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

int PigletScrollCols::operator == (PigletScrollCols & compare)
{
  return(XStart == compare.XStart);
}


PigletScrollCols::PigletScrollCols(void)
{
  memset(this, 0, sizeof(PigletScrollCols));
}


int PigletScrollLine::operator == (PigletScrollLine & compare)
{
  return(!strcmp(Text, compare.Text));
}


PigletScrollLine::PigletScrollLine(void)
{
  memset(this, 0, sizeof(PigletScrollLine));
}



// Main constructor


PigletScrollMenu::PigletScrollMenu(PigletDesktop &de, int wHandle, int sp, int ep, int Title, char nattr, char hattr, char battr, char bhattr, char tattr)
{
/*  NormalAttr=0x71;
  HighlightAttr=0x0e;
  BarAttr=0x74;
  BarHighlightAttr=0x0e;
  TitleAttr=0x71; */

  NormalAttr=nattr;
  HighlightAttr=hattr;
  BarAttr=battr;
  BarHighlightAttr=bhattr;
  TitleAttr=tattr;

  TopChar=24;
  BottomChar=25;
  ShadingChar='±';
  BlockChar='Û';

  Handle=wHandle;
  d=&de;

  TopPos=sp;
  BottomPos=ep;

  Count=0;
  Pos=TopPosNumber=1;
  OnScreen=ep-sp+1;
  NoCols=0;
  BlockPos=OldBlockPos=0;
  BarAlive=0;

  ShowTitle=Title;
}

// Destructor

PigletScrollMenu::~PigletScrollMenu(void)
{
  PigletDoubleVListIterator<PigletScrollLine>  it_line(LineList);
  PigletDoubleVListIterator<PigletScrollCols>  it_cols(ColsList);
  PigletScrollCols * temp_cols;
  PigletScrollLine * temp_line;

  unsigned long n;

  for (n=0;n<LineList.GetItemsInContainer();n++) {
    temp_line=it_line++;
    delete temp_line->Text;
  }

  for (n=0;n<ColsList.GetItemsInContainer();n++) {
    temp_cols=it_cols++;
    delete temp_cols->Title;
  }
}



int PigletScrollMenu::AddColumn(char *string, int xs, int xe, int Just)
{
  PigletScrollCols * ToAdd = new PigletScrollCols;

  ToAdd->Title = new char[strlen(string)+2];
  if (!ToAdd->Title) d->MemAbort(1);

  strcpy(ToAdd->Title,string);

  ToAdd->XStart = xs;
  ToAdd->XEnd   = xe;
  ToAdd->Just   = Just;

  NoCols++;

  return(ColsList.AddObject(ToAdd));
}


int PigletScrollMenu::AddEntry(char *string,...)
{
  PigletScrollLine * ToAdd = new PigletScrollLine;
  PigletScrollCols * Column;
  char *s;

  s = new char[strlen(string)+2];
  if (!s) d->MemAbort(1);

  strcpy(s,string);

  char line[200]="";
  char entry[200]="";
  int n,colwidth,l,c;

  PigletDoubleVListIterator<PigletScrollCols> it(ColsList);
  Column = it.Current();

  for (n = 0; n < Column->XStart - 1; n++) line[n]=' ';
  line[n]=0;

  colwidth = Column->XEnd - Column->XStart + 1;
  l=strlen(s);
  if (l>colwidth) {
    s[colwidth]=0;
    l=strlen(s);
  }

  if (Column->Just == PigletScrollMenu::TextLeft) {
    strcat(entry,s);
    c=strlen(entry);
    for (n=c;n<colwidth;n++) entry[n]=' ';
    entry[n]=0;
    strcat(line,entry);
  }
  if (Column->Just == PigletScrollMenu::TextRight) {
    c=colwidth-strlen(s);
    for (n=0;n<c;n++) entry[n]=' ';
    entry[n]=0;
    strcat(entry,s);
    strcat(line,entry);
  }
  if (Column->Just == PigletScrollMenu::TextCentre) {
    c=colwidth-strlen(s);
    c/=2;
    for (n=0;n<c;n++) entry[n]=' ';
    entry[n]=0;
    strcat(entry,s);
    c=strlen(entry);
    for (n=c;n<colwidth;n++) entry[n]=' ';
    entry[n]=0;
    strcat(line,entry);
  }

  int m,lastend;
  va_list ap;
  va_start(ap,string);
  lastend = Column->XEnd;
//  char *t;
  delete s;

  for (m = 1; m < NoCols; m++) {
    it++;
    Column = it.Current();
    colwidth = Column->XEnd - Column->XStart + 1;
    for (n = lastend; n < Column->XStart - 1; n++) entry[n-lastend]=' ';
    entry[n-lastend]=0;
    strcat(line,entry);
    s = va_arg(ap, char *);

    l=strlen(s);
    if (l>colwidth) {
      s[colwidth]=0;
      l=strlen(s);
    }

    if (Column->Just == PigletScrollMenu::TextLeft) {
      strcat(entry,s);
      c=strlen(entry);
      for (n=c;n<colwidth;n++) entry[n]=' ';
      entry[n]=0;
      strcat(line,entry);
    }
    if (Column->Just == PigletScrollMenu::TextRight) {
      c=colwidth-strlen(s);
      for (n=0;n<c;n++) entry[n]=' ';
      entry[n]=0;
      strcat(entry,s);
      strcat(line,entry);
    }
    if (Column->Just == PigletScrollMenu::TextCentre) {
      c=colwidth-strlen(s);
      c/=2;
      for (n=0;n<c;n++) entry[n]=' ';
      entry[n]=0;
      strcat(entry,s);
      c=strlen(entry);
      for (n=c;n<colwidth;n++) entry[n]=' ';
      entry[n]=0;
      strcat(line,entry);
    }
    lastend = Column->XEnd;
  }
//  delete s;
  va_end(ap);

  PigletWindow * Win = d->FindWindow(Handle);
  
  colwidth = Win->X2 - Win->X1 + 1;
  l = strlen(line);
  for (n = l; n < colwidth - 2; n++) entry[n-l]=' ';
  entry[n-l]=0;
  strcat(line,entry);
  l=strlen(line);

  ToAdd->Text = new char[strlen(line)+2];
  if (!ToAdd->Text) d->MemAbort(1);

  strcpy(ToAdd->Text,line);
  Count++;
  return(LineList.AddObject(ToAdd));
}


void PigletScrollMenu::DisplayTitle(void)
{
  if (ShowTitle) {
    char line[200]="";
    char entry[200]="";
    char *s;
    int lastend,colwidth,l,c,m,n;

    PigletDoubleVListIterator<PigletScrollCols> it(ColsList);
    PigletScrollCols * Column;
    Column = it.Current();
    lastend=0;

    for (m = 0; m < NoCols; m++) {
      Column = it.Current();
      colwidth=Column->XEnd - Column->XStart + 1;
      for (n = lastend; n < Column->XStart - 1; n++) entry[n-lastend]=' ';
      entry[n-lastend]=0;
      strcat(line,entry);
      s = new char[strlen(Column->Title)+2];
      if (!s) d->MemAbort(1);

      strcpy(s,Column->Title);

      l=strlen(s);
      if (l>colwidth) {
        s[colwidth]=0;
        l=strlen(s);
      }

      if (Column->Just==PigletScrollMenu::TextLeft) {
        strcat(entry,s);
        c=strlen(entry);
        for (n=c;n<colwidth;n++) entry[n]=' ';
        entry[n]=0;
        strcat(line,entry);
      }
      if (Column->Just==PigletScrollMenu::TextRight) {
        c=colwidth-strlen(s);
        for (n=0;n<c;n++) entry[n]=' ';
        entry[n]=0;
        strcat(entry,s);
        strcat(line,entry);
      }
      if (Column->Just==PigletScrollMenu::TextCentre) {
        c=colwidth-strlen(s);
        c/=2;
        for (n=0;n<c;n++) entry[n]=' ';
        entry[n]=0;
        strcat(entry,s);
        c=strlen(entry);
        for (n=c;n<colwidth;n++) entry[n]=' ';
        entry[n]=0;
        strcat(line,entry);
      }
      lastend=Column->XEnd;
      it++;
      delete s;
    }
    d->WinWriteString(Handle,line,1,ShowTitle,TitleAttr);
  }
}


int PigletScrollMenu::Go(int flag)
{
  long oldPos;

  if (Count<OnScreen) OnScreen=(int)Count;

  if (flag!=ScrollOld) {
    if (flag==ScrollBottom) {
      Pos=Count;
      TopPosNumber=Count-(OnScreen-1);
    } else
      if (flag==ScrollTop) {
      Pos=1;
      TopPosNumber=1;
    } else {
      Pos=flag;
      if (!(TopPosNumber <= flag && flag <= TopPosNumber+OnScreen)) {
        TopPosNumber=Pos-(OnScreen/2);
        if (TopPosNumber<1) TopPosNumber=1;                                 
        if ( ((Count-TopPosNumber)+1) < OnScreen ) TopPosNumber=Count-OnScreen+1;
      }
    }
  }

  SendChangeInfo();
  
  PigletWindow * Win = d->FindWindow(Handle);
  
  DisplayTitle();
  if (Count>OnScreen) {
    BarAlive=1;
    DrawBar();
  }
  int MousePress;
  do {
    MousePress=0;
    WritePage();
    if (BarAlive) {
      char string[5];
      BlockPos = (int) ( (Pos*(long)(BottomPos - TopPos - 1))/Count );
      if (BlockPos == BottomPos-TopPos-1) BlockPos--;
      string[0] = ShadingChar;
      string[1] = BarAttr;
      string[2] = 0;
      d->WriteRawV(Win, Win->X2 - Win->X1, TopPos + 1 + OldBlockPos, string);
      OldBlockPos = BlockPos;
      string[0] = BlockChar;
      string[1] = NormalAttr;
      string[2] = 0;
      d->WriteRawV(Win, Win->X2 - Win->X1, TopPos + 1 + BlockPos, string);
      d->BufferScreen(Win, Win->X2 - Win->X1, TopPos + 1, Win->X2 - Win->X1, BottomPos);
    }
    Char = d->GetKey(0);
    if (!d->Mouse) SendKeypress();  // Only call if an actual keypress is recorded and not a mouse press
    if (!(Char== CTRL_PAGE_UP || Char== CTRL_PAGE_DOWN || Char==HOME || Char==END || Char==CUR_UP || Char==CUR_DOWN || Char==PAGE_UP || Char==PAGE_DOWN)) SendAlienKeypress();
    oldPos=Pos;

    if (d->Mouse && d->MousePress) {
      MousePress=1;

      PigletWindow * Win = d->FindWindow(Handle);

      int sx,ex,sy,ey;

      sx = Win->X1;
      ex = Win->X2 - 2;
      sy = Win->Y1 + TopPos - 2;
      ey = sy + OnScreen;


      if (BarAlive  &&  d->MouseX == ex+1  &&   d->MouseY >= sy+2  &&  d->MouseY <= ey-1) {
        int length, bar_Pos;
        length=(ey-1)-(sy+2)+1;
        bar_Pos=d->MouseY-(sy+2)+1;

        Pos=(Count*bar_Pos)/length;
        if (Pos>Count-OnScreen) {
          TopPosNumber=Count-OnScreen+1;
        } else {
          TopPosNumber=Pos-(OnScreen/2);
          if (TopPosNumber<1) TopPosNumber=1;
        }
      }
      if (d->MouseX >= sx && d->MouseX <=ex && d->MouseY > sy && d->MouseY <= ey ) {
        Pos=d->MouseY-sy+TopPosNumber-1;
        if (oldPos==Pos) {
          Char=13;
          MousePress=0;
        }
      }

      if (d->MouseX==ex+1 && BarAlive) {
        if (d->MouseY==sy+1) {
          Char=CUR_UP;
        }
        if (d->MouseY==ey) {
          Char=CUR_DOWN;
        }
      }
    }
    if (Char==CUR_DOWN) {
      Pos++;
      if (Pos>Count) Pos=Count;
      if (Pos==TopPosNumber+OnScreen) TopPosNumber++;
    }
    if (Char==CUR_UP) {
      Pos--;
      if (!Pos) Pos=1;
      if (Pos==TopPosNumber-1) TopPosNumber--;
    }
    if (Char==CTRL_PAGE_UP) {
      TopPosNumber=Pos=1;
    }
    if (Char==CTRL_PAGE_DOWN) {
      Pos=Count;
      TopPosNumber=Pos-(OnScreen-1);
      if (Count<OnScreen) TopPosNumber=1;
    }
    if (Char==PAGE_DOWN) {
      int no_down;
      no_down=(int)(Pos-(TopPosNumber-1));
      Pos+=OnScreen;
      TopPosNumber+=OnScreen;
      if (TopPosNumber>Count-OnScreen) {
        TopPosNumber=Count-(OnScreen-1);
        Pos=TopPosNumber+(no_down-1);
      }
      if (Count<OnScreen) TopPosNumber=1;
    }
    if (Char==PAGE_UP) {
      if (TopPosNumber!=1) {
        int no_down;
        no_down=(int)(Pos-(TopPosNumber-1));
        TopPosNumber-=OnScreen;
        Pos-=OnScreen;
        if (Pos<=1) {
          TopPosNumber=1;
          Pos=TopPosNumber+(no_down-1);
        }
      }
    }
    if (Char==HOME) Pos=TopPosNumber;
    if (Char==END)  Pos=TopPosNumber+(OnScreen-1);

    if (oldPos!=Pos) SendChangeInfo();

    SendInfo();
  } while (MousePress || Char== CTRL_PAGE_UP || Char== CTRL_PAGE_DOWN || Char==HOME || Char==END || Char==CUR_UP || Char==CUR_DOWN || Char==PAGE_UP || Char==PAGE_DOWN);
  return Char;
}

// Virtual functions that do nothing

int PigletScrollMenu::SendInfo(void)
{
  // At the moment this does nothing
  return 0;
}

int PigletScrollMenu::SendKeypress(void)
{
  // At the moment this does nothing
  return 0;
}

int PigletScrollMenu::SendAlienKeypress(void)
{
  // At the moment this does nothing
  return 0;
}

int PigletScrollMenu::SendChangeInfo(void)
{
  // At the moment this does nothing
  return 0;
}



void PigletScrollMenu::WritePage(void)
{
  PigletDoubleVListIterator<PigletScrollLine> it(LineList);
  PigletScrollLine * ToWrite;

  it.GoToItemNumber(TopPosNumber);

  int n;
  for (n=0;n<OnScreen;n++) {
    ToWrite = it.Current();
    if (n + TopPosNumber == Pos) d->WinWriteString(Handle, ToWrite->Text, 1, TopPos + n, HighlightAttr);
    else d->WinWriteString(Handle, ToWrite->Text, 1, TopPos + n, NormalAttr);
    it++;
  }
}


void PigletScrollMenu::DrawBar(void)
{
  PigletWindow * Win = d->FindWindow(Handle);
  
  int n;
  char *line,*pointer;
  line = new char[(unsigned int)(BottomPos-TopPos+2)*2];
  if (!line) d->MemAbort(1);

  pointer=line;

  *pointer++=TopChar;
  *pointer++=BarHighlightAttr;
  for (n=0;n<BottomPos-TopPos-1;n++) {
    *pointer++=ShadingChar;
    *pointer++=BarAttr;
  }
  *pointer++=BottomChar;
  *pointer++=BarHighlightAttr;
  *pointer=0;


  Win = d->WriteRawV(Win, Win->X2 - Win->X1, TopPos, line);
  d->BufferScreen(Win, Win->X2 - Win->X1, TopPos, Win->X2 - Win->X1, BottomPos);

  delete line;

}

                                                                  

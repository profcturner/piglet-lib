/*
** Piglet Productions
**
** FileName       : PigletDesktop.CPP
**
** Implements     : PigletDesktop
**                  PigletWindow
**
** Description
**            
** See Desktop.H for description and revision history.
**   
** Initial Coding : Chris McCandless
**           
** Date           : 1995
**                
*/

//#define g_RunDebug

#ifdef PigletLibDebug
  #include <PigletLog.h>
extern class PigletLog Trace;
#endif

#include <PigletDesktop.h>

/****************************************************************************
*                                                                           *
*                  G E N E R A L   S T R U C T U R E S                      *
*                                                                           *
*****************************************************************************/


/****************************************************************************
*                                                                           *
*              W I N D O W   C L A S S   B E G I N S   H E R E              *
*                                                                           *
*****************************************************************************/


/****************************************************************************
*                                                                           *
*               P U B L I C   W I N D O W   F U N C T I O N S               *
*                                                                           *
*****************************************************************************/


PigletWindow::PigletWindow(void)
{
  memset(this, 0, sizeof(class PigletWindow));
}


PigletWindow::PigletWindow(int cX1, int cY1, int cX2, int cY2 )
{
  memset(this, 0, sizeof(class PigletWindow));
  X1 = cX1;
  X2 = cX2;
  Y1 = cY1;
  Y2 = cY2;
}


int PigletWindow::operator == (PigletWindow & compare)
{
  return(Handle == compare.Handle);
}


int PigletWindow::GetHandle(void)
{
  return Handle;
}


/****************************************************************************
*                                                                           *
*            D E S K T O P   C L A S S   B E G I N S   H E R E              *
*                                                                           *
*****************************************************************************/



/****************************************************************************
*                                                                           *
*           P R I V A T E   D E S K T O P   F U N C T I O N S               *
*                                                                           *
*****************************************************************************/


/**
**    FindWindow
**
** This function finds a window with a matching handle in the maintained list
** and returns a direct handle to its data.
**
**    Parameters
**
**    Handle      Handle of the window to fetch
**
**    Returns
**
**    NULL if no window is found, otherwise a pointer to the Window class.
**
**/
PigletWindow * PigletDesktop::FindWindow(int Handle)
{
  PigletWindow ToFind;

  ToFind.Handle = Handle;
  return(WindowList.FindObject(&ToFind));
}

#ifdef PigletDesktop_RangeCheck

void PigletDesktop::SubScriptError(int Code)
{
  #ifdef PigletRunDebug_Desktop
  Trace.Printf('!', "DesktopClass: subscript error code %u\n", Code);
  #endif
  PigletDesktop::~PigletDesktop();
  printf("\nSubscript Error\nExit code %i\nCoreleft %lu\n", Code, coreleft());
  exit(1);
}


#endif // PigletDesktop_RangeCheck



/**
**    MemAbort
**
** A function called from various sites within the class if a serios memory
** allocation failure occurs. The class is destroyed.
**
**    Parameters
**
**    code    A code which allows the site of the error to be determined.
**
**/
void PigletDesktop::MemAbort(int code)
{
  #ifdef PigletRunDebug_Desktop
  Trace.Printf('!', "DesktopClass: Fatal memory allocation error code %u\n", code);
  #endif
  PigletDesktop::~PigletDesktop();
  printf("\nMemory allocation failure\nExit code %i\nCoreleft %lu\n",code,coreleft());
  exit(1);
}


void PigletDesktop::ShowIdent(void)
{
  int n,m;
  for (m = 0; m < VRows; m++) {
    for (n = 0; n < VCols; n++) {        // Suppa duppa fast window read system
      _GotoXY(n+1,m+1);
      printf("%i", ident[(m*VCols) + n]);
    }
  }
}


PigletWindow * PigletDesktop::WriteRawV(PigletWindow * w, int x, int y, char * string, char attrib)
{
  char temp[400];
  int  n;
  
  for (n = 0; n < strlen(string); n++) {
    temp[2*n] = string[n];
    temp[(2*n)+1] = attrib;
  }
  temp[2*n] = 0;

  return(WriteRawV(w, x, y, temp));
}


PigletWindow * PigletDesktop::WriteRawV(PigletWindow * w, int x, int y, char * string)
{
  int    n = 0;
  char   temp[4];
  char   attrib;
  char * pointer;

  pointer = string;

  while (*pointer) {
    temp[0] = *pointer++;
    temp[1] = 0;
    attrib = *pointer++;
    w = WriteBuffer(w, x, y+n, attrib, temp);
    n++;
  }

  return w;
}


PigletWindow * PigletDesktop::WriteRawH(PigletWindow * w, int x, int y, char * string, char attrib)
{
  char temp[400];
  int  n;

  for (n = 0; n < strlen(string); n++) {
    temp[2*n] = string[n];
    temp[(2*n)+1] = attrib;
  }
  temp[2*n] = 0;

  return(WriteRawH(w, x, y, temp));
}


PigletWindow * PigletDesktop::WriteRawH(PigletWindow * w, int x, int y, char * string)
{
  int    n = 0;
  char   temp[4];
  char    attrib;
  char * pointer;

  pointer = string;

  while (*pointer) {
    temp[0] = *pointer++;
    temp[1] = 0;
    attrib = *pointer++;
    w = WriteBuffer(w, x+n, y, attrib, temp);
    n++;
  }

  return w;
}


/**
**    RefreshScreen
**
** This function is called when a window is closed, and places the window
** at the end of the Window linked list.
**
**    Parameters
**
**    Win       A pointer of the window to relocate
**
**    Returns
**
**/
void PigletDesktop::RefreshScreen(PigletWindow * Win)
{
  PigletWindow * TempWin;
  
  int top, bottom, Piglet_Far_left, Piglet_Far_right, n;

  // Don't destroy the window, but remove it from the list for now
  WindowList.UnlinkObject(Win);

  // The iterator may no longer be valid
  PigletDoubleVListIterator<PigletWindow> it(WindowList);

  if (Win->Shadow == PigletWindow::ShadowOn) {
    Win->X2++;
    Win->Y2++;
  }
  
  // Repaints the windows
  while (!it.AtEnd()) {

    TempWin = it.Current();
    
    if (TempWin->Shadow == PigletWindow::ShadowOn) {
      TempWin->X2++;
      TempWin->Y2++;
    }

    Piglet_Far_left = Piglet_Far_right=0;
    for (n = TempWin->X1; n <= TempWin->X2; n++) {
      if (n >= Win->X1 && n <= Win->X2) Piglet_Far_right = n;
      if (!Piglet_Far_left) Piglet_Far_left=Piglet_Far_right;
    }
    
    top = bottom = 0;
    for (n = TempWin->Y1; n <= TempWin->Y2; n++) {
      if (n >= Win->Y1 && n <= Win->Y2) bottom = n;
      if (!top) top = bottom;
    }

    if (TempWin->Shadow == PigletWindow::ShadowOn) {
      TempWin->X2--;
      TempWin->Y2--;
    }


    if (top && Piglet_Far_left) {
      int m;
      for (m = top; m <= bottom; m++) {
        for (n = Piglet_Far_left; n <= Piglet_Far_right; n++) {
          if (ident[((m-1)*VCols)+(n-1)] == Win->Handle) ident[((m-1)*VCols)+(n-1)]= (char) TempWin->Handle;
        }
      }
      // Repaint a relevant rectangle of this window
      BufferScreen(TempWin, Piglet_Far_left - TempWin->X1, top - TempWin->Y1, Piglet_Far_right - TempWin->X1, bottom - TempWin->Y1);
    }
    // Next Window
    it++;
  }

  if (Win->Shadow == PigletWindow::ShadowOn) {
    Win->X2--;
    Win->Y2--;
  }
  
  
  // Add the window back to the list, this places it at the end (bottom)
  WindowList.AddObject(Win);
}


/**
**    WriteBuffer
**
** This function writes a string in a specified colour directly into the
** memory buffer of a given window (which is not the video memory itself).
**
**    Parameters
**
**    Win       The window whose buffer we will write into
**    x, y      The coordinates in the window where the string will begin
**    attrib    The text attribute or colour of the text
**    string    The string to write
**
**    Returns
**
**    The pointer of the window once again
**/
PigletWindow * PigletDesktop::WriteBuffer(PigletWindow * Win, int x, int y, char attrib, char * string)
{
  char Piglet_Far * pointer = Win->Buffer;
  char * ptr = string;
  char * line;
  int length = strlen(string)*2;

  #ifdef PigletDesktop_RangeCheck
  int RangeMaxBuffer = (Win->X2 - Win->X1 + 3) * (Win->Y2 - Win->Y1 + 3) * 2;
  #endif

  if (Win->Shadow == PigletWindow::ShadowOn) Win->X2++;
  #ifdef PigletDesktop_RangeCheck
  if((y*(Win->X2 - Win->X1+1)*2)+(x*2) + length > RangeMaxBuffer) SubScriptError(1);
  #endif
  pointer += (y*(Win->X2 - Win->X1+1)*2)+(x*2);
  if (Win->Shadow == PigletWindow::ShadowOn) Win->X2--;
  line = new char[(strlen(string)*2)+2];
  if (!line) MemAbort(1);

  _fmemset(line,0,(strlen(string)*2)+2);

  int count=0;
  for (;*ptr;*ptr++) {
    line[count]=*ptr;
    line[count+1]=attrib;
    count+=2;
  }
  _fmemcpy(pointer,line,length);
  delete line;

  return Win;
}


/**
**    IsObscured
**
** This function is used to determine if a specified point within a specified
** window is covered (obscured) or not by another window.
**
**    Parameters
**
**    Handle        Handle of the window to check
**    x,y           Co-ordinates within window to check
**
**    Returns
**
**    1 indicates co-ordinate is obscured by another window
**    0 inficates co-ordinate is not obscured
**/
int PigletDesktop::IsObscured(int Handle, int x, int y)
{
  if (ident[((y-1)*VCols)+(x-1)]!=Handle) return 1;
  return 0;
}


/**
**    BufferScreen
**
** This function repaints the contents of a window as they emerge through a
** specified recectangle. It takes account of any obscuring windows and does
** not redraw such characters.
**
**    Parameters
**
**    Win           The Window whose contents we wish to repaint
**    X1,Y1,X2,Y2   The absolute rectangle to repaint
**
**/
void PigletDesktop::BufferScreen(PigletWindow * Win, int X1, int Y1, int X2, int Y2)
{
  int  n, m;
  char ch;
  char attrib;
  char Piglet_Far *buffer;

  #ifdef PigletDesktop_RangeCheck
  int RangeMaxBuffer = (Win->X2 - Win->X1 + 3) * (Win->Y2 - Win->Y1 + 3) * 2;
  #endif

  if (Win->Shadow == PigletWindow::ShadowOn) {
    Win->X2++;
    Win->Y2++;
  }

  for (n = Y1; n <= Y2; n++) {
    for (m = X1; m <= X2; m++) {
      if (!IsObscured(Win->Handle, m + Win->X1, n + Win->Y1)) {
        buffer = (char Piglet_Far *) Win->Buffer;
        #ifdef PigletDesktop_RangeCheck
        if((n*(Win->X2 - Win->X1 + 1)*2)+(m*2) + 2 > RangeMaxBuffer) SubScriptError(2);
        #endif
        buffer += (n*(Win->X2 - Win->X1 + 1)*2)+(m*2);
        ch=*buffer++;
        attrib=*buffer;
        FillRowChar(m + Win->X1, n + Win->Y1, 1, attrib, ch);
      }
    }
  }
  if (Win->Shadow == PigletWindow::ShadowOn) {
    Win->X2--;
    Win->Y2--;
  }

}


/**
**    UpdateShadow
**
** This updates the shadow of the active window (the window with focus)
**
**/
void PigletDesktop::UpdateShadow()
{
  PigletWindow * Win = FindWindow(GetActive());

  if(!Win) return;
  if (Win->Shadow == PigletWindow::ShadowOn && Win->BorderFlag != PigletWindow::NoBorder) {
    FillRowAttr(Win->X1+1, Win->Y2+1, (Win->X2 - Win->X1) + 1, 0x08);
    FillColAttr(Win->X2+1, Win->Y1+1, (Win->Y2 - Win->Y1) + 1, 0x08);
  }
}


/**
**    DrawDesktop
**
** Draws the desktop, by opening a special window whose handle is stored in
** the dh member variable. Several basic desktop layouts are supported
** directly, but BlankDesktop can be used as a starting point to create
** one of your own.
**
**/
void PigletDesktop::DrawDesktop(void)
{
  if (DesktopFlag==BlankDesktop) {
    dh=WinInit(1,1,VCols,VRows);
    SetWinShadow(dh,PigletWindow::ShadowOff);
    SetWinBackgroundChar(dh,DesktopBackgroundChar);
    SetWinBorder(dh,PigletWindow::NoBorder);
    WinDraw(dh);
  }

  if (DesktopFlag==NoDesktop) {
    dh=WinInit(1,1,VCols,VRows);
    SetWinShadow(dh,PigletWindow::ShadowOff);
    SetWinBackgroundChar(dh,DesktopBackgroundChar);
    SetWinBorder(dh,PigletWindow::NoBorder);

    PigletWindow * Win = FindWindow(dh);
    _fmemcpy(Win->Buffer, VBaseSeg, VRows*VCols*2);
    WinOpen(dh);
  }
  
  if (DesktopFlag==FDDesktop) {
    dh=WinInit(1,1,VCols,VRows);
    SetWinShadow(dh,PigletWindow::ShadowOff);
    SetWinBackgroundChar(dh,DesktopBackgroundChar);
    SetWinBorder(dh,PigletWindow::NoBorder);
    WinDraw(dh);
    WinLineYStyle(dh,2,PigletColours.StatLineTop,PigletWindow::DoubleBorder);
    WinLineYStyle(dh,VRows-1,PigletColours.StatLineBottom,PigletWindow::SingleBorder);
    WinFillRowChar(dh,1,1,VCols,PigletColours.MenuBar,' ');
    WinFillRowChar(dh,1,VRows,VCols,PigletColours.MenuBar,' ');
  }
  
  if (DesktopFlag==GEchoDesktop) {
    int header_Handle;
    dh=WinInit(1,5,VCols,VRows);
    SetWinShadow(dh,PigletWindow::ShadowOff);
    SetWinBackgroundChar(dh,DesktopBackgroundChar);
    SetWinBorder(dh,PigletWindow::NoBorder);
    header_Handle=WinInit(1,1,VCols,4);
    SetWinShadow(header_Handle,PigletWindow::ShadowOff);
    SetWinBorderAttr(header_Handle,PigletColours.HeaderBorder);
    SetWinBackgroundAttr(header_Handle,PigletColours.Header);
    WinDraw(header_Handle);
    WinLock(header_Handle);
    WinDraw(dh);
    FillRowChar(1,VRows-1,VCols,PigletColours.StatLineBottom,'Ä');
    FillRowChar(1,VRows,VCols,PigletColours.MenuBar,' ');
  }
}


/**
**    GetXY
**
** Determines the starting x and y coordinates of the text cursor, and stores
** them in the StartX and StartY member variables.
**
**/
void PigletDesktop::GetXY(void)
{
  union REGS regs;
  regs.h.ah=3;
  regs.h.bh=0;
  #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__) || defined(__OS2__)
  int386(0x10,&regs,&regs);
  #else
  int86(0X10,&regs,&regs);
  #endif
  StartX=regs.h.dl;
  StartY=regs.h.dh;

}


/**
**    ReturnChar
**
** Fetches the character stored in the video memory at specified absolute
** coordinates.
**
**    Parameters
**
**    x,y       Absolute coordinates to fetch character from
**
**    Returns
**
**    See above
**
**/
char PigletDesktop::ReturnChar(int x, int y)
{
  char Piglet_Far *vbs = (char Piglet_Far *) VBaseSeg;
  vbs+=(VCols*2*(y-1))+((x-1)*2);
  return *vbs;
}


void PigletDesktop::ScrollUp(int Handle)
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;

  int X1,X2,Y1,Y2;
  X1 = Win->X1;
  X2 = Win->X2;
  Y1 = Win->Y1;
  Y2 = Win->Y2;
  if (Win->BorderFlag != PigletWindow::NoBorder) {
    X1++;
    X2--;
    Y1++;
    Y2--;
  }

  char Piglet_Far *vbs = (char Piglet_Far *) VBaseSeg;
  int n;
  for (n = 0; n <= Y2 - Y1 + 1; n++) {
    _fmemcpy(Win->Buffer+(n*(X2-X1+2)*2),vbs+((n+Y1-1)*2*VCols)+((X1-1)*2),((X2-X1+2)*2));
  }
  for (n = 1; n < Y2 - Y1 + 1; n++) {
    _fmemcpy(vbs+((n+Y1-2)*2*VCols)+((X1-1)*2), Win->Buffer+(n*(X2-X1+2)*2),((X2-X1+2)*2));
  }
  char *string;
  string = new char[X2-X1+2];
  if (!string) MemAbort(1);

  for (n = 0; n < X2 - X1 + 1; n++) {
    string[n]=' ';
  }
  string[n]='\0';
  DspMsg(X1, Y2, string);

  delete string;
}


/**
**    _GotoXY
**
** Moves the text cursor to specified absolute coordinates.
**
**    Parameters
**
**    x,y     absolute coordinates to move the cursor to
**
**    Returns
**
**/
void PigletDesktop::_GotoXY(int x, int y)
{
  union REGS regs;

  regs.h.ah=0x0f;
  #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
  int386(0x10,&regs,&regs);
  #else
  int86(0x10,&regs,&regs);
  #endif
  regs.h.ah = 0x02;
  regs.h.dh = (unsigned char) (y - 1);
  regs.h.dl = (unsigned char) (x - 1);
  #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
  int386(0x10,&regs,&regs);
  #else
  int86(0x10,&regs,&regs);
  #endif
}


void PigletDesktop::Cursor(int ch, int cl)
{
  union REGS regs;

  regs.h.ah = 0x01;
  regs.h.ch = (unsigned char) ch;
  regs.h.cl = (unsigned char) cl;
  #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
  int386(0x10,&regs,&regs);
  #else
  int86(0x10,&regs,&regs);
  #endif
}


/**
**    ClrWin
**
** "Clears" an area of the screen with a specified character and colour.
** Directly affects video memory.
**
**    Parameters
**
**    X1        top left x co-ordinate
**    Y1        top left y co-ordinate
**    X2        bottom right x co-ordinate
**    Y2        bottom right y co-ordinate
**    attrib    attribute (colour) for the area
**    ch        fill character for the area
**
**/
void PigletDesktop::ClrWin(int X1, int Y1, int X2, int Y2, char attrib, char ch )
{
  char Piglet_Far *vbs = (char Piglet_Far *) VBaseSeg;
  unsigned long length;
  register int n;

  #ifdef PigletDesktop_RangeCheck
  int RangeMaxVideo = VRows * VCols * 2;
  #endif // (PigletDesktop_RangeCheck)

  if (X1<1) X1=1;
  if (X2>VCols) X2=VCols;
  if (Y1<1) Y1=1;
  if (Y2>VRows) Y2=VRows;

  length=(X2-X1)+1;
  char Piglet_Far *line;
  line = new char[(unsigned short) (length*2L)];
  if (!line) MemAbort(1);

  for (n=0;n<length;n++) {
    line[n*2] = ch;
    line[(n*2)+1] = attrib;
  }

  for (n=Y1;n<=Y2;n++) {
    #ifdef PigletDesktop_RangeCheck
    if((2*(X1-1))+((n-1)*VCols*2) + (length * 2)  > RangeMaxVideo) SubScriptError(3);
    #endif // (PigletDesktop_RangeCheck)
    _fmemcpy( vbs+(2*(X1-1))+((n-1)*VCols*2) , line, (size_t) (length*2));
  }

  delete line;
}


/**
**    FillRowChar
**
** Fills a row with specified characters in a specified colour.
** Directly affects video memory.
**
**    Parameters
**
**    x         starting x co-ordinate
**    y         starting y co-ordinate
**    length    number of characters to display
**    attrib    attribute of characters
**    ch        character to display
**
**/
void PigletDesktop::FillRowChar(int x, int y, int length, char attrib, char ch )
{
  char Piglet_Far *vbs = (char Piglet_Far *) VBaseSeg;
  register int n;

  #ifdef PigletDesktop_RangeCheck
  int RangeMaxVideo = VRows * VCols * 2;
  #endif // (PigletDesktop_RangeCheck)

  if (x<1) x=1;
  if (x>VCols) x=VCols;
  if (y<1) y=1;
  if (y>VRows) y=VRows;

  char Piglet_Far *line;
  line = new char[length*2];
  if (!line) MemAbort(1);

  for (n=0;n<length;n++) {
    line[n*2] = ch;
    line[(n*2)+1] = attrib;
  }

  #ifdef PigletDesktop_RangeCheck
  if((2*(x-1))+((y-1)*VCols*2) + (length*2) > RangeMaxVideo) SubScriptError(4);
  #endif // (PigletDesktop_RangeCheck)
  _fmemcpy( vbs+(2*(x-1))+((y-1)*VCols*2), line, length*2);

  delete line;
}


/**
**    FillRowAttr
**
** Fills a row with a specified attribute (colour) without changing the
** characters placed there.
** Directly affects video memory.
**
**    Parameters
**
**    x         starting x co-ordinate
**    y         starting y co-ordinate
**    length    number of characters to display
**    attrib    attribute of caracters
**
**/
void PigletDesktop::FillRowAttr(int x, int y, int length, char attrib )
{
  char Piglet_Far *vbs = (char Piglet_Far *) VBaseSeg;
  register int n;

  if (x<1) x=1;
  if (x>VCols) x=VCols;
  if (y<1) y=1;
  if (y>VRows) y=VRows;

  char Piglet_Far *line;
  line = new char[length*2];
  if (!line) MemAbort(1);

  _fmemcpy( line, vbs+(2*(x-1))+((y-1)*VCols*2), length*2 );

  for (n=0;n<length;n++) {
    line[(n*2)+1] = attrib;
  }

  _fmemcpy( vbs+(2*(x-1))+((y-1)*VCols*2), line, length*2);

  delete line;
}


/**
**    FillColChar
**
** See FillRowChar for details.
**
**/
void PigletDesktop::FillColChar(int x, int y, int length, char attrib, char ch )
{
  char Piglet_Far *vbs = (char Piglet_Far *) VBaseSeg;
  register int n;

  if (x<1) x=1;
  if (x>VCols) x=VCols;
  if (y<1) y=1;
  if (y>VRows) y=VRows;

  char line[2];

  line[0]=ch;
  line[1]=attrib;


  for (n=y;n<y+length;n++) {
    _fmemcpy( vbs+(2*(x-1))+((n-1)*VCols*2) , line, 2);
  }
}


/**
**    FillColAttr
**
** See FillRowAttr for details.
**
**/
void PigletDesktop::FillColAttr(int x, int y, int length, char attrib )
{
  char Piglet_Far *vbs = (char Piglet_Far *) VBaseSeg;
  register int n;

  if (x<1) x=1;
  if (x>VCols) x=VCols;
  if (y<1) y=1;
  if (y>VRows) y=VRows;

  char line[2];

  for (n=y;n<y+length;n++) {
    _fmemcpy( line, vbs+(2*(x-1))+((n-1)*VCols*2), 2);
    line[1]=attrib;
    _fmemcpy( vbs+(2*(x-1))+((n-1)*VCols*2) , line, 2);
  }
}


//  Draws the frame for windows used by Drawwindow, make_Active and WinClose
void PigletDesktop::DrawFrame(int X1, int Y1, int X2, int Y2, char border, char Piglet_Far *TopTitle, int TJust, char Piglet_Far *BottomTitle, int BJust, char TitleAttr, int border_style)
{
  char br,h,lb,v,tr,lt;
  if (border_style==PigletWindow::SingleBorder) {
    br = 'Ú';
    h  = 'Ä';
    lb = '¿';
    v  = '³';
    tr = 'À';
    lt = 'Ù';
  } else
    if (border_style==PigletWindow::DoubleBorder) {
    br = 'É';
    h  = 'Í';
    lb = '»';
    v  = 'º';
    tr = 'È';
    lt = '¼';
  }

  FillRowChar(X1,Y1,1,border,br);
  if (!TopTitle) {
    FillRowChar(X1+1,Y1,X2-X1,border,h);
  } else {
    int length=_fstrlen(TopTitle);
    if (TJust==PigletWindow::TitleRight) {
      FillRowChar(X1+1,Y1,X2-X1-1-length,border,h);
      ColorMsg(X2-length,Y1,TitleAttr,TopTitle);
    }
    if (TJust==PigletWindow::TitleLeft) {
      ColorMsg(X1+1,Y1,TitleAttr,TopTitle);
      FillRowChar(X1+length+1,Y1,X2-X1-1-length,border,h);
    }
    if (TJust==PigletWindow::TitleCentre) {
      int number,number2;
      number=number2=((X2-X1+1)-length-2)/2;
      if (((X2-X1+1)-length-2)%2) number++;
      FillRowChar(X1+1,Y1,number,border,h);
      ColorMsg(X1+1+number,Y1,TitleAttr,TopTitle);
      FillRowChar(X1+1+number+length,Y1,number2,border,h);
    }
  }
  FillRowChar(X2,Y1,1,border,lb);
  FillRowChar(X1,Y2,1,border,tr);

  if (!BottomTitle) {
    FillRowChar(X1+1,Y2,X2-X1,border,h);
  } else {
    int length=_fstrlen(BottomTitle);
    if (BJust==PigletWindow::TitleRight) {
      FillRowChar(X1+1,Y2,X2-X1-1-length,border,h);
      ColorMsg(X2-length,Y2,TitleAttr,BottomTitle);
    }
    if (BJust==PigletWindow::TitleLeft) {
      ColorMsg(X1+1,Y2,TitleAttr,BottomTitle);
      FillRowChar(X1+length+1,Y2,X2-X1-1-length,border,h);
    }
    if (BJust==PigletWindow::TitleCentre) {
      int number,number2;
      number=number2=((X2-X1+1)-length-2)/2;
      if (((X2-X1+1)-length-2)%2) number++;
      FillRowChar(X1+1,Y2,number,border,h);
      ColorMsg(X1+1+number,Y2,TitleAttr,BottomTitle);
      FillRowChar(X1+1+number+length,Y2,number2,border,h);
    }
  }

  FillRowChar(X2,Y2,1,border,lt);
  FillColChar(X1,Y1+1,(Y2-Y1)-1,border,v);
  FillColChar(X2,Y1+1,(Y2-Y1)-1,border,v);

}


/**
**    DrawWindow
**
** Draws a window with defined characteristics.
**
**    Parameters
**
**    X1        top left x co-ordinate
**    Y1        top left y co-ordinate
**    X2        bottom right x co-ordinate
**    Y2        bottom right y co-ordinate
**    border    border attribute (if any)
**    attrib    window attribute
**    ch        character for window fill
**    Shadow    Shadow status
**
**/
void PigletDesktop::DrawWindow(int X1, int Y1, int X2, int Y2, int b_flag, char border, char attrib, char ch, char Piglet_Far *TopTitle, int TJust, char Piglet_Far *BottomTitle, int BJust, char TitleAttr, int Shadow )
{
  if (b_flag==PigletWindow::SingleBorder || b_flag==PigletWindow::DoubleBorder) {
    ClrWin(X1+1,Y1+1,X2-1,Y2-1,attrib,ch);

    DrawFrame(X1,Y1,X2,Y2,border,TopTitle,TJust,BottomTitle,BJust,TitleAttr,b_flag);

  } else
    if (b_flag==PigletWindow::NoBorder) {
    ClrWin(X1,Y1,X2,Y2,attrib,ch);
  }
  Shadow++;
}


/**
**    DspMsg
**
** Writes a text string to the screen using at specified absolute coordinates.
** Directly affects video memory.
**
**    Parameters
**
**    x         absolute x coordinate of string start
**    y         absolute y coordinate of string start
**    text      The string to place on the screen
**
**/
void PigletDesktop::DspMsg(int x, int y, char *text)
{
  char Piglet_Far *v_start = (char Piglet_Far *) VBaseSeg;

  v_start = v_start + ((y-1)*VCols*2) + ((x-1)*2);
  for (;*text;*text++) {
    if (*text!='\n') {
      _fmemcpy(v_start, text, 1);
      v_start+=2;
    } else {
      int current_row;

      current_row = (int) ((int)(v_start - VBaseSeg) / VCols);
      if ((v_start - VBaseSeg) % VCols) current_row++;
      v_start = VBaseSeg + ((current_row-1)*VCols*2);
    }
  }
}


void PigletDesktop::ColorMsg(int x, int y, char attrib, char Piglet_Far *text)
/*
** Writes a text string on the screen
**
** x    - x co-ordinate to start
** y    - y co-ordinate to start
** text - pointer to the string ( NULL terminated )
*/
{
  char Piglet_Far *v_start = (char Piglet_Far *) VBaseSeg;

  v_start = v_start + ((y-1)*VCols*2) + ((x-1)*2);
  for (;*text;*text++) {
    if (*text!='\n') {
      _fmemcpy(v_start++, text, 1);
      _fmemcpy(v_start++, &attrib, 1);
    } else {
      int current_row;

      current_row = (int) ((int)(v_start - VBaseSeg) / VCols);
      if ((v_start - VBaseSeg) % VCols) current_row++;
      v_start = VBaseSeg + ((current_row-1)*VCols*2);
    }
  }
}


/**
**    MouseOn
**
** Enables mouse operations (provided a driver is present and StartMouse has
** been called).
**
**/
void PigletDesktop::MouseOn(void)
{
  if (Mouse) {
    MouseActive=1;
    union REGS regs;
    #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
    regs.x.eax=5;
    regs.x.ebx=0;    
    int386(0x33,&regs,&regs);
    #else
    regs.x.ax=5;
    regs.x.bx=0;
    int86(0x33,&regs,&regs);
    #endif
    
    #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
    regs.x.eax=5;
    regs.x.ebx=1;    
    int386(0x33,&regs,&regs);
    #else
    regs.x.ax=5;
    regs.x.bx=1;  
    int86(0x33,&regs,&regs);
    #endif
  }
}


/**
**    MouseOff
**
** Disables mouse operations.
**
**/
void PigletDesktop::MouseOff(void)
{
  if (Mouse) MouseActive=0;
}


/**
**    MouseCursorOn
**
** Often handled automatically, this allows the user to manually turn on the
** mouse cursor.
**
**/
void PigletDesktop::MouseCursorOn(void)
{
  if (Mouse && MouseActive) {

    union REGS regs;

    
    #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
    regs.x.eax=1;
    int386(0x33,&regs,&regs);
    #else
    regs.x.ax=1;
    int86(0x33,&regs,&regs);
    #endif
  }
}


/**
**    MouseCursorOff
**
** Often handled automatically, this allows the user to manually turn off the
** mouse cursor.
**
**/
void PigletDesktop::MouseCursorOff(void)
{
  if (Mouse  && MouseActive) {

    union REGS regs;

    #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
    regs.x.eax=2;
    int386(0x33,&regs,&regs);
    #else
    regs.x.ax=2;
    int86(0x33,&regs,&regs);
    #endif
  }
}


/**
**    StartMouse
**
** This initializes the mouse if present, and if successfull enables it for
** use throughout.
**
**    Returns
**
**    1 if sucessful, 0 on failure (no mouse or driver)
**/
int PigletDesktop::StartMouse(void)
{
  union REGS regs;

  // Check for Mouse
  #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
  regs.x.eax=0;
  int386(0x33,&regs,&regs);
  #else
  regs.x.ax=0;
  int86(0x33,&regs,&regs);
  #endif
  
  #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
  if (regs.x.eax!=0xFFFF) {
    Mouse=0;
    MouseActive=0;
    return 0;
  }
  #else
  if (regs.x.ax!=0xFFFF) {
    Mouse=0;
    MouseActive=0;
    return 0;
  }
  #endif

  // There is a Mouse and it is installed !!
  #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
  regs.x.eax = 4;
  regs.x.ecx = 0;
  regs.x.edx = 0;
  int386(0x33,&regs,&regs);
  #else
  regs.x.ax = 4;
  regs.x.cx = 0;
  regs.x.dx = 0;
  int86(0x33,&regs,&regs);
  #endif

  #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
  regs.x.eax = 7;
  regs.x.ecx = 0;
  regs.x.edx = (VCols-1)*8;  
  int386(0x33,&regs,&regs);
  #else
  regs.x.ax = 7;
  regs.x.cx = 0;
  regs.x.dx = (VCols-1)*8;
  int86(0x33,&regs,&regs);
  #endif

  #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
  regs.x.eax = 8;
  regs.x.ecx = 0;
  regs.x.edx = (VRows-1)*8;
  int386(0x33,&regs,&regs);
  #else
  regs.x.ax = 8;
  regs.x.cx = 0;
  regs.x.dx = (VRows-1)*8;
  int86(0x33,&regs,&regs);
  #endif

  Mouse=1;
  MouseActive=1;
  return 1;
}


/**
**    WinWriteStringCommon
**
** A common function called by all the WinWriteString family.
**
**    Parameters
**
**    handle    Handle of window to write to
**    string    The string to write
**    attrib    The attribute of the text (colour)
**    type      0 indicates DSP, 1 indicates Colour
**
**/
void PigletDesktop::WinWriteStringCommon(int handle, char * string, char attrib, int type)
{
  PigletWindow * Win = FindWindow(handle);

  char *pointer;  // pointer to start of line
  int linelength;
  pointer = string;

  linelength = Win->X2 - Win->X1 - 1;
  if (Win->BorderFlag == PigletWindow::NoBorder) linelength+=2;

  char *line;
  line = new char[linelength+1];
  if (!line) MemAbort(1);

  int ochars,chars;
  int quit=0;
  int ocx,ocy;
  int y2;
  y2  = Win->Y2;
  ocx = Win->CX;
  ocy = Win->CY;
  if (Win->BorderFlag == PigletWindow::NoBorder) {
    y2++;
    ocx--;
    ocy--;
  }
  while (!quit) {
    ochars = chars = Win->CX - 1;
    while (chars!=linelength && *pointer!='\n' && *pointer) {
      line[(chars++)-ochars]=*pointer++;
      Win->CX++;
    }
    line[chars-ochars]='\0';
    if (!(*pointer)) quit=1;
    if (chars==linelength) {
      Win->CX = 1;
      Win->CY++;
    }
    if (*pointer=='\n') {
      *pointer++;
      Win->CX = 1;
      Win->CY++;
    }
    if ( Win->CY == y2 ) {
      Win->CY--;
      ScrollUp(handle);
    }
    
    if (type == 0) Win = WriteBuffer(Win, ocx, ocy, Win->BackAttr, line);
    if (type == 1) Win = WriteBuffer(Win, ocx, ocy, attrib, line);

    BufferScreen(Win, ocx, ocy, ocx + strlen(string) - 1, ocy);

    ocx = Win->CX;
    ocy = Win->CY;
    if (Win->BorderFlag == PigletWindow::NoBorder) {
      ocx--;
      ocy--;
    }
  }
  delete line;
  if (!(Win->CursorScanStart == 0x20 && Win->CursorScanEnd == 0x00)) _GotoXY(Win->X1 + ocx, Win->Y1 + ocy);
}


/****************************************************************************
*                                                                           *
*             P U B L I C   D E S K T O P   F U N C T I O N S               *
*                                                                           *
*****************************************************************************/


/**
**    WinScrollUp
**
** Scrolls the contents of a specified window one row upwards.
**
**    Parameters
**
**    Handle      Handle of window to scroll
**/
void PigletDesktop::WinScrollUp(int Handle)
{
  PigletWindow * Win = FindWindow(Handle);
  char *TempBufferPtr;
  char *ptr, *ptr1;
  int length, height;

  if(!Win) return;

  int sh=0;  // Shadow Monitor
  if (Win->Shadow == PigletWindow::ShadowOn) sh=1;
  int nob=0;
  if (Win->BorderFlag == PigletWindow::NoBorder) nob=2;

  length = Win->X2 - Win->X1 - 1;
  height = Win->Y2 - Win->Y1 - 1;

  TempBufferPtr = new char[(length+2+sh)*(height+2)*2];
  if (!TempBufferPtr) MemAbort(5);

  memcpy(TempBufferPtr, Win->Buffer, (length+2+sh)*(height+2)*2);
  ptr=Win->Buffer;
  ptr1=TempBufferPtr;

  if (Win->BorderFlag != PigletWindow::NoBorder) {
    ptr  += (length+3+sh)*2;
    ptr1 += (length+3+sh)*2;
    ptr1 += (length+2+sh)*2;
  } else {
    ptr1 += (length+2+sh)*2;
  }

  int n;
  for (n = 2; n <= height + nob; n++) {
    memcpy(ptr, ptr1, (length + nob)*2);
    ptr  += (length+2+sh)*2;
    ptr1 += (length+2+sh)*2;
  }

  char *line;
  line = new char[(length+nob)*2];
  if (!line) MemAbort(5);

  memset(line, (char) Win->BackCh, (length+nob)*2);
  for (n=1;n<=((length+nob)*2);n+=2) line[n]=(char) Win->BackAttr;

  memcpy(ptr, line, (length+nob)*2);


  delete line;
  delete TempBufferPtr;

  int sX1, sX2, sY1, sY2;
  sX1=sY1=1;
  sX2=sX1+length-1;
  sY2=sY1+height-1;

  if (Win->BorderFlag == PigletWindow::NoBorder) {
    sX1=sY1=0;
    sX2++;
    sY2++;
  }

  BufferScreen(Win, sX1, sY1, sX2, sY2);
}


/**
**    WinScrollDown
**
** Scrolls the contents of a specified window one row downwards.
**
**    Parameters
**
**    Handle      Handle of window to scroll
**/
void PigletDesktop::WinScrollDown(int Handle)
{
  PigletWindow * Win = FindWindow(Handle);
  char *TempBufferPtr;
  char *ptr, *ptr1, *ptr2;
  int length, height;

  if(!Win) return;
  
  int sh=0;  // Shadow Monitor
  if (Win->Shadow == PigletWindow::ShadowOn) sh=1;
  int nob=0;
  if (Win->BorderFlag == PigletWindow::NoBorder) nob=2;

  length = Win->X2 - Win->X1 - 1;
  height = Win->Y2 - Win->Y1 - 1;

  TempBufferPtr = new char[(length+2+sh)*(height+2)*2];
  if (!TempBufferPtr) MemAbort(5);

  memcpy(TempBufferPtr, Win->Buffer, (length+2+sh)*(height+2)*2);
  ptr2 = ptr = Win->Buffer;
  ptr1 = TempBufferPtr;

  if (Win->BorderFlag!=PigletWindow::NoBorder) {
    ptr  += (length+3+sh)*2;
    ptr2  = ptr;
    ptr1 += (length+3+sh)*2;
    ptr  += (length+2+sh)*2;
  } else {
    ptr  += (length+2+sh)*2;
  }


  int n;
  for (n=2;n<=height+nob;n++) {
    memcpy(ptr, ptr1, (length+nob)*2);
    ptr  += (length+2+sh)*2;
    ptr1 += (length+2+sh)*2;
  }

  char *line;
  line = new char[(length+nob)*2];
  if (!line) MemAbort(5);

  memset(line, (char) Win->BackCh, (length+nob)*2);
  for (n = 1; n <= ((length+nob)*2); n += 2) line[n]=(char) Win->BackAttr;

  memcpy(ptr2, line, (length+nob)*2);


  delete line;
  delete TempBufferPtr;

  int sX1, sX2, sY1, sY2;
  sX1=sY1=1;
  sX2=sX1+length-1;
  sY2=sY1+height-1;

  if (Win->BorderFlag == PigletWindow::NoBorder) {
    sX1=sY1=0;
    sX2++;
    sY2++;
  }

  BufferScreen(Win, sX1, sY1, sX2, sY2);
}


/*
** RegisterIdleFunction
**
** This registers a function with the desktop class which it will call
** every IdleDelay seconds when possible. (In menus, and other places where
** the class is waiting for keyboard input).
**
**  Parameters
**
**    Function pointer. Function should have prototype
**    void FunctionName(PigletDesktop * desktop)
*/
void PigletDesktop::RegisterIdleFunction( void (*p) (PigletDesktop * d))
{
  OnIdle = p;
}


/*
** SetIdleDelay
**
** If the idle function has been registered using RegisterIdleFunction, the
** idle function will be called by default every 20 seconds. This function
** can be used to alter this timer.
**
**    Parameters
**
**    idleTime    Time in seconds between calls to idle function
*/
void PigletDesktop::SetIdleDelay(int idleTime)
{
  IdleDelay = idleTime;
}


/**
**    SetOpSys
**
** This sets the internal Operating Systems services pointer in the Desktop
** class to a specified value.
**
** At construction, this pointer is set to NULL and no time slicing is
** performed (although IdleFunctions will still be called). When this is set
** the TimeSlice() member function will be called while polling for keys.
**
**    Parameters
**
**    newOS       Pointer to the Operating Systems object.
**/
void PigletDesktop::SetOpSys(PigletOpSys * newOS)
{
  OS = newOS;
}

PigletDesktop::PigletDesktop(int desktop, int border, char bchar)
// Constructor function for PigletDesktop
{
  union REGS regs;
  struct SREGS sregs;
  struct PigletVideoInformation v;

#ifdef g_RunDebug
  Trace.Printf("%tDesktop contructor: Entered\n");
#endif

  GetXY();

  PigletColours.MenuBar = 0x07;
  PigletColours.StatLineTop = 0x01;
  PigletColours.StatLineBottom = 0x01;
  PigletColours.Background = 0x07;
  PigletColours.InfoLine = 0x07;
  PigletColours.Header = 0x71;
  PigletColours.HeaderBorder = 0x71;
  PigletColours.DefWin=0x07;
  PigletColours.DefWinBorder=0x0b;
  PigletColours.DefWinTitle=0X1e;
  PigletColours.DefWinBorderBGrnd=0x01;
  PigletColours.InputAttr=0X21;
  PigletColours.OutputAttr=0x03;

  PigletColours.VBNormalAttr=0x07;
  PigletColours.VBHighlightAttr=0x71;

  DesktopFlag=desktop;
  BorderFlag=border;
  NewHandle=0;
  Active=-1;
  DesktopBackgroundChar=bchar;
  Shutdown=0;

  OS = NULL;

  OnIdle = NULL;
  IdleDelay = 20;
  LastIdleTime = 0;

/*  regs.h.ah=0X10;        // High intensity characters turned on
  regs.h.al=0x03;
  regs.h.bl=0x00;
  int86(0X10,&regs,&regs); */

  // Video stuff

#ifdef g_RunDebug
  Trace.Printf("%tDesktopConstructor: Establishing video information\n");
#endif



  #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
  regs.h.ah=0X1b;
  regs.x.ebx=0x00;
  regs.x.edi=FP_OFF(&v);
  sregs.es=FP_SEG(&v);
  
  int386x(0X10,&regs,&regs,&sregs);
  #else
  regs.h.ah=0X1b;
  regs.x.bx=0x00;
  regs.x.di=FP_OFF(&v);
  sregs.es=FP_SEG(&v);
  
  int86x(0X10,&regs,&regs,&sregs);
  #endif

  if (regs.h.al==0X1b) {
    VMode = v.mode;
    VCols = v.cols;
    VRows = v.rows;
    if (VMode==7) VBaseSeg = (char Piglet_Far *) 0xb0000000L;
    else VBaseSeg = (char Piglet_Far *) 0xb8000000L;
  } else {
    VMode = 3;
    VCols = 80;
    VRows = 25;
    VBaseSeg = (char Piglet_Far *) 0xb8000000L;
#ifdef g_RunDebug
    Trace.Printf("%tDesktopConstructor: Video detection failed, assuming defaults\n");
#endif
  }

#ifdef g_RunDebug
  Trace.Printf("%tDesktopConstructor: Mode %5u; Cols %5u; Rows %5u\n", VMode, VCols, VRows);
#endif

  // Fast keyboard

#ifdef g_RunDebug
  Trace.Printf("%tDesktopConstructor: Attempting to install fastkey support\n");
#endif

  regs.h.ah=3;
  regs.h.al=5;
  regs.h.bh=0;
  regs.h.bl=0;

  #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
  int386(0X16,&regs,&regs);
  #else
  int86(0X16,&regs,&regs);
  #endif

  Mouse=0;
  MouseActive=0;
  MousePress=0;

  ident = new char[VRows*VCols];
  if (!ident) MemAbort(2);
  _fmemset(ident, 0 , sizeof(ident));

  StartBackground = new char[VRows*VCols*2];
  if (!StartBackground) MemAbort(3);
  _fmemcpy(StartBackground,VBaseSeg,VRows*VCols*2);  // Copy BackGround

#ifdef g_RunDebug
  Trace.Printf("%tDesktopConstructor: Memory allocation successful\n");
#endif

  //MTask=detect_multitasker();

  DrawDesktop();
}


/**
**    WinFillRowChar
**
** This function writes a row of characters to a specified position in a
** specified window.
**
**    Parameters
**
**    Handle      Handle of the window to write to
**    x, y        Coordinates within the window which the row starts at
**    length      Number of characters in the row
**    attrib      Text attribute or colour
**    ch          The character to repeat to form the row
**
**/
void PigletDesktop::WinFillRowChar(int Handle, int x, int y, int length, char attrib, char c)
{
  PigletWindow * Win = FindWindow(Handle);
  int n;
  char *line;

  if(!Win) return;

  line = new char[length+2];
  if (!line) MemAbort(1);

  for (n=0;n<length;n++) line[n]=c;
  line[n]=0;

  if (Win->BorderFlag == PigletWindow::NoBorder) {
    x--;
    y--;
  }

  WriteBuffer(Win, x, y, attrib, line);
  BufferScreen(Win, x, y, x + length - 1, y);

  delete line;
}


void PigletDesktop::BetaCoreleft(void)
{
  char temp[50];
  char temp1[50];

  strcpy(temp," Beta Coreleft [ ");
  strcat(temp,ultoa(coreleft(),temp1,10));
  strcat(temp," ] ");
//  WinWriteString(desk(),temp,4,Rows-3,0x70);
  ColorMsg(4, VRows-3,0x70,temp);
}


/**
**    WinXMax
**
**    Get X extent of a window.
**
**    Parameters
**
**    Handle      The handle of the window to check.
**
**    Returns
**
**    If successful, the function returns the x extent of the specified
**    window.
**    zero is returned on an error.
**
**/
int PigletDesktop::WinXMax(int Handle)
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return(0);
  return(Win->X2 - Win->X1 + 1);
}


/**
**    WinYMax
**
**    Get Y extent of a window.
**
**    Parameters
**
**    Handle      The handle of the window to check.
**
**    Returns
**
**    If successful, the function returns the y extent of the specified
**    window.
**    zero is returned on an error.
**
**/
int PigletDesktop::WinYMax(int Handle)
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return(0);
  return(Win->Y2 - Win->Y1 + 1);
}


/**
**    WinCX
**
**    Get the centre x coordinate of a specified window
**
**    Parameters
**
**    Handle      The handle of the window to check.
**
**    Returns
**
**    If successful, the function returns the centre x coordinate of the
**    specified window.
**    zero is returned on an error.
**
**/
int PigletDesktop::WinCX(int Handle)
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return(0);
  return((Win->X2 - Win->X1 + 1)/2);
}


/**
**    WinCY
**
**    Get the centre y coordinate of a specified window
**
**    Parameters
**
**    Handle      The handle of the window to check.
**
**    Returns
**
**    If successful, the function returns the centre y coordinate of the
**    specified window.
**    zero is returned on an error.
**
**/
int PigletDesktop::WinCY(int Handle)
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return(0);
  return((Win->Y2 - Win->Y1 + 1)/2);
}


/**
**    StatusLine
**
** Naive status line comment support. This is useful for a single line status
** line at the bottom of the screen only. Texx encapsulated in ^ characters
** will be shown in a highlighted colour.
**
**    Parameters
**
**    string      The string to place in the status line as described above
**
**/
void PigletDesktop::StatusLine(char * string)
{
  StatusLine(string, Desk(), Rows());
}


/**
**    StatusLine
**
** An overloaded form of the above function, this more generalized version
** allows dedicated multiline status windows. It is possible to specify the
** handle of the window and the row within it for writing.
**
**    Parameters
**
**    string      Status string, as detailed above
**    Handle      Window handle to write status line into
**    y           The row of the window (relative) to write into
**
**/
void PigletDesktop::StatusLine(char * string, int Handle, int y)
{
  char *ptr=string;
  char line[80];
  int n,count=0;
  char attrib=0x07;

  if (strlen(string)) {

    WinGotoXY(Handle,1,y);

    while (*ptr) {
      n=0;
      while (*ptr && *ptr!='^') {
        count++;
        line[n++]=*ptr++;
      }
      line[n]=0;
      WinWriteString(Handle,line,attrib);
      if (*ptr=='^') {
        *ptr++;
        if (attrib==0x07) attrib=0x0e;
        else attrib=0x07;
      }
    }
    for (n=0;n<(VCols-count-1);n++) line[n]=' ';
    line[n]=0;
    WinWriteString(Handle,line);

  }
}


#if 0

#define g_ready 0
#define g_read  1

int PigletDesktop::GKey(int flag)
{

  int n,m;
  unsigned int l,k;
  char Piglet_Far *head=(char Piglet_Far *)0x0040001aL;
  char Piglet_Far *tail=(char Piglet_Far *)0x0040001cL;
  char Piglet_Far *temp=(char Piglet_Far *)0x00400000L;


  n=*head;
  m=*tail;

  if (m==n) return 0;

  char Piglet_Far *bleh;
  bleh = temp + n;
  l=*bleh++;
  k=*bleh;


  if (flag==g_read) {
    *head+=2;
    if (*head==0x3e) *head=0X1e;
  }


  if (k==0x00) return((l << 8) >> 8);
  if (l!=0xffe0 && l!=0x0000) return( l + (k << 8));
  return(k << 8);
}

#endif

int PigletDesktop::CheckKey()
{
  if(OS) OS->TimeSlice();
  return(bioskey(_KEYBRD_READY));
  /*while (!bioskey(_KEYBRD_READY) && !quit) {
    // Should we execute the IdleFunction?
    if (OnIdle) {
      // Function has been registered
      if ((long) (time(NULL) - LastIdleTime) > (long) IdleDelay) {
        LastIdleTime = time(NULL);
        OnIdle(this);
      }
    }
    //timeslice();

    if (Mouse && MouseActive) {
      MousePress=0;
      union REGS regs;
      regs.x.ax=5;
      regs.x.bx=0;
      int86(0x33,&regs,&regs);
      if (regs.x.bx) {
        MousePress=1;
        MouseX=regs.x.cx/8;
        MouseY=regs.x.dx/8;
        MouseCursorOff();
        return 0;
      }
      regs.x.ax=5;
      regs.x.bx=1;
      int86(0x33,&regs,&regs);
      if (regs.x.bx) {
        input=27;
        quit=1;
        MousePress=0;
      }
    }
  } */
}


int PigletDesktop::GetKey(int c1,...)
{
  // Time slices while looking for characters in the list you give
  // If c1=zero, returns the first character pressed
  int cn;
  int result;
  int input=0;
  int finished=0;
  int quit;
  va_list ap;

  MouseCursorOn();

  while (!finished) {
    input=0;
    quit=0;
    while (!bioskey(_KEYBRD_READY) && !quit) {
      // Should we execute the IdleFunction?
      if (OnIdle) {
        // Function has been registered
        if ((long) (time(NULL) - LastIdleTime) > (long) IdleDelay) {
          LastIdleTime = time(NULL);
          OnIdle(this);
        }
      }
      if(OS) OS->TimeSlice();

      if (Mouse && MouseActive) {
        MousePress=0;
        union REGS regs;
        #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
        regs.x.eax=5;
        regs.x.ebx=0;
        int386(0x33,&regs,&regs);
        #else
        regs.x.ax=5;
        regs.x.bx=0;
        int86(0x33,&regs,&regs);
        #endif
        
        #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
        if (regs.x.ebx) {
          MousePress = 1;
          MouseX=regs.x.ecx / 8;
          MouseY=regs.x.edx / 8;
          MouseCursorOff();
          return 0;
        }
        #else
        if (regs.x.bx) {
          MousePress = 1;
          MouseX=regs.x.cx / 8;
          MouseY=regs.x.dx / 8;
          MouseCursorOff();
          return 0;
        }
        #endif

        #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
        regs.x.eax=5;
        regs.x.ebx=1;
        int386(0x33,&regs,&regs);
        #else
        regs.x.ax=5;
        regs.x.bx=1;
        int86(0x33,&regs,&regs);
        #endif
        
        #if (defined(__386__) && defined(__DOS__)) || defined(__NT__) || defined(__OS2__)
        if (regs.x.ebx) {
          input = 27;
          quit = 1;
          MousePress = 0;
        }
        #else
        if (regs.x.bx) {
          input = 27;
          quit = 1;
          MousePress = 0;
        }
        #endif
      }
    }
    // Ok, a key has been pressed so let's find out what.


    if (!input) input=bioskey(_KEYBRD_READ);

    // is c1=0?
    if (c1==0) {
      MouseCursorOff();
      if ((input & 0xFF)==0) return(input);
      else return(input & 0xFF);
    }

    // Now compare input with each of our parameters
    va_start(ap, c1);
    if (((c1 & 0xFF) == c1) && ((input & 0xFF) == c1)) {
      result=c1;
      finished=1;
    }
    if (input==c1) {
      result=input;
      finished=1;
    }
    if (!finished) {
      while ((cn=va_arg(ap, int))!=0) {
        if (((cn & 0xFF) == cn) && ((input & 0xFF) == cn)) {
          result=cn;
          finished=1;
        }
        if (input==cn) {
          result=input;
          finished=1;
        }
      }
    }
  }
  va_end(ap);  // Hmmm this doesn't always get called ?
  MouseCursorOff();
  return(result);
}


// Destructor - closes down the desktop
PigletDesktop::~PigletDesktop(void)
{
  PigletDoubleVListIterator<PigletWindow> it(WindowList);

  if (!Shutdown) {
    Shutdown=1;
    // Colin
/*	 while(WindowList.GetItemsInContainer()){
    it.restart();
    WinDelete(it.current().get_Handle());
   }*/

    //WindowList.empty;
    _fmemcpy(VBaseSeg,StartBackground,VRows*VCols*2);
    _GotoXY(StartX+1,StartY+1);
    // Colin
    delete StartBackground;
    delete ident;
    Cursor(0x06,0x07);

  }
}


int PigletDesktop::CY(void)
// Returns y centre of desktop
{
  if (DesktopFlag == PigletDesktop::FDDesktop) {
    return(((VRows-4)/2)+2);
  } else return((VRows-4)/2);
}


int PigletDesktop::GetActive(void)
// Returns the Active window
{
  return Active;
}


/**
**    WinInit
**
** This is a common core for all the various WinInit functions. It creates and
** allocates space for a new window (but does not draw it).
**
**    Parameters
**
**    x1, y1, x2, y2    Window coordinates as per normal desktop convention
**    TopTitle          The TopTitle for the window (if any, NULL indicates
**                      none)
**    TJust             Justification for TopTitle, see documentation
**    BottomTitle       As per TopTitle, only at the bottom ;-)
**    BJust             As per TJust, only at the bottom
**    lock              Boolean, window is locked on 1, not on 0
**
**    Returns
**
**    Window Handle on success, 0 on failure (note however that one window
**    legitimately has a handle of 0 - the desktop background itself)
**
**/
int PigletDesktop::WinInit(int x1, int y1, int x2, int y2, char Piglet_Far * TopTitle, int TJust, char Piglet_Far * BottomTitle, int BJust, int lock)
{
  PigletWindow * ToAdd;

  // Validation
  if((x2 - x1) < 1) return 0;
  if((y2 - y1) < 1) return 0;

  if(x2 < 0 || x2 > VCols) return 0;
  if(y2 < 0 || y2 > VRows)    return 0;

  ToAdd = new PigletWindow(x1, y1, x2, y2);

  ToAdd->BorderFlag = BorderFlag;
  ToAdd->BackAttr = PigletColours.DefWin;
  ToAdd->BorderAttr = PigletColours.DefWinBorder;
  ToAdd->BorderBGrndAttr = PigletColours.DefWinBorderBGrnd;
  ToAdd->Buffer = new char[((x2-x1)+3)*((y2-y1)+3)*2];
  if (!ToAdd->Buffer) MemAbort(4);

  memset(ToAdd->Buffer,0,((x2-x1)+3)*((y2-y1)+3)*2);

  ToAdd->Shadow = 1;
  ToAdd->CX=ToAdd->CY = 1;
  ToAdd->CursorScanStart = 0X20;
  ToAdd->CursorScanEnd = 0x00;
  ToAdd->Locked = lock;

  if(TopTitle){
    ToAdd->TopTitle=new char[_fstrlen(TopTitle)+1];
    if (!ToAdd->TopTitle) MemAbort(4);
    _fstrcpy(ToAdd->TopTitle, TopTitle);
    ToAdd->TJust=TJust;
    ToAdd->TitleAttr=PigletColours.DefWinTitle;
  }

  if(BottomTitle){
    ToAdd->BottomTitle=new char [_fstrlen(BottomTitle)+1];
    if (!ToAdd->BottomTitle) MemAbort(4);
    _fstrcpy(ToAdd->BottomTitle, BottomTitle);
    ToAdd->BJust=BJust;
    ToAdd->TitleAttr=PigletColours.DefWinTitle;
  }

  ToAdd->Handle=NewHandle++;
  WindowList.AddObject(ToAdd);
  return(NewHandle-1);
}


int PigletDesktop::WinInit(int x1, int y1, int x2, int y2)
{
  return(WinInit(x1, y1, x2, y2, NULL, 0, NULL, 0, 0));
}


int PigletDesktop::WinInit(int x1, int y1, int x2, int y2, char Piglet_Far *TopTitle, int TJust, char Piglet_Far *BottomTitle, int BJust)
{
  return(WinInit(x1, y1, x2, y2, TopTitle, TJust, BottomTitle, BJust, 0));
}


int PigletDesktop::WinInitLock(int x1, int y1, int x2, int y2)
{
  return(WinInit(x1, y1, x2, y2, NULL, 0, NULL, 0, 1));
}


int PigletDesktop::WinInitLock(int x1, int y1, int x2, int y2, char Piglet_Far *TopTitle, int TJust, char Piglet_Far *BottomTitle, int BJust)
{
  return(WinInit(x1, y1, x2, y2, TopTitle, TJust, BottomTitle, BJust, 1));
}


int PigletDesktop::WinDraw(int Active_Handle)
{
  return(WinDrawOpen(Active_Handle,0));
}

int PigletDesktop::WinOpen(int Active_Handle)
{
  return(WinDrawOpen(Active_Handle,1));
}


/**
**    WinDrawOpen
**
** This function either opens or draws a specified window, detailed by a given
** handle.
**
**    Parameters
**
**    Handle    Handle of the window
**    flag      0 represents WinDraw, 1 represents WinOpen
**
**    Returns
**
**    1 on success, 0 on failure (window already open etc.)
**
**/
int PigletDesktop::WinDrawOpen(int Handle, int flag)
{
  PigletWindow * Win = FindWindow(Handle);
  PigletWindow * OldTopWin = FindWindow(GetActive());

  // Can't find the window
  if(!Win) return (0);

  // Is the window open already?
  if (!Win->Open) {

    Win->Open=1;

    int n,m;

    if (Win->Shadow == PigletWindow::ShadowOn) {
      Win->Y2++;
      Win->X2++;
    }

    // We need to indicate that our new window is uppermost on its character
    // matrix
    for (m = Win->Y1; m <= Win->Y2; m++) {
      for (n = Win->X1; n <= Win->X2; n++) {        // Suppa duppa fast window read system
        ident[((m-1)*VCols)+(n-1)] = (char) Handle;
      }
    }

    if (Win->Shadow == PigletWindow::ShadowOn) {
      Win->Y2--;
      Win->X2--;
    }

    // Set the cursor type and position our new window requires
    Cursor(Win->CursorScanStart, Win->CursorScanEnd);
    if (!(Win->CursorScanStart == 0X20 && Win->CursorScanEnd == 0x00)) {
      if (Win->BorderFlag == PigletWindow::NoBorder) _GotoXY(Win->CX + Win->X1 - 1, Win->CY + Win->Y1 - 1);
      else _GotoXY(Win->CX + Win->X1, Win->CY + Win->Y1);
    }

    // OldTopWin is currently on top
    if (OldTopWin->Open && !OldTopWin->Locked) {
      if (OldTopWin->BorderFlag != PigletWindow::NoBorder) {
        // Remove the focus highlight from the old window
        FillRowAttr(OldTopWin->X1,OldTopWin->Y1,OldTopWin->X2-OldTopWin->X1+1,OldTopWin->BorderBGrndAttr);
        FillRowAttr(OldTopWin->X1,OldTopWin->Y2,OldTopWin->X2-OldTopWin->X1+1,OldTopWin->BorderBGrndAttr);
        FillColAttr(OldTopWin->X1,OldTopWin->Y1+1,OldTopWin->Y2-OldTopWin->Y1-1,OldTopWin->BorderBGrndAttr);
        FillColAttr(OldTopWin->X2,OldTopWin->Y1+1,OldTopWin->Y2-OldTopWin->Y1-1,OldTopWin->BorderBGrndAttr);

        if (OldTopWin->Shadow==PigletWindow::ShadowOn) {
          OldTopWin->Y2++;
          OldTopWin->X2++;
        }

        // Refresh its contents one more time
        for (n=0;n<=(OldTopWin->Y2-OldTopWin->Y1);n++) {
          _fmemcpy(OldTopWin->Buffer+(n*(OldTopWin->X2-OldTopWin->X1+1)*2),VBaseSeg+((n+OldTopWin->Y1-1)*2*VCols)+((OldTopWin->X1-1)*2),((OldTopWin->X2-OldTopWin->X1+1)*2));
        }

        if (OldTopWin->Shadow==PigletWindow::ShadowOn) {
          OldTopWin->Y2--;
          OldTopWin->X2--;
        }
      }
    }

    // Sets Active window to the new one
    Active = Win->Handle;

    // Move object to start of list
    WindowList.UnlinkObject(Win);
    WindowList.AddObjectAtStart(Win);

    // Draw or open the window as required
    if (flag==0) DrawWindow(Win->X1,Win->Y1,Win->X2,Win->Y2,Win->BorderFlag,Win->BorderAttr,Win->BackAttr,Win->BackCh,Win->TopTitle,Win->TJust,Win->BottomTitle,Win->BJust,Win->TitleAttr,Win->Shadow);
    if (flag==1) BufferScreen(Win,Win->X1,Win->X2,Win->Y1,Win->Y2);

    // Deal with the shadow if required
    if (Win->Shadow==PigletWindow::ShadowOn) {
      if (Win->Y2!=VRows) {
        if (Win->X2==VCols) FillRowAttr(Win->X1+2,Win->Y2+1,(Win->X2-(Win->X1+1)),0x08);
        else FillRowAttr(Win->X1+2,Win->Y2+1,(Win->X2-Win->X1),0x08);
      }
      if (Win->X2!=VCols) {
        if (Win->Y2==VRows) FillColAttr(Win->X2+1,Win->Y1+1,(Win->Y2-Win->Y1),0x08);
        else FillColAttr(Win->X2+1,Win->Y1+1,(Win->Y2-Win->Y1)+1,0x08);
      }
      Win->X2++;
      Win->Y2++;
    }

    // Refresh the buffer
    for (n=0;n<=(Win->Y2-Win->Y1);n++) {
      _fmemcpy(Win->Buffer+(n*(Win->X2-Win->X1+1)*2),VBaseSeg+((n+Win->Y1-1)*2*VCols)+((Win->X1-1)*2),((Win->X2-Win->X1+1)*2));
    }

    if (Win->Shadow==PigletWindow::ShadowOn) {
      Win->X2--;
      Win->Y2--;
    }
    return 1;  // Succesful opening of window
  }
  return 0;  // The window already was open !

}


void PigletDesktop::SetWinTitleAttr(int ActiveHandle, char tattr)
{
  PigletWindow * Win = FindWindow(ActiveHandle);

  if(!Win) return;
  Win->TitleAttr = tattr;
}


void PigletDesktop::SetWinBackgroundChar(int ActiveHandle, char ch)
{
  PigletWindow * Win = FindWindow(ActiveHandle);

  if(!Win) return;
  Win->BackCh = ch;
}


void PigletDesktop::SetWinBorder(int ActiveHandle, int newBorderFlag)
{
  PigletWindow * Win = FindWindow(ActiveHandle);
  
  if(!Win) return;
  Win->BorderFlag = newBorderFlag;
  if (newBorderFlag == PigletWindow::NoBorder) Win->Shadow = PigletWindow::ShadowOff;
}


void PigletDesktop::SetWinBorderAttr(int ActiveHandle, char newBorderAttr)
{
  PigletWindow * Win = FindWindow(ActiveHandle);

  if(!Win) return;
  Win->BorderAttr = newBorderAttr;
}


void PigletDesktop::SetWinBorderBGrndAttr(int ActiveHandle, char newBorderAttr)
{
  PigletWindow * Win = FindWindow(ActiveHandle);

  if(!Win) return;
  Win->BorderBGrndAttr = newBorderAttr;
}


void PigletDesktop::SetWinBackgroundAttr(int ActiveHandle, char newBackAttr)
{
  PigletWindow * Win = FindWindow(ActiveHandle);

  if(!Win) return;
  Win->BackAttr = newBackAttr;
}


void PigletDesktop::WinLock(int ActiveHandle)
{
  PigletWindow * Win = FindWindow(ActiveHandle);
  
  if(!Win) return;
  Win->Locked = 1;
}


void PigletDesktop::WinUnlock(int ActiveHandle)
{
  PigletWindow * Win = FindWindow(ActiveHandle);
  
  if(!Win) return;
  Win->Locked = 0;
}


void PigletDesktop::SetWinShadow(int ActiveHandle, int shadowStatus)
{
  PigletWindow * Win = FindWindow(ActiveHandle);

  if(!Win) return;
  Win->Shadow = shadowStatus;
}


void PigletDesktop::WinCloseForeground(void)
{
  WinClose(GetActive());
}


void PigletDesktop::WinClose(int ActiveHandle)
{
  PigletWindow * Win = FindWindow(ActiveHandle);

  if(!Win) return;

  if (Win->Open) {
    RefreshScreen(Win);
    Win->Open=0;

    PigletDoubleVListIterator<PigletWindow> it(WindowList);
    Win = it.GoToStart();
    Active = Win->Handle;
  }

  
  if (Win->Open && !Win->Locked ) {  // dh - desktop Handle
    if (Win->BorderFlag != PigletWindow::NoBorder) {
      // Draw the frame
      DrawFrame(Win->X1, Win->Y1, Win->X2, Win->Y2, Win->BorderAttr, Win->TopTitle, 
                Win->TJust, Win->BottomTitle, Win->BJust, Win->TitleAttr, Win->BorderFlag);

      int n;

      if (Win->Shadow == PigletWindow::ShadowOn) {
        Win->X2++;
        Win->Y2++;
      }

      for (n = 0; n <= (Win->Y2 - Win->Y1); n++) {
        _fmemcpy(Win->Buffer+(n*(Win->X2-Win->X1+1)*2),VBaseSeg+((n+Win->Y1-1)*2*VCols)+((Win->X1-1)*2),((Win->X2-Win->X1+1)*2));
      }

      if (Win->Shadow == PigletWindow::ShadowOn) {
        Win->X2--;
        Win->Y2--;
      }
    }
  }
}


/**
**    WinDelete
**
** Closes a window if open, but also deallocates all storage for it and removes
** it from the window list.
**
**    Parameters
**
**    Handle      The window handle to delete
**
**    Returns
**
**/
int PigletDesktop::WinDelete(int Handle)
{
  PigletWindow * Win = FindWindow(Handle);
  
  if(!Win) return(0);
  WinClose(Handle);

  delete Win->Buffer;
  delete Win->TopTitle;
  delete Win->BottomTitle;
  WindowList.RemoveObject(Win);
  return 0;
}


void PigletDesktop::SetWinCursorType(int Handle, int ctype)
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;

  if (ctype == PigletWindow::CursorBig) {
    Win->CursorScanStart=0x00;
    Win->CursorScanEnd=0x07;
  }
  if (ctype == PigletWindow::CursorSmall) {
    Win->CursorScanStart=0x06;
    Win->CursorScanEnd=0x07;
  }
  if (ctype == PigletWindow::CursorOff) {
    Win->CursorScanStart=0X20;
    Win->CursorScanEnd=0x00;
  }

  if (Handle == GetActive()) Cursor(Win->CursorScanStart, Win->CursorScanEnd);
}


void PigletDesktop::DesktopMessage(int y, char attrib, char *text)
{
  int x;
  x=strlen(text);
  x=(VCols/2)-(x/2);
  WinWriteString(dh,text,x,y,attrib);
}


void PigletDesktop::WinGotoXY(int Handle, int x, int y)
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;

  if (x<1) x=1;
  if (y<1) y=1;

  if (Win->BorderFlag != PigletWindow::NoBorder) {
    if (x >= Win->X2 - Win->X1) x = Win->X2 - Win->X1 - 1;
    if (y >= Win->Y2 - Win->Y1) y = Win->Y2 - Win->Y1 - 1;
  } else {
    if (x > Win->X2 - Win->X1 + 1) x = Win->X2 - Win->X1 + 1;
    if (y > Win->Y2 - Win->Y1 + 1) y = Win->Y2 - Win->Y1 + 1;
  }

  Win->CX = x;
  Win->CY = y;

  if (Handle == GetActive()) {
    if (Win->BorderFlag == PigletWindow::NoBorder) {
      x--;
      y--;
    }
    if (!(Win->CursorScanStart == 0x20 && Win->CursorScanEnd == 0x00)) _GotoXY(Win->X1 + x, Win->Y1 + y);
  }
}

// Draw line functions

void PigletDesktop::WinLineY(int Handle, int y)
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;

  int length = Win->X2 - Win->X1 + 1;
  WinLineY(Handle, 1, y, length, Win->BorderAttr, Win->BorderFlag);
}


void PigletDesktop::WinLineY(int Handle, int x, int y, int length )
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;

  WinLineY(Handle, x, y, length, Win->BorderAttr, Win->BorderFlag);
}


void PigletDesktop::WinLineYStyle(int Handle, int y, char attrib, int style )
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;

  int length = Win->X2 - Win->X1 + 1;
  int x = 1;

  if ( (style!=Win->BorderFlag) && (Win->BorderFlag!=PigletWindow::NoBorder) ) {
    length-=2;
    x++;
  }
  WinLineY(Handle, x, y, length, attrib, style);
}


void PigletDesktop::WinLineY(int Handle, int x, int y, int length, char attrib, int type )
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;

  x--;
  if (Win->BorderFlag == PigletWindow::NoBorder) y--;

  if (Win->Open && (y>=0) && (y<=(1+Win->Y2-Win->Y1)) ) {
    char br,h,lb,v,tr,lt,c,vr,rv,th,hb;

    if (type==PigletWindow::SingleBorder) {
      br = 'Ú';
      h  = 'Ä';
      lb = '¿';
      v  = '³';
      tr = 'À';
      lt = 'Ù';
      c  = 'Å';
      vr = 'Ã';
      rv = '´';
      th = 'Á';
      hb = 'Â';
    } else
      if (type==PigletWindow::DoubleBorder) {
      br = 'É';
      h  = 'Í';
      lb = '»';
      v  = 'º';
      tr = 'È';
      lt = '¼';
      c  = 'Î';
      vr = 'Ì';
      rv = '¹';
      th = 'Ê';
      hb = 'Ë';
    }

    char *line;
    line = new char[length+1];
    if (!line) MemAbort(1);

    int n;
    char t,b;
    int top, bottom;
    for (n=0;n<length;n++) line[n]=h;
    line[n]=0;
    for (n=0;n<length;n++) {
      top=bottom=0;
      if (y!=0) {
        t=ReturnChar(n+Win->X1+x,Win->Y1+y-1);
        if (t==rv || t==v || t==lb || t==br || t==hb || t==vr || t==c) top=1;
      }
      if (y!=Win->Y2-Win->Y1+1) {
        b=ReturnChar(n+Win->X1+x,Win->Y1+y+1);
        if (b==rv || b==v || b==lt || b==tr || b==th || b==vr || b==c ) bottom=1;
      }
      if (top && bottom) {
        if (n==0) line[n]=vr;
        else
          if (n==length-1) line[n]=rv;
        else
          line[n]=c;
      } else
        if (top) {
        if (n==0) line[n]=tr;
        else
          if (n==length-1) line[n]=lt;
        else
          line[n]=th;
      } else
        if (bottom) {
        if (n==0) line[n]=br;
        else
          if (n==length-1) line[n]=lb;
        else
          line[n]=hb;
      }
    }
//  ColorMsg(w.X1+x,w.Y1+y,attrib,line);
    Win = WriteRawH(Win,x,y,line,attrib);
    BufferScreen(Win,x,y,Win->X2-Win->X1,y);

    delete line;
  }
}


void PigletDesktop::WinLineX(int Handle, int x )
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;

  int length = Win->Y2 - Win->Y1 + 1;
  WinLineX(Handle, x, 1, length, Win->BorderAttr);
}


void PigletDesktop::WinLineX(int Handle, int x, int y, int length )
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;

  WinLineX(Handle, x, y, length, Win->BorderAttr);
}


void PigletDesktop::WinLineX(int Handle, int x, int y, int length, char attrib )
{
  PigletWindow * Win = FindWindow(Handle);

  y--;
  if (Win->BorderFlag == PigletWindow::NoBorder) x--;


  if (Win->Open && (x>=1) && (x<=(1 + Win->X2 - Win->X1)) ) {
    char br,h,lb,v,tr,lt,c,vr,rv,th,hb;

    if (Win->BorderFlag==PigletWindow::SingleBorder) {
      br = 'Ú';
      h  = 'Ä';
      lb = '¿';
      v  = '³';
      tr = 'À';
      lt = 'Ù';
      c  = 'Å';
      vr = 'Ã';
      rv = '´';
      th = 'Á';
      hb = 'Â';
    } else
      if (Win->BorderFlag==PigletWindow::DoubleBorder) {
      br = 'É';
      h  = 'Í';
      lb = '»';
      v  = 'º';
      tr = 'È';
      lt = '¼';
      c  = 'Î';
      vr = 'Ì';
      rv = '¹';
      th = 'Ê';
      hb = 'Ë';
    }
    char *line;
    line = new char[length+1];
    if (!line) MemAbort(1);

    int n;
    char l,r;
    int xleft, xright;
    for (n=0;n<length;n++) line[n]=v;
    line[n]=0;
    for (n=0;n<length;n++) {
      xleft=xright=0;
      if (x!=0) {
        l=ReturnChar(Win->X1 + x - 1, n + Win->Y1 + y);
        if (l==tr || l==br || l==th || l==hb || l==vr || l==h || l==c) xleft=1;
      }
      if (x!=Win->X2 - Win->X1 + 1) {
        r=ReturnChar(Win->X1 + x + 1, n + Win->Y1 + y);
        if (r==rv || r==lb || r==lt || r==th || r==hb || r==h || r==c ) xright=1;
      }
      if (xleft && xright) {
        if (n==0) line[n]=hb;
        else
          if (n==length-1) line[n]=th;
        else
          line[n]=c;
      } else
        if (xleft) {
        if (n==0) line[n]=lb;
        else
          if (n==length-1) line[n]=br;
        else
          line[n]=rv;
      } else
        if (xright) {
        if (n==0) line[n]=br;
        else
          if (n==length-1) line[n]=lb;
        else
          line[n]=vr;
      }
    }
    // for(n=0;n<length;n++) FillRowChar(w.X1+x,w.Y1+n+y,1,attrib,line[n]);
    Win = WriteRawV(Win, x, y, line, attrib);
    BufferScreen(Win, x, y, x, Win->Y2 - Win->Y1);

    delete line;
  }
}



// Writing text functions

// 0 - dsp, 1- color
void PigletDesktop::WinWriteString(int Active_string, char *string)
{
  WinWriteStringCommon(Active_string, string, 0, 0);
}

void PigletDesktop::WinWriteString(int Active_Handle, char *string, int x, int y)
{
  WinGotoXY(Active_Handle,x,y);
  WinWriteStringCommon(Active_Handle, string, 0, 0);
}

void PigletDesktop::WinWriteString(int Handle, char *string, int x, int y, char attrib, int length)
{
  WinGotoXY(Handle,x,y);
  char temp[300];
  strcpy(temp,string);
  temp[length]=0;
  WinWriteStringCommon(Handle,temp,attrib,1);
}

void PigletDesktop::WinWriteStringFill(int Handle, char *string, int x, int y, char attrib, int length)
{
  WinGotoXY(Handle,x,y);

  if (strlen(string) > length) string[length]=0;
  char *temp;
  temp = new char[length+2];
  if (!temp) MemAbort(1);

  strcpy(temp,string);
  int n;
  n = length-strlen(string);
  for (n=strlen(string);n<length;n++) temp[n]=' ';
  temp[n]=0;

  WinWriteStringCommon(Handle,temp,attrib,1);

  delete temp;
}


void PigletDesktop::WinWriteStringCentre(int Handle, char * string, int y)
{
  PigletWindow * Win = FindWindow(Handle);
  int x;

  x=(Win->X2 - Win->X1 + 1)/2;
  x-=((strlen(string))/2);

  WinGotoXY(Handle, x, y);
  WinWriteStringCommon(Handle, string, 0, 0);
}


void PigletDesktop::WinWriteString(int Active_string, char *string, char attrib)
{
  WinWriteStringCommon(Active_string, string, attrib, 1);
}


void PigletDesktop::WinWriteString(int Handle, char * string, int x, int y, char attrib)
{
  WinGotoXY(Handle, x, y);
  WinWriteStringCommon(Handle, string, attrib, 1);
}


// Write to the border
void PigletDesktop::WinWriteBorder(int Handle, char * string, int x, char attrib)
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;
  if (Win->BorderFlag != PigletWindow::NoBorder) {
    Win = WriteBuffer(Win, x, 0, attrib, string);
    BufferScreen(Win, x, 0, x + strlen(string) + 1, 0);
  }
}


void PigletDesktop::WinBorderLine(int Handle, int x, int length)
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;
  if (Win->BorderFlag != PigletWindow::NoBorder) {
    char h;
    if (Win->BorderFlag == PigletWindow::SingleBorder) h  = 'Ä';
    else h  = 'Í';
    WinFillRowChar(Handle, x, 0, length, Win->BorderAttr, h);
  }
}

// Character writing functions

void PigletDesktop::WinWriteChar(int Handle, char character, int x, int y)
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;

  char string[3];
  string[0]=character;
  string[1]=0;

  if (Win->BorderFlag == PigletWindow::NoBorder) {
    x--;
    y--;
  }

  Win = WriteBuffer(Win, x, y, Win->BackAttr, string);
  BufferScreen(Win, x, y, x, y);
}


void PigletDesktop::WinWriteChar(int Handle, char character, int x, int y, char attrib)
{
  PigletWindow * Win = FindWindow(Handle);

  if(!Win) return;

  char string[3];
  string[0]=character;
  string[1]=0;

  if (Win->BorderFlag == PigletWindow::NoBorder) {
    x--;
    y--;
  }


  Win = WriteBuffer(Win, x, y, attrib, string);
  BufferScreen(Win, x, y, x, y);
}


void PigletDesktop::BottomStatusLine(void)
{
  FillRowChar(1,VRows-1,VCols,PigletColours.StatLineBottom,'Ä');
  FillRowChar(1,VRows,VCols,PigletColours.MenuBar,' ');
}


int PigletDesktop::GrabString(int Handle, char *string, int x, int y, int type, int width, int slength)
{
  return(GrabString(Handle,string,x,y,type,width,slength,PigletColours.InputAttr,PigletColours.OutputAttr));
}


int PigletDesktop::GrabString(int Handle, char *string, int x, int y, int type, int width, int slength, char InputAttr, char OutputAttr)
{
  // Types are as follows
  // 0 - normal
  // 1 - path type ( no spaces, and capatilise
  // 2 - address type
  // 3 - password
  // 4 - No spaces allowed


  int currentlength;  // length of string
  int xpos=0;  // position on the entry bar
  int apos=0;  // actual position in the string
  char Activestring[255];  // the string we manipulate
  char *start;  // the portion of the Active string currently displayed
  char temp[255]; // temporary storage
  int n;
  int insert=0;  // insert mode on

  SetWinCursorType(Handle,PigletWindow::CursorSmall);
  strcpy(Activestring,string);
  apos=currentlength=strlen(Activestring);
  strcpy(temp,Activestring);
  start=temp;
  if (currentlength>width) start+=(currentlength-width);

  WinWriteString(Handle,start,x,y,InputAttr);
  xpos=strlen(start);

  WinFillRowChar(Handle,x+xpos,y,width-strlen(start),InputAttr,'°');
  WinGotoXY(Handle,x+xpos,y);

  do {
    n=GetKey(0);
    if (n==INSERT) {  // Toggle insert mode
      if (insert==1) {
        insert=0;
        SetWinCursorType(Handle,PigletWindow::CursorSmall);
      } else {
        insert=1;
        SetWinCursorType(Handle,PigletWindow::CursorBig);
      }
    }
    if (n==DELETE) {  // Delete
      if (apos!=strlen(Activestring)) {
        int k;
        strcpy(temp,Activestring);
        for (k=apos+1;k<=strlen(Activestring);k++) temp[k-1]=Activestring[k];
        strcpy(Activestring,temp);
        currentlength--;
        if (apos-xpos>0) {
          start--;
          apos--;
        }
        temp[apos-xpos+width]='\0';

        WinWriteString(Handle,start,x,y,InputAttr);

        if (strlen(Activestring)<width) {
          WinFillRowChar(Handle,x+strlen(Activestring),y,width-strlen(Activestring),InputAttr,'°');
        }
        WinGotoXY(Handle,x+xpos,y);
      }
    }
    if (n==8) {  // Backspace
      if (xpos!=0) {
        int k;
        strcpy(temp,Activestring);
        for (k=apos;k<=strlen(Activestring);k++) temp[k-1]=Activestring[k];
        strcpy(Activestring,temp);
        apos--;
        if (!(xpos==width && apos-xpos>=0)) xpos--;
        currentlength--;
        temp[apos-xpos+width]='\0';
        start=temp+(apos-xpos);
        WinWriteString(Handle,start,x,y,InputAttr);
        WinGotoXY(Handle,x+xpos,y);
        if (strlen(Activestring)<width) {
          WinFillRowChar(Handle,x+strlen(Activestring),y,width-strlen(Activestring),InputAttr,'°');
        }

      } else {
        if (apos!=0) {
          int k;
          strcpy(temp,Activestring);
          for (k=apos;k<=strlen(Activestring);k++) temp[k-1]=Activestring[k];
          strcpy(Activestring,temp);
          apos--;
          currentlength--;
          start=temp+(apos-xpos);
        }
      }
    }
    if (n==END) {  // End key
      if (xpos==width-1) {
        apos=strlen(Activestring);
        xpos=width;
        strcpy(temp,Activestring);
        start=temp+(strlen(Activestring)-width);
        WinWriteString(Handle,start,x,y,InputAttr);
      } else {
        if (strlen(Activestring)<=width) {
          xpos=apos=strlen(Activestring);
        } else {
          if (apos-xpos+width==strlen(Activestring)) {
            xpos=width;
            apos=strlen(Activestring);
          } else {
            apos=apos-xpos+width-1;
            xpos=width-1;
          }
        }
      }
      WinGotoXY(Handle,x+xpos,y);
    }
    if (n==HOME) {  // Home key
      if (xpos!=0) {
        apos=apos-xpos;
        xpos=0;
        WinGotoXY(Handle,x+xpos,y);
      } else {
        xpos=apos=0;
        strcpy(temp,Activestring);
        start=temp;
        temp[apos+(width-xpos)]='\0';
        WinWriteString(Handle,start,x,y,InputAttr);
        WinGotoXY(Handle,x+xpos,y);
      }
    }
    if (n==CUR_LEFT) {   // Move Cursor to the left
      if (xpos!=0) {
        xpos--;
        apos--;
        WinGotoXY(Handle,x+xpos,y);
      } else {
        if (apos!=0) {
          apos--;  // hmmmmmmm
          strcpy(temp,Activestring);
          temp[apos+(width-xpos)]='\0';
          start--;
          WinWriteString(Handle,start,x,y,InputAttr);
          WinGotoXY(Handle,x+xpos,y);
        }
      }
    }

    if (n==CUR_RIGHT) {  // Move Cursor to right
      if (apos!=strlen(Activestring)) {
        if (xpos<width-1) {
          xpos++;
          apos++;
          WinGotoXY(Handle,x+xpos,y);
        } else {
          if (apos!=currentlength) {
            apos++;
            strcpy(temp,Activestring);
            temp[apos-xpos+width]='\0';
            if (apos!=strlen(Activestring)) start++;
            if (!(apos<currentlength)) {
              xpos++;
            }
            WinWriteString(Handle,start,x,y,InputAttr);
            WinGotoXY(Handle,x+xpos,y);
          }
        }
      }
    }

    //Translate characters
    // 0 - normal
    // 1 - path type ( no spaces, and capatilise )
    // 2 - address type
    // 3 - password
    // 4 - no spaces

    if (type==1) {
      if (n==32) n='_';
      if (n>=97 && n<=122) n-=32;
    }
    if (type==2) {
      if (n!=13 && n!=27) if (!(n>=46 && n<=58)) n=0;
    }
    if (type==4) {
      if (n!=13 && n!=27) if (n==32) n='_';
    }

    if (n && n!=HOME && n!=END && n!=CUR_LEFT && n!=CUR_RIGHT && n!=8 && n!=DELETE && n!=INSERT && n!=13 && !(n>>8)) {
      // Normal Character
      if (apos==currentlength) {
        if (currentlength!=slength) {
          currentlength=strlen(Activestring)+1;
          Activestring[apos]= (char) n;
          Activestring[currentlength]='\0';
          apos++;
          if (xpos!=width) xpos++;
          strcpy(temp,Activestring);
          start=temp;
          if ((apos-width)>0) start+=(apos-width);
          WinWriteString(Handle,start,x,y,InputAttr);
          WinGotoXY(Handle,x+xpos,y);
        }
      } else {
        if (xpos<width-1) {
          if (insert) {
            if (currentlength!=slength) {
              int k;
              currentlength++;
              strcpy(temp,Activestring);
              temp[apos]= (char) n;
              for (k=apos;k<=strlen(Activestring);k++) temp[k+1]=Activestring[k];
              strcpy(Activestring,temp);
              apos++;
              xpos++;
              temp[apos-xpos+width]='\0';
              WinWriteString(Handle,start,x,y,InputAttr);
              WinGotoXY(Handle,x+xpos,y);
            }
          } else {
            Activestring[apos]= (char) n;
            apos++;
            xpos++;
            strcpy(temp,Activestring);
            temp[apos-xpos+width]='\0';
            WinWriteString(Handle,start,x,y,InputAttr);
            WinGotoXY(Handle,x+xpos,y);
          }
        } else {
          if (insert) {
            if (currentlength!=slength) {
              int k;
              currentlength++;
              strcpy(temp,Activestring);
              temp[apos]= (char) n;
              for (k=apos;k<=strlen(Activestring);k++) temp[k+1]=Activestring[k];
              strcpy(Activestring,temp);
              apos++;
              temp[apos-xpos+width]='\0';
              start++;
              WinWriteString(Handle,start,x,y,InputAttr);
              WinGotoXY(Handle,x+xpos,y);
            }
          } else {
            Activestring[apos]= (char) n;
            apos++;
            strcpy(temp,Activestring);
            temp[apos-xpos+width]='\0';
            if (apos==currentlength) xpos++;
            else start++;
            WinWriteString(Handle,start,x,y,InputAttr);
            WinGotoXY(Handle,x+xpos,y);
          }
        }
      }
    }
  } while (n!=13 && n!=27);
  // Write string
  if (n==13) strcpy(string,Activestring);
  strcpy(temp,string);
  start=temp;
  WinFillRowChar(Handle,x,y,width,OutputAttr,' ');
  if (type==3) {
    int max;
    max=strlen(start);
    if (max>width) max=width;
    WinFillRowChar(Handle,x,y,max,OutputAttr,'þ');
  } else {
    temp[width]='\0';
    WinWriteString(Handle,start,x,y,OutputAttr);
  }
  
  SetWinCursorType(Handle,PigletWindow::CursorOff);
  return(n);
}




/****************************************************************************
*                                                                           *
*                             T H E   E N D                                 *
*                                                                           *
*****************************************************************************/


/*
** Piglet Productions
**
** FileName       : Desktop.H
**
** Defines        : PigletWindow
**                  PigletDesktop
**                  PigletHBarData
**                  PigletHBarMenu
**                  PigletVBarData
**                  PigletVBarMenu
**                  PigletScrollCols
**                  PigletScrollLine
**                  PigletScrollMenu
**                  PigletTagSMenu <- PigletScrollMenu
**                  PigletDialogStorage
**                  PigletDialog
**                  PigletVMenuTemplate
**
** Description
**
** Provides a text mode "graphics" library; allowing manipulation of windows
** menus, mouse control and other goodies.
**
** A full set of documentation for all these features exists.
**
**
** Initial Coding : Chris McCandless
**
** Date           : 1995
**
**
** Copyright applies on this file, and distribution may be limited.
*/

/*
** Revision 2.02
**
** Colin Turner, 20th April 1999
**
** Fixed a bug in RefreshWindow()
** 
** The function names of the menus have been revamped to bring about greater
** consistency.
**
** Attributes have been changed from int types to the more correct char.
**
** Now compiles under 16bit DOS as before, and
** 32bit DOS, Windows32 bit (Console), OS/2 32bit (Console).
** Untested on these platforms so far!
**
*/

/*
** Revision 2.01
**
** Colin Turner, 14th April 1999
**
** WinInit functions now use a common core function, which should reduce code
** size. The new core function also performs some simple validation.
**
*/
  
/*
** Revision 2.00
**
** Colin Turner, 22nd November 1998
**
** Split code into multiple CPP files for greater flexibility.
** Major consistency overhaul.
** Uses new naming for containers.
**
** Watcom Compliant
*/

/*
** Revision 1.00
**
*/

// Uncomment this to enable range checking
//#define PigletDesktop_RangeCheck


#ifndef PigletDesktop_H
  #define PigletDesktop_H

  // Standard Run Time Library                          
  #include <dos.h>
  #include <conio.h>
  #include <string.h>
  #include <stdarg.h>
  #include <stdlib.h>
  #include <bios.h>
  #include <stdio.h>
  #include <ctype.h>
  #include <assert.h>
  #include <time.h>
  
  // Compiler specific RTL
  #ifdef __BORLANDC__
    #include <alloc.h>
  #else
    // Watcom requires some different handling
    #include <i86.h>
    #include <malloc.h>
    #define coreleft() _memavl()
    #define bioskey(a) _bios_keybrd(a)
  #endif


  // Temporary
  #include <PigletOpSys.h>
  #include <PigletKeys.h>

  // Subject to new renaming
  #include <PigletDVList.h>

  #define Piglet_Far


// Declare some class names

// Basic desktop functionality

class PigletDesktop;
class PigletWindow;

// Vertical bar menus

class PigletVBarData;
class PigletVBarMenu;

// Horizontal bar menus

class PigletHBarData;
class PigletHBarMenu;

// Scrolling (vertical) menus, and tag-scrollable menus

class PigletScrollCols;
class PigletScrollLine;
class PigletScrollMenu;
class PigletTagSMenu;

// Dialog boxes

class PigletDialogStorage;
class PigletDialog;

// Template code

class PigletVMenuTemplate;

#pragma pack(1);

struct PigletVideoInformation {
  // Used to hold information on current video configuration
  char unused1[4];
  char mode;
  unsigned int cols;
  char unused2[27];
  char rows;
  char unused3[10];
  char status;
  char unused4[30];
};

#pragma pack();


struct PigletColourStruct {
  char MenuBar;             // FD mode only
  char StatLineTop;        // FD mode only
  char StatLineBottom;
  char Background;
  char InfoLine;
  char Header;
  char HeaderBorder;
  char DefWin;              // Defaults for window colours
  char DefWinBorder;
  char DefWinTitle;
  char DefWinBorderBGrnd;
  char InputAttr;           // for grabstring functions  color
  char OutputAttr;          // grabstring output
  char VBNormalAttr;
  char VBHighlightAttr;
};



/*
** Class        : PigletWindow
**
** Derived from : None
**
** Essentially a container for all the important data concerning a window.
**
*/
class PigletWindow
{
  // Data

private:

  int  X1, Y1, X2, Y2;
  int  CX, CY;
  int  BorderFlag;                // Border style
  char Piglet_Far * Buffer;       // Memory region containing window contents
  char Piglet_Far * Background;   // Background occulted by window
  char Piglet_Far * TopTitle;     // Text of title bar (if any)
  char Piglet_Far * BottomTitle;  // Text of bottom bar (if any)
  int  TJust;                     // Top bar justification
  int  BJust;                     // Bottom bar justification
  char BackAttr;                  // Background attribute
  char BorderAttr;                // Border text attribute
  char BorderBGrndAttr;           // Border attribute
  char TitleAttr;                 // Title attribute
  char BackCh;                    // Background fill character
  int  Open;                      // Window open indicator
  int  Handle;                    // Unique (in desktop) window identifier
  int  Shadow;                    // Shadow status indicator
  int  Locked;                    // Lock status indicator
  int  CursorScanStart;           // First cursor scan line
  int  CursorScanEnd;             // Last cursor scan line

  // Services

public:

  PigletWindow(void);
  PigletWindow(int cx1, int cy1, int cx2, int cy2 );
  int GetHandle(void);

  // Implementation

public:

  int operator == (PigletWindow & compare);

  // Enumeration

public:
  
  enum {
    SingleBorder = 0,
    DoubleBorder,
    NoBorder,

    ShadowOff = 0,
    ShadowOn,

    CursorBig = 0,
    CursorSmall,
    CursorOff,

    TitleLeft = 0,        // Justification and style of titles
    TitleCentre,
    TitleRight
  };

  // Friendships

  friend class PigletDesktop;
  friend class PigletVBarMenu;
  friend class PigletScrollMenu;
  friend class PigletDialog;
};



/*
** Class        : PigletDesktop
**
** Derived from : None
**
** Co-ordinates and oversees desktop functions. Maintains the list of windows
** and provides services to write into their contents and query the user.
**
** Also provides time slicing support in appropriate environments, and the
** automatic calling of functions at timed intervals.
**
*/
class PigletDesktop
{
  // Data

private:

  // OS Setup
  PigletOpSys * OS;                   // Operating system data (if not NULL)

  // Video setup
  int  VMode;                         // Current video mode; 7 - MDA
  int  VCols;                         // No. of screen columns eg. 80
  int  VRows;                         // No. of screen rows eg. 25, 43, 50
  char Piglet_Far * VBaseSeg;         // Video Memory either b800:0000 or b000:0000
  char Piglet_Far * StartBackground;

  char Piglet_Far * ident;

  // Basic cosmetics
  int  DesktopFlag;                   // Desktop style
  int  BorderFlag;                    // Default window border style
  int  NewHandle;                     // Handle the next window opened will use

  // Mouse handling
  int  Mouse;                         // Mouse installation indicator
  int  MouseActive;                   // Mouse activation indcator
  int  MouseX;                        // Mouse X Co-ordinate
  int  MouseY;                        // Mouse Y Co-ordinate
  int  MousePress;                    // Last "key" was a mouse press

  // Mechanics
  int  Active;                                // records the active window
  PigletDoubleVList<PigletWindow> WindowList; // List of windows in use
  char DesktopBackgroundChar;                  // desktop background character

  int StartX, StartY;                         // Starting cursor positions
  int Shutdown;                               // Dormant indicator

  int MTask;                                  // Multitasker in use
  
  // Idle Function handling
  void (*OnIdle) (PigletDesktop * d);         // Function pointer to call
  int  IdleDelay;                             // Delay in seconds between calls
  long LastIdleTime;                          // Unix time of last call

  int dh;                                     // Desktop Handle

  // Services

public:

  // Idle Function handling
  void RegisterIdleFunction( void (*p) (PigletDesktop * d));
  void SetIdleDelay(int idleTime);

  void SetOpSys(PigletOpSys * newOS);

  // Scrolling Window Contents
  void WinScrollUp(int window);
  void WinScrollDown(int window);

  void BetaCoreleft(void);

  void ShowIdent(void);

  int Desk(void) { return dh;}


  PigletColourStruct PigletColours;  // Color structure

  int IsActive(int handle)
  {
    return(GetActive()==handle);
  };

  int CheckKey();
  virtual int GetKey(int c1,...);   // Key input function
  int GKey(int flag);
  PigletDesktop(int desktop = PigletDesktop::FDDesktop, int border = PigletDesktop::SingleBorder, char bchar = '±');
  virtual ~PigletDesktop(void);

  int StartMouse(void);
  void MouseCursorOn(void);
  void MouseCursorOff(void);
  void MouseOn(void);
  void MouseOff(void);

  int CY(void);
  int Rows(void) { return VRows;}
  int Cols(void) { return VCols;}
  int GetActive(void);

  void WinFillRowChar(int handle, int x, int y, int length, char c, char attrib);
  void StatusLine(char *string);
  void StatusLine(char *string, int handle, int y);

  // Initialising Windows
  int  WinInit(int x1, int y1, int x2, int y2);
  int  WinInit(int x1, int y1, int x2, int y2, char Piglet_Far *top_title, int tjust, char Piglet_Far *bottom_title, int bjust);
  int  WinInitLock(int x1, int y1, int x2, int y2);
  int  WinInitLock(int x1, int y1, int x2, int y2, char Piglet_Far *top_title, int tjust, char Piglet_Far *bottom_title, int bjust);



  // Opening, Closing, Deleting Windows
  int  WinDraw(int active_handle);
  int  WinOpen(int active_handle);
  int  WinDrawOpen(int Active_Handle, int flag);
  void WinClose(int active_handle);
  int  WinDelete(int handle);
  void WinCloseForeground(void);


  // Setting characters and attributes in windows
  void SetWinBackgroundChar(int active_handle, char ch);
  void SetWinTitleAttr(int active_handle, char tattr);
  void SetWinBorder(int active_handle, int new_border_flag);
  void SetWinBorderAttr(int active_handle, char new_border_attr);
  void SetWinBorderBGrndAttr(int active_handle, char new_border_attr);
  void SetWinBackgroundAttr(int active_handle, char new_back_attr);

  // Setting window status
  void SetWinShadow(int active_handle, int shadow_status);
  void SetWinCursorType(int active_handle, int ctype);
  void WinLock(int active_handle);
  void WinUnlock(int active_handle);

  void WinGotoXY(int active_handle, int x, int y);

  // Line drawing in windows
  void WinLineX(int handle, int x, int y, int length, char attrib );
  void WinLineY(int handle, int x, int y, int length, char attrib, int type );
  void WinLineX(int handle, int x, int y, int length );
  void WinLineY(int handle, int x, int y, int length );
  void WinLineX(int handle, int x );
  void WinLineY(int handle, int y );
  void WinLineYStyle(int handle, int y, char attrib, int style );


  void WinWriteString(int active_handle, char *string);
  void WinWriteString(int active_handle, char *string, int x, int y);
  void WinWriteString(int active_string, char *string, char attrib);
  void WinWriteString(int active_handle, char *string, int x, int y, char attrib);
  void WinWriteString(int handle, char *string, int x, int y, char attrib, int length);

  void WinWriteStringFill(int handle, char *string, int x, int y, char attrib, int length);
  void WinWriteStringCentre(int active_handle, char *string, int y);
  void WinWriteBorder(int handle, char *string, int x, char attrib);

  void WinBorderLine(int handle, int x, int length);

  void WinWriteChar(int handle, char character, int x, int y);
  void WinWriteChar(int handle, char character, int x, int y, char attrib);

  int  WinXMax(int handle);
  int  WinYMax(int handle);
  int  WinCX(int handle);
  int  WinCY(int handle);

  void DesktopMessage(int y, char attrib, char *text);

  void BottomStatusLine(void);
  int GrabString(int handle, char *string, int x, int y, int type, int width, int slength);
  int GrabString(int handle, char *string, int x, int y, int type, int width, int slength, char input_attr, char output_attr);


  // Implementation

private:

  #ifdef PigletDesktop_RangeCheck
  void PigletDesktop::SubScriptError(int Code);
  #endif

  int  WinInit(int x1, int y1, int x2, int y2, char Piglet_Far *top_title, int tjust, char Piglet_Far *bottom_title, int bjust, int lock);
  PigletWindow * FindWindow(int Handle);
  void MemAbort(int code);
  void GetXY(void);
  char ReturnChar(int x, int y);

  void ScrollUp(int handle);

  void _GotoXY(int x, int y);
  void Cursor(int ch, int cl);
  void ClrWin(int x1, int y1, int x2, int y2, char attrib, char ch );
  void FillRowChar(int x, int y, int length, char attrib, char ch );
  void FillColChar(int x, int y, int length, char attrib, char ch );
  void FillRowAttr(int x, int y, int length, char attrib );
  void FillColAttr(int x, int y, int length, char attrib );
  void DrawFrame(int x1, int y1, int x2, int y2, char border, char Piglet_Far * top_title, int tjust, char Piglet_Far * bottom_title, int bjust, char title_attr, int border_style);
  void DrawWindow(int x1, int y1, int x2, int y2, int b_flag, char border, char attrib, char ch, char Piglet_Far * top_title, int tjust, char Piglet_Far * bottom_title, int bjust, char title_attr, int shadow );
  void DspMsg(int x, int y, char * text);
  void ColorMsg(int x, int y, char attrib, char Piglet_Far * text);
  void WinWriteStringCommon(int active_handle, char * string, char attrib, int type);

  int WDrawOpen(int active_handle, int flag);
  void DrawDesktop(void);


  PigletWindow * WriteBuffer(PigletWindow * w, int x, int y, char attrib, char * string);

  int IsObscured(int handle, int x, int y);
  void BufferScreen(PigletWindow * w, int x1, int y1, int x2, int y2);
  void RefreshScreen(PigletWindow * w);
  void UpdateShadow(void);
  PigletWindow * WriteRawV(PigletWindow * w, int x, int y, char * string);
  PigletWindow * WriteRawV(PigletWindow * w, int x, int y, char * string, char attrib);
  PigletWindow * WriteRawH(PigletWindow * w, int x, int y, char * string);
  PigletWindow * WriteRawH(PigletWindow * w, int x, int y, char * string, char attrib);


  // Enumeration

public:

  enum {
    BlankDesktop = 0,
    NoDesktop,
    FDDesktop,
    GEchoDesktop,

    SingleBorder = 0,
    DoubleBorder,
    NoBorder
  };

  // Friendships

  friend class PigletHBarMenu;
  friend class PigletVBarMenu;
  friend class PigletScrollMenu;
  friend class PigletDialog;

};



/*
** Class        : PigletVBarData
**
** Derived from : None
**
** Holds data for an individual vertical bar menu entry.
**
*/
class PigletVBarData
{
  // Data

public:

  char Text[200];                        // Text for menu
  char Describe[200];                    // Text for status line

  // Services

public:

  PigletVBarData(void);                      // Constructor
  int operator == (PigletVBarData & Compare);
};



/*
** Class        : PigletVBarMenu
**
** Derived from : None
**
** A vertical bar menu (non scrolling).
**
*/
class PigletVBarMenu
{
  // Data

private:

  char NormalAttrib;                          // Normal attribute for heading
  char HighlightAttrib;                       // Highlighted ....
  int  Handle;                                // Handle of window to display output on
  int  Widest;                                // Widest a menu bar can be
  int  X, Y;                                  // x,y location of start of menu
  int  Count;                                 // Number of menu options
  PigletDesktop * d;                         // The desktop
  PigletDoubleVList<PigletVBarData> BarList; // List of menu options
  int Selected;                              // Currently selected
  int Old;                                   // Last selected

  // Services

public:

  PigletVBarMenu(PigletDesktop & de, int w_handle, int sx, int sy); // Constructor
  
  void Add(char *menu, char * description); // Add a bar to the menu
  int  Go(void);                            // go go go lets do it !
  void Display(void);
  int  GetOption(void) { return Selected;};
  int  GetHandle(void) { return Handle;};
  int  LoadTemplate(FILE * tp, char * Ident);

};



/*
** Class        : PigletHBarData
**
** Derived from : None
**
** Holds data for an individual horizontal bar menu entry.
**
*/
class PigletHBarData
{
  // Data

public:
  
  char Data[50];                    // Text to display
  int X;                            // X Position
  PigletVBarMenu * Menu;            // Reference of vertical menu connected to option

  // Services

public:

  PigletHBarData(void);             // Constructor

  // Implementation

public:

  int operator == (PigletHBarData & compare);
};



/*
** Class        : PigletHBarMenu
**
** Derived from : None
**
** A horizontal bar menu which links to a vertical menu for each entry.
**
*/
class PigletHBarMenu
{
  // Data

private:

  char NormalAttrib;
  char HighlightAttrib;
  int  Count;                                  // No. of records
  PigletDesktop * d;
  PigletDoubleVList<PigletHBarData> BarList;  // Double list of g_hbardata
  int Selected;                               // Current option 
  int Old;                                    // Last Option
  int KeyPress;

  // Services

public:

  PigletHBarMenu(PigletDesktop & de);            // Constructor
  int  Go(void);                                 // Runs the menu
  void Display(void);                            // Displays the menu only
  int  GetOption(void);                          // Get the selected option
  int  Add(char *name, int x, PigletVBarMenu & zmenu);
  int  GetOld(void) { return Old;}
};



/*
** Class        : PigletScrollCols
**
** Derived from : None
**
** 
**
*/
class PigletScrollCols
{
  // Data

public:

  int    XStart;      // X offset to start of this column
  int    XEnd;        // X offset to end of this column
  int    Just;        // Justification for this column
  char * Title;       // (Optional) title for this column

  // Services

public:

  PigletScrollCols(void);
  int operator == (PigletScrollCols & compare);

};



/*
** Class        : PigletScrollLine
**
** Derived from : None
**
** 
**
*/
class PigletScrollLine
{
  // Data

public:

  char * Text;        // Text of entire line in scroll menu
  char Status; // For various derived classes

  // Services

public:

  PigletScrollLine(void);
  int operator == (PigletScrollLine & compare);
};



/*
** Class        : PigletScrollMenu
**
** Derived from : None
**
** 
**
*/
class PigletScrollMenu
{
  // Data

private:

  // Attributes

  char NormalAttr;             // Normal text attribute
  char HighlightAttr;          // Highlighted text attribute
  char BarAttr;                // Side bar normal attribute
  char BarHighlightAttr;       // Side bar highlight attribute
  char TitleAttr;              // Title attribute (if used)

  // Characters

  char TopChar;                // Up arrow character
  char BottomChar;             // Down arrow character
  char ShadingChar;            // Shading character in scroll bar
  char BlockChar;              // Block (position) character in scroll bar

protected:

  int TopPos;                 // Y Offset to first line in window for scroll menu
  int BottomPos;              // Y Offset to last line in window for scroll menu

  int OnScreen;               // Number on rows on screen
  int NoCols;                 // Number of columns on screen

  int ShowTitle;              // 0 - no title, any other value is the Y offset

  int BlockPos;               // Offset to the block position
  int OldBlockPos;            // Last offset to the block position

  int BarAlive;               // Undocumented

  PigletDoubleVList<PigletScrollCols> ColsList;
  PigletDoubleVList<PigletScrollLine> LineList;

  int Handle;                 // Handle of window used for the scroll menu
  PigletDesktop * d;          // Reference to desktop

  long Count;                 // Total number of lines in menu 
  long Pos;                   // Actual position
  long TopPosNumber;          // Top position

  unsigned int Char;          // used in go function to record keypress

  // Services

public:

  PigletScrollMenu(PigletDesktop & de, int whandle, int sp, int ep, int title, char nattr, char hattr, char battr, char bhattr, char tattr);
  ~PigletScrollMenu(void);

  int  AddColumn(char *string, int xs, int xe, int just);
  int  AddEntry(char *string,...);
  int  Go(int flag);
  long GetOption(void) {return Pos;}
  long Items(void) {return Count;}

  // Implementation

private:

  void DisplayTitle(void);
  void WritePage(void);
  void UpdatePage(int oldpos);
  void DrawBar(void);

protected:

  virtual int SendInfo(void);
  virtual int SendKeypress(void);
  virtual int SendAlienKeypress(void);
  virtual int SendChangeInfo(void);

  // Enumeration

public:

  enum{
    TextLeft = 0,
    TextRight,
    TextCentre
  };

  enum{
    ScrollOld = -3,
    ScrollBottom,
    ScrollTop
  };

};



/*
** Class        : PigletTagSMenu
**
** Derived from : PigletScrollMenu
**
** 
**
*/
class PigletTagSMenu : public PigletScrollMenu
{

  // Services

public:

  PigletTagSMenu(PigletDesktop & de, int whandle, int sp, int ep, int title, char nattr, char hattr, char battr, char bhattr, char tattr) : PigletScrollMenu(de, whandle, sp, ep, title, nattr, hattr, battr, bhattr, tattr)
  {
    AddColumn("", 2, 4, TextCentre);
  }

  void Set(long item);            // Set item
  void SetAll();                  // Set all items
  void Clear(long item);          // Clear item
  void ClearAll();                // Clear all
  void Toggle(long item);         // Toggle item
  long Get(long item);            // Get status on item
};



/*
** Class        : PigletDialogData
**
** Derived from : None
**
** 
**
*/
class PigletDialogData
{
  // Data

public:

  char * Text;
  int    X;
  int    Y;
  char   Letter;

  // Services

public:

  static PigletDialogData * ZERO;
  PigletDialogData(void);
  int operator == (PigletDialogData & compare);
};



/*
** Class        : PigletDialog
**
** Derived from : None
**
** 
**
*/
class PigletDialog
{
  // Data

private:

  PigletDesktop * d;
  int Handle;
  char NAttr;
  char LAttr;
  char HAttr;

  PigletDoubleVList<PigletDialogData> DialogList;
  int Pos;
  int Count;

  // Services

public:

  PigletDialog(PigletDesktop & desktop, int WindowHandle, char NormalAttr, char LetterAttr, char HighlightAttr);
  int  Add(char * text, int x, int y);
  void Display(void);
  int  Go(void);
  int  GetOption(void) {return Pos;}

};



/*
** Class        : PigletVMenuTemplate
**
** Derived from : None
**
** 
**
*/
class PigletVMenuTemplate
{
public:
  char           Ident[30];  // Unique identifier for this data set
  unsigned short xOffset,    // Offsets to menu display in window
  yOffset;
  unsigned char  DataLen,
  StatusLen;
  unsigned short Items;      // Number of g_MenuItemTemplate objects

  friend PigletVBarMenu::LoadTemplate(FILE * tp, char * Ident);
};


#endif

// End Of File

/**
** Piglet Productions
**
** FileName       : PigletOpSys.cpp
**
** Implements     : PigletOpSys
**
** 
**
** Description
**
**
**
**
** Initial Coding : Colin Turner
**
** Date           : 2/4/1999
**
**
** Copyright applies on this file, and distribution may be limited.
*/

/*
** Revision 1.00
**
*/

#include <PigletOpSys.h>

PigletOpSys::PigletOpSys()
{
  DetectMultiTasker();
}


char * PigletOpSys::GetName()
{
  return(OpSysName);
}


char * PigletOpSys::GetVersion()
{
  return(OpSysVersion);
}

void PigletOpSys::DetectMultiTasker()
{
  #if   defined(__NT__)
  OpSysCode = OS_Win32;
  return;
  #elif defined(__OS2__)
  OpSysCode = OS_OS2;
  return;
  #else

  unsigned short TrueDosVersion = 0;
  union REGS regs;

  // Get the True DOS Version
  #if defined(__WATCOMC__) && defined(__386__)
  regs.w.ax = 0x3306;
  int386(0x21, &regs, &regs);
  if (regs.h.bh < 100 && regs.h.bl >= 5) {
    TrueDosVersion = regs.w.bx;
  }
  #else
  regs.x.ax = 0x3306;
  int86(0x21, &regs, &regs);
  if (regs.h.bh < 100 && regs.h.bl >= 5 && regs.w.bx == 0x3205) {
    TrueDosVersion = regs.w.bx;
  }
  #endif

  // detect DESQview 
  #if defined(__WATCOMC__) && defined(__386__)
  regs.w.ax = 0x2B01;
  regs.w.cx = 0x4445;
  regs.w.dx = 0x5351;
  int386(0x21, &regs, &regs);
  #else
  regs.x.ax = 0x2B01;
  regs.x.cx = 0x4445;
  regs.x.dx = 0x5351;
  int86(0x21, &regs, &regs);
  #endif
  if (regs.h.al != 0xFF) {
    strcpy(OpSysName, "DESQview");
    OpSysCode = OS_DESQview;
    return;
  }

  // detect OS/2
  regs.h.ah = 0x30;
  #if defined(__WATCOMC__) && defined(__386__)
  int386(0x21, &regs, &regs);
  #else
  int86(0x21, &regs, &regs);
  #endif
  if (regs.h.al >= 10) {
    strcpy(OpSysName, "OS/2");
    OpSysCode = OS_OS2;
    return;
  }

  // detect Windows
  #if defined(__WATCOMC__) && defined(__386__)
  regs.w.ax = 0x160A;
  int386(0x2F, &regs, &regs);
  if (regs.w.ax == 0) {
    strcpy(OpSysName, "Windows");
    switch (regs.h.bh) {
      case 0x04: // Windows 9x
        strcat(OpSysName, " 9x");
        OpSysCode = OS_Win32;
        break;
      default:
        OpSysCode = OS_Win16;
        break;
    }
  }
  #else
  regs.x.ax = 0x160A;
  int86(0x2F, &regs, &regs);
  if (regs.w.ax == 0) {
    strcpy(OpSysName, "Windows");
    switch (regs.h.bh) {
      case 0x04: // Windows 9x
        strcat(OpSysName, " 9x");
        OpSysCode = OS_Win32;
        break;
      default:
        OpSysCode = OS_Win16;
        break;
    }
  }
  #endif

  // detect Windows NT
  if (TrueDosVersion == 0x3205) {
    strcpy(OpSysName, "Windows NT");
    OpSysCode = OS_WinNT;
    return;
  }

  // must be MS-DOS
  strcpy(OpSysName, "No Multitasker");
  OpSysCode = OS_DOS;

#endif

}


void PigletOpSys::TimeSlice(void)
{
  union REGS regs;

  switch (OpSysCode) {
    case OS_OS2:
    case OS_Win16:
    case OS_Win32:
    case OS_WinNT:
      #if defined(__WATCOMC__) && defined(__386__)
      regs.w.ax = 0x1680;
      int386(0x2F, &regs, &regs);
      #else
      regs.x.ax = 0x1680;
      int86(0x2F, &regs, &regs);
      #endif
      break;

    case OS_DESQview:
      #if defined(__WATCOMC__) && defined(__386__)
      regs.w.ax = 0x1000;
      int386(0x15, &regs, &regs);
      #else
      regs.x.ax = 0x1000;
      int86(0x15, &regs, &regs);
      #endif
      break;

    case OS_DoubleDOS:
      #if defined(__WATCOMC__) && defined(__386__)
      regs.w.ax = 0xEE01;
      int386(0x21, &regs, &regs);
      #else
      regs.x.ax = 0xEE01;
      int86(0x21, &regs, &regs);
      #endif
      break;

    default:
      break;
  }
}


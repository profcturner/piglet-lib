/****************************************************************************/
/*                                                                          */
/* Gold Development - FrontDoor <TM> Style log file                         */
/*                                                                          */
/* (c) 1996 Colin Turner                                                    */
/*                                                                          */
/* FileName        : G_LOG.H                                                */
/* Defined Classes : g_Log                                                  */
/*                                                                          */
/* This allows you to log information to an external text file.             */
/*                                                                          */
/****************************************************************************/

// Version 1.00

#ifndef Piglet_Log_H
#define Piglet_Log_H

#define PigletLog_FlushEvery       0x0001
#define PigletLog_EchoPrint        0x0002
#define PigletLog_EchoTime         0x0004

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <share.h>
#include <time.h>
#include <dos.h>
#include <ctype.h>
#include <malloc.h>

class PigletLog;

class PigletLog
{
  // Attributes
  protected:
	 FILE    *log;
	 char    Filename[72];
	 char    Extra[20];
	 char    Preline;
	 long    Flags;
	 int     DebugLevel, EchoLevel;
	 int     DateFormat;
	 char    CoDSep[2];
	 char    CoTSep[2];
	 char    Name[30];
	 char    Version[30];
	 char    Template[20];
	 int     Colour;

  public:
	 void SetFlags(long newflags)   { Flags=newflags; }
	 void SetDebug(int debuglevel)  { DebugLevel=debuglevel; }
	 int  GetDebug(void)            { return(DebugLevel); }
	 void SetEcho(int echolevel)    { EchoLevel=echolevel; }
	 void SetExtra(char *extra)     { strcpy(Extra, extra); }
	 void SetPreline(char preline)  { Preline=preline; }
	 void SetName(char *name)       { strcpy(Name, name); }
	 void SetVersion(char *version) { strcpy(Version, version); }
	 void SetFile(char *filename)   { strcpy(Filename, filename); }
	 char *GetFile(void)            { return(Filename); }
	 void SetColour(int colour)     { Colour=colour; }

  // Services
  public:
	 void Finish();
	 void Flush();
	 void Abnormal();
	 void Printf(char *string, ...);
	 void Printf(char preline, char *string, ...);
	 void Debugf(int level, char *string, ...);
	 void Debugf(int level, char preline, char *string, ...);
	 void VPrintf(int level, char *string, va_list ap);

  private:
	 virtual void EchoPrintf(char *string, ...);
	 float Extime(void);
	 void  StartLine(int level);

  // Implementation
  public:
	 // Constructors
	 PigletLog();
	 PigletLog(FILE *fp, char *name, char *version, char *extra, long flags);
	 PigletLog(char *filename, char *name, char *version, char *extra, long flags);

	 // Destructors
	 ~PigletLog() { Finish(); }

	 // Starting Class
	 int  Attach(FILE *fp) { log=fp; return(log!=NULL); }
	 int  Open();
	 void Start();

  private:
	 // Base Constuctor
	 void Constructor(char *name, char *version, char *extra, long flags);
	 void GetCountry();
	 int  IsSpecial(char test);

};

#endif // (Piglet_Log_H)
